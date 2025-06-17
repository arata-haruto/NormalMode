#include "TitleScene.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/Vector2D.h"
#include "DxLib.h"

void TitleScene::Initialize()
{
	__super::Initialize();

	// �w�i�摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int> tmp;


	back_ground_image = rm->GetImageResource("Resource/Images/concrete-closed-room1.jpg")[0];

	select_menu = ePLAY;

	pickup_color = GetColor(255, 0, 0);
	dropoff_color = GetColor(255, 255, 255);

	cursor_sound = rm->GetSoundResource("Resource/Sound/�J�[�\���ړ�5.mp3");
	decision_sound = rm->GetSoundResource("Resource/Sound/����{�^��������7.mp3");

	if (cursor_sound == -1 || decision_sound == -1) {
		printfDx("Error: Failed to load TitleScene sounds.\n");
	}
}

eSceneType TitleScene::Update(float delta_second)
{
	InputManager* input = InputManager::GetInstance();

	//���L�[�������ꂽ�ꍇ
	if (input->GetKeyState(KEY_INPUT_DOWN) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::Pressed)
	{
		SetDownSelectMenuType();
		PlaySoundMem(cursor_sound, DX_PLAYTYPE_BACK);
	}

	//��L�[�������ꂽ�ꍇ
	if (input->GetKeyState(KEY_INPUT_UP) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_DPAD_UP) == eInputState::Pressed)
	{
		SetUpSelectMenuType();
		PlaySoundMem(cursor_sound, DX_PLAYTYPE_BACK);
	}

	//���菈��
	if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
	{
		PlaySoundMem(decision_sound, DX_PLAYTYPE_BACK);
		switch (select_menu)
		{
		case ePLAY:
			return eSceneType::eInGame;
			break;

		case eHELP:
			return eSceneType::eHelp;
			break;

		case eEXIT:
			return eSceneType::eExit;
			break;

		default:
			return eSceneType::eTitle;
			break;
		}
	}

	return GetNowSceneType();
}

void TitleScene::Draw() const
{
	//�^�C�g���w�i�摜�̕`��;
	DrawGraph(0, 0, back_ground_image, TRUE);

	SetFontSize(60);

	DrawFormatString(500, 100, GetColor(255, 255, 255), "Lethal Judge");

	SetFontSize(40);

	//�I������Ă��郁�j���[�ɂ���ĐF��ς���
	switch (select_menu)
	{
	case eNONE:
		DrawFormatString(510, 350, dropoff_color, "PLAY START");
		DrawFormatString(510, 400, dropoff_color, "HELP");
		DrawFormatString(510, 450, dropoff_color, "EXIT");
		break;

	case ePLAY:
		DrawFormatString(510, 350, pickup_color, "PLAY START");
		DrawFormatString(510, 400, dropoff_color, "HELP");
		DrawFormatString(510, 450, dropoff_color, "EXIT");
		break;

	case eHELP:
		DrawFormatString(510, 350, dropoff_color, "PLAY START");
		DrawFormatString(510, 400, pickup_color, "HELP");
		DrawFormatString(510, 450, dropoff_color, "EXIT");
		break;

	case eEXIT:
		DrawFormatString(510, 350, dropoff_color, "PLAY START");
		DrawFormatString(510, 400, dropoff_color, "HELP");
		DrawFormatString(510, 450, pickup_color, "EXIT");
		break;
	default:
		break;
	}

	__super::Draw();
}

void TitleScene::Finalize()
{
	__super::Finalize();
	
	if (back_ground_image != -1) {
		DeleteGraph(back_ground_image);
		back_ground_image = -1;
	}
	
	if (cursor_sound != -1) {
		DeleteSoundMem(cursor_sound);
		cursor_sound = -1;
	}
	if (decision_sound != -1) {
		DeleteSoundMem(decision_sound);
		decision_sound = -1;
	}
}

eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::eTitle;
}

//�J�[�\���㏸����
void TitleScene::SetDownSelectMenuType()
{
	switch (select_menu)
	{
	case eNONE:
		select_menu = ePLAY;
		break;

	case ePLAY:
		select_menu = eHELP;
		break;

	case eHELP:
		select_menu = eEXIT;
		break;

	case eEXIT:

		break;
	default:
		break;
	}
}

//�J�[�\�����~����
void TitleScene::SetUpSelectMenuType()
{
	switch (select_menu)
	{
	case eNONE:
		break;

	case ePLAY:
		break;

	case eHELP:
		select_menu = ePLAY;
		break;

	case eEXIT:
		select_menu = eHELP;
		break;

	default:
		break;
	}
}