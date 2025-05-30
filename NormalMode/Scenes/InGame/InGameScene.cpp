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

//#define DEBUG_MODE		(0)

void InGameScene::Initialize()
{
	Enemy::GetInstance()->Initialize();
	// 背景画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int> tmp;


	back_ground_image = rm->GetImageResource("Resource/Images/concrete-closed-room02.jpg")[0];

	pass_icon_image = rm->GetImageResource("Resource/Images/pass.png")[0];
	heal_icon_image = rm->GetImageResource("Resource/Images/heal.png")[0];
	start_flg = true;

	player1 = new Player(Player::PlayerID::Player1);
	player2 = new Player(Player::PlayerID::Player2);

	player1->Initialize();
	player2->Initialize();
	//LoadStageMapCSV();

	//SEを読み込む
	//mainbgm = rm->GetSoundResource("Resource/Sounds/BGM/mainbgm.mp3");
}

eSceneType InGameScene::Update(float delta_second)
{
	InputManager* input = InputManager::GetInstance();

	/*if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
	{
		return eSceneType::eTitle;
	}*/

	// Enemyの更新
	Enemy::GetInstance()->Update();
	//Player::GetInstance()->Update();
	TurnManager* turnManager = TurnManager::GetInstance();
	Turn currentTurn = turnManager->GetCurrentTurn();


	if (Enemy::GetInstance()->IsDestroyed())
	{
		return eSceneType::eResult;
	}

	turnManager->Update(delta_second);

	if (currentTurn == Turn::Player1 && player1) {
		player1->Update();
	}
	else if (currentTurn == Turn::Player2 && player2) {
		player2->Update();
	}
		__super::Update(delta_second);

	return GetNowSceneType();
}

void InGameScene::Draw() const
{
	//背景画像の描画;
	DrawGraph(0, 0, back_ground_image, TRUE);

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
			DrawFormatString(5, 440, GetColor(255, 255, 255), "パス:");

			int x = 80 + i * 40; // 左側に並べる
			int y = 415; // 画面の下部に配置
			DrawExtendGraph(x, y, x + 90, y + 90, pass_icon_image, TRUE);
		}

		for (int i = 0; i < player1->GetHealCount(); ++i) {
			int x = 50 + i * (90 + 10);
			int y = 370; // パスアイコンの少し下に配置
			DrawExtendGraph(x, y, x + 64, y + 64, heal_icon_image, TRUE);
		}
	}
	// プレイヤー2のアイコン描画
	if (currentTurn == Turn::Player2) {

		for (int i = 0; i < player2->GetPassCount(); ++i) {
			DrawFormatString(450, 440, GetColor(255, 255, 255), "パス:");

			int x = 525 + i * 40;
			int y = 415; 
			DrawExtendGraph(x, y, x + 90, y + 90, pass_icon_image, TRUE);
		}
		for (int i = 0; i < player2->GetHealCount(); ++i) {
			int x = 495 + i * (90 + 10);
			int y = 370;
			DrawExtendGraph(x, y, x + 64, y + 64, heal_icon_image, TRUE);
		}
	}

	if (turnManager->ShowTurnMessage()) {
		const std::string& msg = turnManager->GetTurnMessage();

		// 画面中央にテキスト表示
		int screenWidth = 640;
		int screenHeight = 480;

		int textX = screenWidth / 2 - 80;
		int textY = screenHeight / 2 - 20;

		DrawBox(textX - 10, textY - 10, textX + 200, textY + 40, GetColor(0, 0, 0), TRUE);
		DrawString(textX, textY, msg.c_str(), GetColor(255, 255, 255));

		__super::Draw();
	}
}

void InGameScene::Finalize()
{
	player = nullptr;

	if (player1) {
		delete player1;
		player1 = nullptr;
	}
	if (player2) {
		delete player2;
		player2 = nullptr;
	}

	// 動的配列の解放
}

eSceneType InGameScene::GetNowSceneType() const
{
	return eSceneType::eInGame;
}
