#include "InGameScene.h"
#include "../Result/ResultScene.h"
#include "../SceneFactory.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/Vector2D.h"
#include "../../Objects/Enemy/Enemy.h"
#include "../../Objects/Player/Player.h"
#include "../../Objects/TurnManager.h"
#include <fstream>
#include <string>
#include <random>
#include <chrono>
//#define DEBUG_MODE		(0)

// UI���C�A�E�g
namespace {
	const int ICON_SPACING = 1; // �A�C�R���Ԃ̃X�y�[�X

	const int UI_FONT_SIZE = 32;

	// �e�L�X�g�ƃA�C�R���̐��������̒����I�t�Z�b�g�B
	// �t�H���g�T�C�Y�ƃA�C�R���T�C�Y�ɂ���Ē������K�v�ł��B
	// �e�L�X�g�̐^�񒆂ƃA�C�R���̐^�񒆂����킹��悤�Ȍv�Z (-24) �������Ă��܂��B
	const int ICON_TEXT_VERTICAL_ALIGN_OFFSET = -24;

	// �v���C���[1 UI�̊�ʒu (��ʍ���)
	const int P1_UI_BASE_X = 50;
	const int P1_UI_BASE_Y = 380;

	// �v���C���[2 UI�̊�ʒu (��ʉE��B��ʕ�1280�ɑ΂��Ē���)
	const int P2_UI_BASE_X = 800; // ���̒l�𒲐����āA�E�[����̋����𒲐�
	const int P2_UI_BASE_Y = 380;

	// P1 �p�X�\���ʒu
	const int P1_PASS_TEXT_X = P1_UI_BASE_X;
	const int P1_PASS_TEXT_Y = P1_UI_BASE_Y;
	const int P1_PASS_ICON_START_X = P1_PASS_TEXT_X + 80; // "�p�X:" �e�L�X�g����̃I�t�Z�b�g
	const int P1_PASS_ICON_Y = P1_PASS_TEXT_Y + ICON_TEXT_VERTICAL_ALIGN_OFFSET; // �e�L�X�g�ƃA�C�R���̐�������

	// P1 �񕜕\���ʒu
	const int P1_HEAL_TEXT_X = P1_UI_BASE_X;
	const int P1_HEAL_TEXT_Y = P1_UI_BASE_Y + 80; // �p�X�s�����Y�I�t�Z�b�g
	const int P1_HEAL_ICON_START_X = P1_HEAL_TEXT_X + 80;
	const int P1_HEAL_ICON_Y = P1_HEAL_TEXT_Y + ICON_TEXT_VERTICAL_ALIGN_OFFSET;

	// P2 �p�X�\���ʒu
	const int P2_PASS_TEXT_X = P2_UI_BASE_X;
	const int P2_PASS_TEXT_Y = P2_UI_BASE_Y;
	const int P2_PASS_ICON_START_X = P2_PASS_TEXT_X + 80;
	const int P2_PASS_ICON_Y = P2_PASS_TEXT_Y + ICON_TEXT_VERTICAL_ALIGN_OFFSET;

	// P2 �񕜕\���ʒu
	const int P2_HEAL_TEXT_X = P2_UI_BASE_X;
	const int P2_HEAL_TEXT_Y = P2_UI_BASE_Y + 80;
	const int P2_HEAL_ICON_START_X = P2_HEAL_TEXT_X + 80;
	const int P2_HEAL_ICON_Y = P2_HEAL_TEXT_Y + ICON_TEXT_VERTICAL_ALIGN_OFFSET;
}

InGameScene::InGameScene() :
	player1(nullptr),
	player2(nullptr),
	back_ground_image(-1),
	heal_icon_image(-1),
	pass_icon_image(-1),
	mainbgm(-1),
	warning_sound(-1),
	currentGameState(GameState::Initializing),
	initialTurnDecided(false),
	initialTurnDecisionTimer(0.0f),
	firstPlayer(Turn::Player1),
	isGameOver(false),
	transitionAlpha(0),
	transitionTimer(0)
{
	
}


void InGameScene::Initialize()
{

	Enemy::GetInstance()->Initialize();
	// �w�i�摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	TurnManager* turnManager = TurnManager::GetInstance();

	turnManager->Reset();

	std::vector<int> tmp;


	back_ground_image = rm->GetImageResource("Resource/Images/concrete-closed-room3.jpg")[0];

	pass_icon_image = rm->GetImageResource("Resource/Images/pass.png")[0];
	heal_icon_image = rm->GetImageResource("Resource/Images/heal.png")[0];
	start_flg = true;

	player1 = new Player(Player::PlayerID::Player1);
	player2 = new Player(Player::PlayerID::Player2);

	player1->Initialize();
	player2->Initialize();
	//LoadStageMapCSV();

	// �x�����o�̏�����
	warning_effect_active = false;
	warning_timer = 0.0f;
	result_reserved = false;
	is_warning = false;
	go_to_result = false;

	//SE��ǂݍ���
	mainbgm = rm->GetSoundResource("Resource/Sound/BGM/mainbgm.wav");
	warning_sound = rm->GetSoundResource("Resource/Sound/Warning.mp3");

	ChangeVolumeSoundMem(150, mainbgm);
	PlaySoundMem(mainbgm, DX_PLAYTYPE_LOOP);

	currentGameState = GameState::ReadyToStart; // ����������Ԃ���J�n
	initialTurnDecided = false;

	isGameOver = false;
	transitionAlpha = 0;
	transitionTimer = 0;
}

eSceneType InGameScene::Update(float delta_second)
{
	InputManager* input = InputManager::GetInstance();

	// Enemy�̍X�V
	Enemy::GetInstance()->Update();
	//Player::GetInstance()->Update();
	TurnManager* turnManager = TurnManager::GetInstance();

	Turn currentTurn = turnManager->GetCurrentTurn();

	

	if (turnManager->ShowTurnMessage() && currentGameState != GameState::TransitionToResult){ // turnMessageState != 0 �̎���true��Ԃ�
		turnManager->Update(delta_second);  
		if (!turnManager->ShowTurnMessage() && currentGameState == GameState::TurnDeciding) {
			currentGameState = GameState::Playing; // ���o���I�������v���C��Ԃ�
		}
		return eSceneType::eInGame;
	}

	switch (currentGameState) {
	case GameState::ReadyToStart:
		// �ǂ��炩�̃R���g���[���[�̃X�^�[�g�{�^���܂���Enter�L�[�ŃQ�[���J�n
		if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed ||
			input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
		{
			currentGameState = GameState::TurnDeciding; // ��U��U�����Ԃ�
			//initialTurnDecisionTimer = 2.0f; // 2�b�Ԍ��艉�o��\��
			PlaySoundMem(turn_start_sound, DX_PLAYTYPE_NORMAL); // ���o�J�nSE
		}
		break;

	case GameState::TurnDeciding:
		//initialTurnDecisionTimer -= delta_second;
		if (!initialTurnDecided) {
			// ��U��U�������_���Ɍ���i��x�������s�j
			std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
			std::uniform_int_distribution<int> dist(0, 1);
			if (dist(rng) == 0) {
				firstPlayer = Turn::Player1;
			}
			else {
				firstPlayer = Turn::Player2;
			}
			turnManager->SetInitialTurn(firstPlayer); // TurnManager�ɍŏ��̃^�[����ݒ肵�A���o���J�n
			initialTurnDecided = true;
		}
		break;

	case GameState::Playing:

		Enemy::GetInstance()->Update(); // �G�̍X�V
		if (player1) {
			player1->Update(); // �v���C���[1�̍X�V
		}
		if (player2) {
			player2->Update(); // �v���C���[2�̍X�V
		}

		if (Enemy::GetInstance()->IsDestroyed() && !result_reserved)
		{
			result_reserved = true; // ���U���g�J�ڗ\��
			Turn currentTurn = turnManager->GetCurrentTurn(); // ���݂̃^�[���v���C���[���擾
			Turn winningPlayer = (currentTurn == Turn::Player1) ? Turn::Player2 : Turn::Player1;

			// ���U���g�V�[���Ɍ��ʂ�`���� (false �̓Q�[���I�[�o�[�ł͂Ȃ� = ����)
			ResultScene::SetGameResult(false, winningPlayer);
			StartResultTransition(true); // �Q�[���I�[�o�[���o���J�n
			
		}

		
		// �x�����o���̃^�C�}�[����
		if (warning_effect_active)
		{
			warning_timer += delta_second;
			if (warning_timer >= warning_duration)
			{
				warning_effect_active = false;
				//return eSceneType::eResult;
			}
		}

		turnManager->Update(delta_second);
		break;

		if (currentTurn == Turn::Player1 && player1) {
			player1->Update();
		}
		else if (currentTurn == Turn::Player2 && player2) {
			player2->Update();
		}
		__super::Update(delta_second);

	case GameState::TransitionToResult:
		UpdateTransition(delta_second); // ���U���g�J�ډ��o�̍X�V
		// ���o������������ResultScene�֑J��
		if (transitionTimer <= 0) {
			return eSceneType::eResult;
		}
		break;

	default:
		break;
	}

		return GetNowSceneType();
	
}

void InGameScene::Draw() const
{
	SetFontSize(UI_FONT_SIZE);
	//�w�i�摜�̕`��;
	DrawGraph(0, 0, back_ground_image, TRUE);

	if (currentGameState == GameState::Playing || currentGameState == GameState::TransitionToResult) {
		// Enemy�̕`��
		Enemy::GetInstance()->Draw();

		if (player1) player1->Draw();
		if (player2) player2->Draw();
		//Player::GetInstance()->Draw();

		//DrawFormatString(10, 10, GetColor(255, 255, 255), "�C���Q�[���V�[��");

		// �^�[�����\��
		TurnManager* turnManager = TurnManager::GetInstance();
		Turn currentTurn = turnManager->GetCurrentTurn();

		const char* turnText = (currentTurn == Turn::Player1) ? "1P" : "2P";

		DrawFormatString(10, 10, GetColor(255, 255, 0), "�^�[��: %s", turnText);

		// �v���C���[1�̃A�C�R���`��
		if (currentTurn == Turn::Player1) {
			// �p�X�A�C�R���̃e�L�X�g�����[�v�O�ň�x�����`��
			DrawFormatString(P1_PASS_TEXT_X, P1_PASS_TEXT_Y, GetColor(255, 255, 255), "�p�X:");
			for (int i = 0; i < player1->GetPassCount(); ++i) {
				int x = P1_PASS_ICON_START_X + i * (iconWidth + ICON_SPACING);
				int y = P1_PASS_ICON_Y;
				DrawExtendGraph(x, y, x + iconWidth, y + iconHeight, pass_icon_image, TRUE);
			}
			// �񕜃A�C�R���̃e�L�X�g�����[�v�O�ň�x�����`��
			DrawFormatString(P1_HEAL_TEXT_X, P1_HEAL_TEXT_Y, GetColor(255, 255, 255), "��:");
			for (int i = 0; i < player1->GetHealCount(); ++i) {
				int x = P1_HEAL_ICON_START_X + i * (iconWidth + ICON_SPACING);
				int y = P1_HEAL_ICON_Y;
				DrawExtendGraph(x, y, x + iconWidth, y + iconHeight, heal_icon_image, TRUE);
			}
		}
		// ���C��: �v���C���[2�̃X�L���A�C�R���`��
		else if (currentTurn == Turn::Player2) {
			// �p�X�A�C�R���̃e�L�X�g�����[�v�O�ň�x�����`��
			DrawFormatString(P2_PASS_TEXT_X, P2_PASS_TEXT_Y, GetColor(255, 255, 255), "�p�X:");
			for (int i = 0; i < player2->GetPassCount(); ++i) {
				int x = P2_PASS_ICON_START_X + i * (iconWidth + ICON_SPACING);
				int y = P2_PASS_ICON_Y;
				DrawExtendGraph(x, y, x + iconWidth, y + iconHeight, pass_icon_image, TRUE);
			}
			// �񕜃A�C�R���̃e�L�X�g�����[�v�O�ň�x�����`��
			DrawFormatString(P2_HEAL_TEXT_X, P2_HEAL_TEXT_Y, GetColor(255, 255, 255), "��:");
			for (int i = 0; i < player2->GetHealCount(); ++i) {
				int x = P2_HEAL_ICON_START_X + i * (iconWidth + ICON_SPACING);
				int y = P2_HEAL_ICON_Y;
				DrawExtendGraph(x, y, x + iconWidth, y + iconHeight, heal_icon_image, TRUE);
			}
		}
	}
	

	if (currentGameState == GameState::ReadyToStart) {
		// �S��ʂ���������I�[�o�[���C��`�悵�āA�w�i�̂���������B��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200); // �������̍� (0-255, 0������, 255���s����)
		DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		SetFontSize(48); // �傫�߂̃t�H���g
		const char* msg = "PRESS START BUTTON";
		int string_width = GetDrawStringWidth(msg, strlen(msg));
		DrawString(640 - string_width / 2, 300, msg, GetColor(255, 255, 255));
		SetFontSize(16); // �t�H���g�T�C�Y�����ɖ߂�
	}
	else if (currentGameState == GameState::TurnDeciding) {
		// "��U��U�����蒆..." �̌Œ胁�b�Z�[�W�́ATurnManager�̉��o�Əd�����邽�ߕ`�悵�Ȃ��B
		// TurnManager::DrawTurnMessageOverlay() ���u1P�̐�U�I�v�u2P�̐�U�I�v���A�j���[�V�����\������B
	}
	else if (currentGameState == GameState::TransitionToResult) {
		// �J�ډ��o�̕`�� (���̕`��̏�ɏd�˂�)
		DrawTransition();
	}

	
	TurnManager::GetInstance()->DrawTurnMessageOverlay();

	if (warning_effect_active) {
		ClearDrawScreen();
		DrawGraph(0, 0, back_ground_image, TRUE);

		int alpha = static_cast<int>((sin(warning_timer * 5.0f) + 1.0f) * 0.5f * 150);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawBox(0, 0, 1280, 720, GetColor(225, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void InGameScene::Finalize()
{
	

	if (mainbgm != -1) {
		StopSoundMem(mainbgm);
	}

	DeleteSoundMem(mainbgm);
	DeleteSoundMem(warning_sound);
	DeleteSoundMem(turn_start_sound);

	delete player1;
	player1 = nullptr;
	delete player2;
	player2 = nullptr;

	// ���I�z��̉��
}

void InGameScene::StartResultTransition(bool gameOver) {
	currentGameState = GameState::TransitionToResult;
	isGameOver = gameOver; // �Q�[���I�[�o�[��������
	transitionAlpha = 0;   // ���������x
	transitionTimer = 90;  // �J�ډ��o�̎������ԁi1.5�b = 90�t���[���j

	// BGM�̒�~
	if (mainbgm != -1) {
		StopSoundMem(mainbgm);
	}

	// �Q�[���I�[�o�[/�������̍Đ� (�K�v�ł����)
	 ResourceManager* rm = ResourceManager::GetInstance();
	 if (isGameOver) {
	     PlaySoundMem(rm->GetSoundResource("Resource/Sound/Warning-Siren05-03(Fast-Short).mp3"), DX_PLAYTYPE_NORMAL);
	 }
}

void InGameScene::UpdateTransition(float delta_second) {
	// transitionTimer�̓t���[�����Ƃ��Ĉ������߁AdeltaTime�͒��ڎg�p����
	// 1�t���[��������1�����炷�Ɖ���
	transitionTimer--;

	if (transitionTimer > 60) { // �ŏ���30�t���[���Ńt�F�[�h�C�� (90-60 = 30�t���[��)
		transitionAlpha += static_cast<int>(255 / 30.0f); // 30�t���[����255��
		if (transitionAlpha > 255) transitionAlpha = 255;
	}
	else if (transitionTimer <= 0) { // ���o�I��
		transitionTimer = 0; // 0�ȉ��ɂ͂Ȃ�Ȃ��悤��
		// �V�[���J�ڂ�InGameScene::Update()���ōs��
	}
	else { // ��ʂ����S�ɍ�����Ԃł̃��b�Z�[�W�\���Ȃ� (60�t���[��)
		// �Ⴆ�΁A�����Ń��b�Z�[�W�����S�ɕ\������Ă��鎞�Ԃ��m�ۂ���
		transitionAlpha = 255; // ���S�ɍ����ۂ�
	}
}

void InGameScene::DrawTransition() const {
	// ��ʑS�̂������t�F�[�h�C��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, transitionAlpha);
	DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE); // ��ʃT�C�Y�ɍ��킹�Ē���
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ��ʂ����S�ɍ����Ƃ��Ƀ��b�Z�[�W��\�� (��: transitionAlpha��255�ɋ߂��Ƃ�)
	if (transitionAlpha > 200 && transitionTimer > 0 && transitionTimer <= 60) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255); // ���b�Z�[�W�͕s������
		SetFontSize(80); // �傫�ȃt�H���g
		const char* message = isGameOver ? "GAME OVER" : "WIN!";
		unsigned int color = isGameOver ? GetColor(255, 50, 50) : GetColor(50, 255, 50); // �Ԃ���
		int string_width = GetDrawStringWidth(message, strlen(message));
		DrawString(640 - string_width / 2, 300, message, color);
		SetFontSize(16); // ���ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

eSceneType InGameScene::GetNowSceneType() const
{
	return eSceneType::eInGame;
}
