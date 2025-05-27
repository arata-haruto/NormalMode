#include "Player.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/InputManager.h"
#include "../Enemy/Enemy.h"
#include "../TurnManager.h"
#include "DxLib.h"

Player::Player(PlayerID id) : playerID(id), attack_power(20), selectedPartIndex(0)
{
}

Player::~Player() 
{
}

void Player::Initialize() 
{
    ResourceManager* rm = ResourceManager::GetInstance();
}

void Player::Update() {
    TurnManager* turnManager = TurnManager::GetInstance();
    InputManager* input = InputManager::GetInstance();

    // 自分のターンかチェック
    if ((playerID == PlayerID::Player1 && turnManager->GetCurrentTurn() != Turn::Player1) ||
        (playerID == PlayerID::Player2 && turnManager->GetCurrentTurn() != Turn::Player2))
    {
        return; // 自分のターンじゃない
    }

    // 攻撃入力
    if (input->GetKeyState(KEY_INPUT_Z) == eInputState::Pressed) {
        Enemy::GetInstance()->TakeDamage(20);
        printfDx("Player %d が攻撃！\n", playerID == PlayerID::Player1 ? 1 : 2);

        // ターン終了
        turnManager->NextTurn();
    }
}

void Player::Draw() const {
    DrawFormatString(50, 400, GetColor(255, 255, 255),
        "Z: Attack  ←→: Change Part (Selected: %d)", selectedPartIndex);
    __super::Draw();
}

//Player* Player::GetInstance() {
//    if (!instance) {
//        instance = new Player();
//    }
//    return instance;
//}