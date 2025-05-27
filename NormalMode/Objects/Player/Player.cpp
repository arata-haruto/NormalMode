#include "Player.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/InputManager.h"
#include "../Enemy/Enemy.h"
#include "DxLib.h"

Player::Player() : attack_power(20), selectedPartIndex(0)
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
    InputManager* input = InputManager::GetInstance();

    if (input->GetKeyState(KEY_INPUT_Z) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_B) == eInputState::Pressed)
    {
        Enemy::GetInstance()->TakeDamage(20);
        printfDx("EnemyÇ…çUåÇÅI\n");
    }
    
}

void Player::Draw() const {
    DrawFormatString(50, 400, GetColor(255, 255, 255),
        "Z: Attack  Å©Å®: Change Part (Selected: %d)", selectedPartIndex);
    __super::Draw();
}

//Player* Player::GetInstance() {
//    if (!instance) {
//        instance = new Player();
//    }
//    return instance;
//}