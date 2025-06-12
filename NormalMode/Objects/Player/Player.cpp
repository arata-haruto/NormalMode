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
    pass_sound = rm->GetSoundResource("Resource/Sound/�ڂ�U���1.mp3");
    attack_sound = rm->GetSoundResource("Resource/Sound/���Ŏa��2.wav");
    beep_sound = rm->GetSoundResource("Resource/Sound/�r�[�v��1.wav");

    effect_image = rm->GetImageResource("Resource/Images/pipo-mapeffect008.png")[0];
    attack_effect_image = rm->GetImageResource("Resource/Images/tor_�a��_����.png")[0];

   /* heal_image = rm->GetImageResource("Resource/Images/healcount.png")[0];
    pass_image = rm->GetImageResource("Resource/Images/passcount.png")[0];*/
}

void Player::Update() {
    TurnManager* turnManager = TurnManager::GetInstance();
    InputManager* input = InputManager::GetInstance();

    if (isHealEffectPlaying)
    {
        healEffectTimer++;
        if (healEffectTimer % 5 == 0)
        {
            healEffectFrame++;
            if (healEffectFrame >= 10) // �S�t���[���Đ�����
            {
                isHealEffectPlaying = false;

                TurnManager::GetInstance()->NextTurn();

            }
        }
        return;
    }

    if (isAttackEffectPlaying)
    {
        attackEffectTimer++;
        if (attackEffectTimer % 5 == 0)
        {
            attackEffectFrame++;
            if (attackEffectFrame >= 10) // �t���[�����ɍ��킹�Ē���
            {
                isAttackEffectPlaying = false;

                TurnManager::GetInstance()->NextTurn(); // �G�t�F�N�g�I����Ƀ^�[���i�s
            }
        }
        return; // �G�t�F�N�g�Đ����͑���֎~
    }

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
        Enemy* enemy = Enemy::GetInstance();

        int damage = enemy->GetSelectedPartDamage();  // �����p�ӂ���

        enemy->TakeDamage(damage);
        //printfDx("Player %d ���U���I\n", playerID == PlayerID::Player1 ? 1 : 2);
        // �U���G�t�F�N�g���J�n
        isAttackEffectPlaying = true;
        attackEffectFrame = 0;
        attackEffectTimer = 0;

        // �I�𒆂̃p�[�c�̈ʒu���擾���ăZ�b�g
        attackEffectPosition = Enemy::GetInstance()->GetSelectedPartPosition();
        PlaySoundMem(attack_sound, DX_PLAYTYPE_BACK);
        
      
    }

}

void Player::Draw() const {
    
    if (isHealEffectPlaying)
    {
        const int frameWidth = 240;  // 1�t���[���̉���
        const int frameHeight = 480; // 1�t���[���̍���
        const int columns = 10;      // ����10�t���[��������ł���

        int fx = (healEffectFrame % columns) * frameWidth;
        int fy = 0;
        /*int fx = (healEffectFrame % columns) * frameWidth;
        int fy = 0;*/

        DrawRectGraph(
            static_cast<int>(healEffectPosition.x),
            static_cast<int>(healEffectPosition.y),
            fx, fy,
            frameWidth, frameHeight,
            effect_image,
            TRUE
        );
    }

    if (isAttackEffectPlaying)
    {
        const int frameWidth = 192;
        const int frameHeight = 192; 

        int columns = 5; 

        int fx = (attackEffectFrame % columns) * frameWidth;
        int fy = 0;

        DrawRectGraph(
            static_cast<int>(attackEffectPosition.x),
            static_cast<int>(attackEffectPosition.y),
            fx, fy,
            frameWidth, frameHeight,
            attack_effect_image,
            TRUE
        );
    }

    __super::Draw();
}

void Player::Heal(int amount)
{
    if (healCount > 0) { // �c��񐔃`�F�b�N
        Enemy::GetInstance()->Heal(40); // �v���C���[��HP��
        PlaySoundMem(heal_sound, DX_PLAYTYPE_BACK);

        // �G�t�F�N�g���o
        isHealEffectPlaying = true;
        healEffectTimer = 0;
        healEffectFrame = 0;
        healEffectPosition = {205, 40};

        healCount--;
        
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