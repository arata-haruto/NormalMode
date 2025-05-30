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

    heal_sound = rm->GetSoundResource("Resource/Sound/�Q�[�W��1.wav");
    pass_sound = rm->GetSoundResource("Resource/Sound/�ڂ�U���1.wav");
    attack_sound = rm->GetSoundResource("Resource/Sound/���Ŏa��2.wav");
    beep_sound = rm->GetSoundResource("Resource/Sound/�r�[�v��1.wav");

   /* heal_image = rm->GetImageResource("Resource/Images/healcount.png")[0];
    pass_image = rm->GetImageResource("Resource/Images/passcount.png")[0];*/
}

void Player::Update() {
    TurnManager* turnManager = TurnManager::GetInstance();
    InputManager* input = InputManager::GetInstance();

    // �����̃^�[�����`�F�b�N
    if ((playerID == PlayerID::Player1 && turnManager->GetCurrentTurn() != Turn::Player1) ||
        (playerID == PlayerID::Player2 && turnManager->GetCurrentTurn() != Turn::Player2))
    {
        return; // �����̃^�[������Ȃ�
    }

    // �^�[���؂�ւ����o���͑���ł��Ȃ�
    if (turnManager->ShowTurnMessage()) {
        return;
    }

    if (input->GetKeyState(KEY_INPUT_X) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_Y) == eInputState::Pressed)
    {
        //Heal���Ăяo��
        Heal(40);
    }

    if (input->GetKeyState(KEY_INPUT_C) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_X) == eInputState::Pressed)
    {
        //Pass���Ăяo��
        Pass();
    }

    // �U��
    if (input->GetKeyState(KEY_INPUT_Z) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_B) == eInputState::Pressed)
    {
        Enemy::GetInstance()->TakeDamage(10);
        //printfDx("Player %d ���U���I\n", playerID == PlayerID::Player1 ? 1 : 2);
        PlaySoundMem(attack_sound, DX_PLAYTYPE_BACK);
        Sleep(500); // 0.5�b�ҋ@
        // �^�[���I��
        turnManager->NextTurn();
    }

}

void Player::Draw() const {
    
    __super::Draw();
}

void Player::Heal(int amount)
{
    if (healCount > 0) { // �c��񐔃`�F�b�N
        Enemy::GetInstance()->Heal(40); // �v���C���[��HP��
        PlaySoundMem(heal_sound, DX_PLAYTYPE_BACK);
        healCount--;
        Sleep(500); // 0.5�b�ҋ@
        TurnManager::GetInstance()->NextTurn();
        //printfDx("Player %d ���񕜁I �c�� %d ��\n", playerID == PlayerID::Player1 ? 1 : 2, healCount);
    }
    else {
        PlaySoundMem(beep_sound, DX_PLAYTYPE_BACK); // �g�p�s�̒ʒm��
    }
}

void Player::Pass()
{
    if (passCount > 0) { // �c��񐔃`�F�b�N
        passCount--;
        PlaySoundMem(pass_sound, DX_PLAYTYPE_BACK);
        Sleep(500); // 0.5�b�ҋ@

        TurnManager::GetInstance()->NextTurn();
        //printfDx("Player %d ���p�X�I �c�� %d ��\n", playerID == PlayerID::Player1 ? 1 : 2, passCount);
    }
    else {
        PlaySoundMem(beep_sound, DX_PLAYTYPE_BACK); // �g�p�s�̒ʒm��
    }
}