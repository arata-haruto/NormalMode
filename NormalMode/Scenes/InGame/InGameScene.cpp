#include "InGameScene.h"
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
	//Player::GetInstance()->Initialize();

	// ”wŒi‰æ‘œ‚Ì“Ç‚İ‚İ
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int> tmp;


	back_ground_image = rm->GetImageResource("Resource/Images/concrete-closed-room02.jpg")[0];

	start_flg = true;

	player1 = new Player(Player::PlayerID::Player1);
	player2 = new Player(Player::PlayerID::Player2);

	player1->Initialize();
	player2->Initialize();
	//LoadStageMapCSV();

	//SE‚ğ“Ç‚İ‚Ş
	//mainbgm = rm->GetSoundResource("Resource/Sounds/BGM/mainbgm.mp3");
}

eSceneType InGameScene::Update(float delta_second)
{
	InputManager* input = InputManager::GetInstance();

	if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
	{
		return eSceneType::eTitle;
	}

	// Enemy‚ÌXV
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
		__super::Update(delta_second);

	return GetNowSceneType();
}

void InGameScene::Draw() const
{
	//”wŒi‰æ‘œ‚Ì•`‰æ;
	DrawGraph(0, 0, back_ground_image, TRUE);

	// Enemy‚Ì•`‰æ
	Enemy::GetInstance()->Draw();

	//Player::GetInstance()->Draw();

	DrawFormatString(10, 10, GetColor(255, 255, 255), "ƒCƒ“ƒQ[ƒ€ƒV[ƒ“");
	DrawFormatString(70, 360, GetColor(255, 255, 255), "UI");

	// ƒ^[ƒ“î•ñ•\¦
	TurnManager* turnManager = TurnManager::GetInstance();
	Turn currentTurn = turnManager->GetCurrentTurn();

	const char* turnText = (currentTurn == Turn::Player1) ? "1P" : "2P";

	DrawFormatString(10, 50, GetColor(255, 255, 0), "Turn: %s", turnText);

	__super::Draw();
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

	// “®“I”z—ñ‚Ì‰ğ•ú
}

eSceneType InGameScene::GetNowSceneType() const
{
	return eSceneType::eInGame;
}
