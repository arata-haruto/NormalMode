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

    // ターン切り替え演出中は操作できない
    if (turnManager->ShowTurnMessage()) {
        return;
    }

    // 攻撃
    if (input->GetKeyState(KEY_INPUT_Z) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_B) == eInputState::Pressed)
    {
        Enemy::GetInstance()->TakeDamage(10);
        printfDx("Player %d が攻撃！\n", playerID == PlayerID::Player1 ? 1 : 2);

        // ターン終了
        turnManager->NextTurn();
    }

    //回復
    if (healCount > 0 &&
       (input->GetKeyState(KEY_INPUT_X) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)) 
    {

        Enemy::GetInstance()->Heal(40);
        healCount --;
        turnManager->NextTurn();
    }

    //パス
    if (passCount > 0 &&
        (input->GetKeyState(KEY_INPUT_C) == eInputState::Pressed ||
            input->GetButtonState(XINPUT_BUTTON_X) == eInputState::Pressed))
    {
        passCount --;
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