#include "Player.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/InputManager.h"
#include "../Enemy/Enemy.h"
#include "../TurnManager.h"
#include "DxLib.h"
#include <random>

namespace {
    // �p�[�e�B�N���֘A
    const int ATTACK_PARTICLE_COUNT = 10; // �U�����p�[�e�B�N����
    const int ATTACK_PARTICLE_LIFE_MIN = 15; // �ŏ�����
    const int ATTACK_PARTICLE_LIFE_MAX = 30; // �ő����
    const int ATTACK_PARTICLE_SIZE_MIN = 2; // �ŏ��T�C�Y
    const int ATTACK_PARTICLE_SIZE_MAX = 5; // �ő�T�C�Y
    const int ATTACK_PARTICLE_VEL_MIN = 1; // �ŏ����x
    const int ATTACK_PARTICLE_VEL_MAX = 4; // �ő呬�x
    const unsigned int ATTACK_PARTICLE_COLOR = GetColor(255, 255, 255); // ��

    const int HEAL_PARTICLE_COUNT = 8; // �񕜎��p�[�e�B�N����
    const int HEAL_PARTICLE_LIFE_MIN = 10;
    const int HEAL_PARTICLE_LIFE_MAX = 25;
    const int HEAL_PARTICLE_SIZE_MIN = 3;
    const int HEAL_PARTICLE_SIZE_MAX = 6;
    const int HEAL_PARTICLE_VEL_MIN = 1;
    const int HEAL_PARTICLE_VEL_MAX = 3;
    const unsigned int HEAL_PARTICLE_COLOR = GetColor(50, 255, 50); // ��
}


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
    attack_effect_image = rm->GetImageResource("Resource/Images/tor_�a��_����1.png")[0];

   /* heal_image = rm->GetImageResource("Resource/Images/healcount.png")[0];
    pass_image = rm->GetImageResource("Resource/Images/passcount.png")[0];*/
}

void Player::Update() {
    TurnManager* turnManager = TurnManager::GetInstance();
    InputManager* input = InputManager::GetInstance();

    if (turnManager->ShowTurnMessage()) {
        return;
    }

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
        for (auto it = attackParticles.begin(); it != attackParticles.end(); ) {
            it->pos.x += it->vel.x;
            it->pos.y += it->vel.y;
            it->life--;
            if (it->life <= 0) {
                it = attackParticles.erase(it);
            }
            else {
                ++it;
            }
        }
        for (auto it = healParticles.begin(); it != healParticles.end(); ) {
            it->pos.x += it->vel.x;
            it->pos.y += it->vel.y;
            it->life--;
            if (it->life <= 0) {
                it = healParticles.erase(it);
            }
            else {
                ++it;
            }
        }
        return; // �����̃^�[������Ȃ�
    }

    // �^�[���؂�ւ����o���͑���ł��Ȃ�
    if (turnManager->ShowTurnMessage()) {
        for (auto it = attackParticles.begin(); it != attackParticles.end(); ) {
            it->pos.x += it->vel.x;
            it->pos.y += it->vel.y;
            it->life--;
            if (it->life <= 0) {
                it = attackParticles.erase(it);
            }
            else {
                ++it;
            }
        }
        for (auto it = healParticles.begin(); it != healParticles.end(); ) {
            it->pos.x += it->vel.x;
            it->pos.y += it->vel.y;
            it->life--;
            if (it->life <= 0) {
                it = healParticles.erase(it);
            }
            else {
                ++it;
            }
        }
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
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> life_dist(ATTACK_PARTICLE_LIFE_MIN, ATTACK_PARTICLE_LIFE_MAX);
        std::uniform_int_distribution<> size_dist(ATTACK_PARTICLE_SIZE_MIN, ATTACK_PARTICLE_SIZE_MAX);
        std::uniform_real_distribution<> vel_dist_x(-ATTACK_PARTICLE_VEL_MAX, ATTACK_PARTICLE_VEL_MAX);
        std::uniform_real_distribution<> vel_dist_y(-ATTACK_PARTICLE_VEL_MAX, ATTACK_PARTICLE_VEL_MAX);

        for (int i = 0; i < ATTACK_PARTICLE_COUNT; ++i) {
            AttackParticle p;
            // �G�t�F�N�g�̒��S�t�߂��烉���_���ɐ��� (���̃t���[���T�C�Y���g�p)
            p.pos.x = attackEffectPosition.x + (192 / 2) + (vel_dist_x(gen) * 5); // ATTACK_EFFECT_FRAME_WIDTH�𒼐ڎw��
            p.pos.y = attackEffectPosition.y + (192 / 2) + (vel_dist_y(gen) * 5); // ATTACK_EFFECT_FRAME_HEIGHT�𒼐ڎw��
            p.vel.x = vel_dist_x(gen);
            p.vel.y = vel_dist_y(gen);
            p.life = life_dist(gen);
            p.maxLife = p.life;
            p.color = ATTACK_PARTICLE_COLOR;
            p.size = size_dist(gen);
            attackParticles.push_back(p);
        }
      
    }
    for (auto it = attackParticles.begin(); it != attackParticles.end(); ) {
        it->pos.x += it->vel.x;
        it->pos.y += it->vel.y;
        it->life--;
        if (it->life <= 0) {
            it = attackParticles.erase(it);
        }
        else {
            ++it;
        }
    }
    for (auto it = healParticles.begin(); it != healParticles.end(); ) {
        it->pos.x += it->vel.x;
        it->pos.y += it->vel.y;
        it->life--;
        if (it->life <= 0) {
            it = healParticles.erase(it);
        }
        else {
            ++it;
        }
    }

}

void Player::Draw() const {
    
    if (isHealEffectPlaying)
    {
        const int frameWidth = 240; 
        const int frameHeight = 480; 
        const int columns = 5;     

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
        const int frameWidth = 280;
        const int frameHeight = 280; 

        int columns = 10; 

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

    for (const auto& p : attackParticles) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * (static_cast<float>(p.life) / p.maxLife))); // �����ɉ����ăt�F�[�h�A�E�g
        DrawBox(static_cast<int>(p.pos.x - p.size / 2),
            static_cast<int>(p.pos.y - p.size / 2),
            static_cast<int>(p.pos.x + p.size / 2),
            static_cast<int>(p.pos.y + p.size / 2),
            p.color, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // �u�����h���[�h�����Z�b�g
    }
    for (const auto& p : healParticles) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * (static_cast<float>(p.life) / p.maxLife)));
        DrawBox(static_cast<int>(p.pos.x - p.size / 2),
            static_cast<int>(p.pos.y - p.size / 2),
            static_cast<int>(p.pos.x + p.size / 2),
            static_cast<int>(p.pos.y + p.size / 2),
            p.color, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
        healEffectPosition = {515, 40};

        healCount--;
        
        
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