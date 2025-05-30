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

    heal_sound = rm->GetSoundResource("Resource/Sound/ゲージ回復1.wav");
    pass_sound = rm->GetSoundResource("Resource/Sound/鞭を振り回す1.wav");
    attack_sound = rm->GetSoundResource("Resource/Sound/剣で斬る2.wav");
    beep_sound = rm->GetSoundResource("Resource/Sound/ビープ音1.wav");

   /* heal_image = rm->GetImageResource("Resource/Images/healcount.png")[0];
    pass_image = rm->GetImageResource("Resource/Images/passcount.png")[0];*/
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

    if (input->GetKeyState(KEY_INPUT_X) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_Y) == eInputState::Pressed)
    {
        //Healを呼び出す
        Heal(40);
    }

    if (input->GetKeyState(KEY_INPUT_C) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_X) == eInputState::Pressed)
    {
        //Passを呼び出す
        Pass();
    }

    // 攻撃
    if (input->GetKeyState(KEY_INPUT_Z) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_B) == eInputState::Pressed)
    {
        Enemy::GetInstance()->TakeDamage(10);
        //printfDx("Player %d が攻撃！\n", playerID == PlayerID::Player1 ? 1 : 2);
        PlaySoundMem(attack_sound, DX_PLAYTYPE_BACK);
        Sleep(500); // 0.5秒待機
        // ターン終了
        turnManager->NextTurn();
    }

}

void Player::Draw() const {
    
    __super::Draw();
}

void Player::Heal(int amount)
{
    if (healCount > 0) { // 残り回数チェック
        Enemy::GetInstance()->Heal(40); // プレイヤーのHP回復
        PlaySoundMem(heal_sound, DX_PLAYTYPE_BACK);
        healCount--;
        Sleep(500); // 0.5秒待機
        TurnManager::GetInstance()->NextTurn();
        //printfDx("Player %d が回復！ 残り %d 回\n", playerID == PlayerID::Player1 ? 1 : 2, healCount);
    }
    else {
        PlaySoundMem(beep_sound, DX_PLAYTYPE_BACK); // 使用不可の通知音
    }
}

void Player::Pass()
{
    if (passCount > 0) { // 残り回数チェック
        passCount--;
        PlaySoundMem(pass_sound, DX_PLAYTYPE_BACK);
        Sleep(500); // 0.5秒待機

        TurnManager::GetInstance()->NextTurn();
        //printfDx("Player %d がパス！ 残り %d 回\n", playerID == PlayerID::Player1 ? 1 : 2, passCount);
    }
    else {
        PlaySoundMem(beep_sound, DX_PLAYTYPE_BACK); // 使用不可の通知音
    }
}