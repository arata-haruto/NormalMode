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
	// 背景画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	TurnManager* turnManager = TurnManager::GetInstance();

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

	// 警告演出の初期化
	warning_effect_active = false;
	warning_timer = 0.0f;
	result_reserved = false;
	is_warning = false;
	go_to_result = false;

	//SEを読み込む
	mainbgm = rm->GetSoundResource("Resource/Sound/BGM/mainbgm.wav");
	warning_sound = rm->GetSoundResource("Resource/Sound/Warning.mp3");

	ChangeVolumeSoundMem(150, mainbgm);
	PlaySoundMem(mainbgm, DX_PLAYTYPE_LOOP);

	currentGameState = GameState::ReadyToStart; // 準備完了状態から開始
	initialTurnDecided = false;

	isGameOver = false;
	transitionAlpha = 0;
	transitionTimer = 0;
}

eSceneType InGameScene::Update(float delta_second)
{
	InputManager* input = InputManager::GetInstance();

	// Enemyの更新
	Enemy::GetInstance()->Update();
	//Player::GetInstance()->Update();
	TurnManager* turnManager = TurnManager::GetInstance();

	Turn currentTurn = turnManager->GetCurrentTurn();

	if (currentTurn == Turn::Player1 && player1) {
		player1->Update();
	}
	else if (currentTurn == Turn::Player2 && player2) {
		player2->Update();
	}

	if (turnManager->ShowTurnMessage() && currentGameState != GameState::TransitionToResult){ // turnMessageState != 0 の時にtrueを返す
		turnManager->Update(delta_second);  
		if (!turnManager->ShowTurnMessage() && currentGameState == GameState::TurnDeciding) {
			currentGameState = GameState::Playing; // 演出が終わったらプレイ状態へ
		}
		return eSceneType::eInGame;
	}

	switch (currentGameState) {
	case GameState::ReadyToStart:
		// どちらかのコントローラーのスタートボタンまたはEnterキーでゲーム開始
		if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed ||
			input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Pressed)
		{
			currentGameState = GameState::TurnDeciding; // 先攻後攻決定状態へ
			//initialTurnDecisionTimer = 2.0f; // 2秒間決定演出を表示
			PlaySoundMem(turn_start_sound, DX_PLAYTYPE_NORMAL); // 演出開始SE
		}
		break;

	case GameState::TurnDeciding:
		//initialTurnDecisionTimer -= delta_second;
		if (!initialTurnDecided) {
			// 先攻後攻をランダムに決定（一度だけ実行）
			std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
			std::uniform_int_distribution<int> dist(0, 1);
			if (dist(rng) == 0) {
				firstPlayer = Turn::Player1;
			}
			else {
				firstPlayer = Turn::Player2;
			}
			turnManager->SetInitialTurn(firstPlayer); // TurnManagerに最初のターンを設定し、演出を開始
			initialTurnDecided = true;
		}
		break;

	case GameState::Playing:
		if (Enemy::GetInstance()->IsDestroyed() && !result_reserved)
		{
			result_reserved = true; // リザルト遷移予約
			StartResultTransition(true); // ゲームオーバー演出を開始
			// 警告演出を開始
			//warning_effect_active = true;
			//warning_timer = 0.0f; // 1秒点滅
			//result_reserved = true;
			//PlaySoundMem(warning_sound, DX_PLAYTYPE_BACK);
			//return eSceneType::eResult;
		}

		
		// 警告演出中のタイマー処理
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
		UpdateTransition(delta_second); // リザルト遷移演出の更新
		// 演出が完了したらResultSceneへ遷移
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
	//背景画像の描画;
	DrawGraph(0, 0, back_ground_image, TRUE);

	if (currentGameState == GameState::Playing || currentGameState == GameState::TransitionToResult) {
		// Enemyの描画
		Enemy::GetInstance()->Draw();

		if (player1) player1->Draw();
		if (player2) player2->Draw();
		//Player::GetInstance()->Draw();

		//DrawFormatString(10, 10, GetColor(255, 255, 255), "インゲームシーン");

		// ターン情報表示
		TurnManager* turnManager = TurnManager::GetInstance();
		Turn currentTurn = turnManager->GetCurrentTurn();

		const char* turnText = (currentTurn == Turn::Player1) ? "1P" : "2P";

		DrawFormatString(10, 10, GetColor(255, 255, 0), "ターン: %s", turnText);

		// プレイヤー1のアイコン描画
		if (currentTurn == Turn::Player1) {

			for (int i = 0; i < player1->GetPassCount(); ++i) {
				DrawFormatString(100, 100, GetColor(255, 255, 255), "パス:");
				int x = 180 + i * (iconWidth + 10); // アイコンの間隔調整
				int y = 75;
				DrawExtendGraph(x, y, x + iconWidth, y + iconHeight, pass_icon_image, TRUE);
			}
			for (int i = 0; i < player1->GetHealCount(); ++i) {
				DrawFormatString(100, 150, GetColor(255, 255, 255), "回復:");
				int x = 180 + i * (64 + 10); // アイコンの間隔調整
				int y = 125;
				DrawExtendGraph(x, y, x + 64, y + 64, heal_icon_image, TRUE);
			}
		}
		// プレイヤー2のアイコン描画
		if (currentTurn == Turn::Player2) {

			for (int i = 0; i < player2->GetPassCount(); ++i) {
				DrawFormatString(450, 100, GetColor(255, 255, 255), "パス:");
				int x = 525 + i * (iconWidth + 10); // アイコンの間隔調整
				int y = 75;
				DrawExtendGraph(x, y, x + iconWidth, y + iconHeight, pass_icon_image, TRUE);
			}
			for (int i = 0; i < player2->GetHealCount(); ++i) {
				DrawFormatString(450, 150, GetColor(255, 255, 255), "回復:");
				int x = 525 + i * (64 + 10); // アイコンの間隔調整
				int y = 125;
				DrawExtendGraph(x, y, x + 64, y + 64, heal_icon_image, TRUE);
			}
		}
	}
	//if (turnManager->ShowTurnMessage()) {
	//	const std::string& msg = turnManager->GetTurnMessage();

	//	// 画面中央にテキスト表示
	//	int screenWidth = 1280;
	//	int screenHeight = 720;

	//	int textX = screenWidth / 2 - 80;
	//	int textY = screenHeight / 2 - 20;

	//	DrawBox(textX - 10, textY - 10, textX + 200, textY + 40, GetColor(0, 0, 0), TRUE);
	//	DrawString(textX, textY, msg.c_str(), GetColor(255, 255, 255));

	//	__super::Draw();
	//}

	if (currentGameState == GameState::ReadyToStart) {
		// 全画面を黒くするオーバーレイを描画して、背景のごちゃつきを隠す
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200); // 半透明の黒 (0-255, 0が透明, 255が不透明)
		DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		SetFontSize(48); // 大きめのフォント
		const char* msg = "PRESS START BUTTON";
		int string_width = GetDrawStringWidth(msg, strlen(msg));
		DrawString(640 - string_width / 2, 300, msg, GetColor(255, 255, 255));
		SetFontSize(16); // フォントサイズを元に戻す
	}
	else if (currentGameState == GameState::TurnDeciding) {
		// "先攻後攻を決定中..." の固定メッセージは、TurnManagerの演出と重複するため描画しない。
		// TurnManager::DrawTurnMessageOverlay() が「1Pの先攻！」「2Pの先攻！」をアニメーション表示する。
	}
	else if (currentGameState == GameState::TransitionToResult) {
		// 遷移演出の描画 (他の描画の上に重ねる)
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

	// 動的配列の解放
}

void InGameScene::StartResultTransition(bool gameOver) {
	currentGameState = GameState::TransitionToResult;
	isGameOver = gameOver; // ゲームオーバーか勝利か
	transitionAlpha = 0;   // 初期透明度
	transitionTimer = 90;  // 遷移演出の持続時間（1.5秒 = 90フレーム）

	// BGMの停止
	if (mainbgm != -1) {
		StopSoundMem(mainbgm);
	}

	// ゲームオーバー/勝利音の再生 (必要であれば)
	// ResourceManager* rm = ResourceManager::GetInstance();
	// if (isGameOver) {
	//     PlaySoundMem(rm->GetSoundResource("Resource/Sound/se_gameover.ogg")[0], DX_PLAYTYPE_NORMAL);
	// } else {
	//     PlaySoundMem(rm->GetSoundResource("Resource/Sound/se_win.ogg")[0], DX_PLAYTYPE_NORMAL);
	// }
}

void InGameScene::UpdateTransition(float delta_second) {
	// transitionTimerはフレーム数として扱うため、deltaTimeは直接使用せず
	// 1フレームあたり1ずつ減らすと仮定
	transitionTimer--;

	if (transitionTimer > 60) { // 最初の30フレームでフェードイン (90-60 = 30フレーム)
		transitionAlpha += static_cast<int>(255 / 30.0f); // 30フレームで255に
		if (transitionAlpha > 255) transitionAlpha = 255;
	}
	else if (transitionTimer <= 0) { // 演出終了
		transitionTimer = 0; // 0以下にはならないように
		// シーン遷移はInGameScene::Update()側で行う
	}
	else { // 画面が完全に黒い状態でのメッセージ表示など (60フレーム)
		// 例えば、ここでメッセージが完全に表示されている時間を確保する
		transitionAlpha = 255; // 完全に黒く保つ
	}
}

void InGameScene::DrawTransition() const {
	// 画面全体を黒くフェードイン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, transitionAlpha);
	DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE); // 画面サイズに合わせて調整
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 画面が完全に黒いときにメッセージを表示 (例: transitionAlphaが255に近いとき)
	if (transitionAlpha > 200 && transitionTimer > 0 && transitionTimer <= 60) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255); // メッセージは不透明に
		SetFontSize(80); // 大きなフォント
		const char* message = isGameOver ? "GAME OVER" : "WIN!";
		unsigned int color = isGameOver ? GetColor(255, 50, 50) : GetColor(50, 255, 50); // 赤か緑
		int string_width = GetDrawStringWidth(message, strlen(message));
		DrawString(640 - string_width / 2, 300, message, color);
		SetFontSize(16); // 元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

eSceneType InGameScene::GetNowSceneType() const
{
	return eSceneType::eInGame;
}
