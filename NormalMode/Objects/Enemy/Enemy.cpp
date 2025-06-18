#include "Enemy.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/InputManager.h"
#include "BodyPart.h"
#include "../Player/Player.h"
#include "../TurnManager.h"
#include "DxLib.h"

#include <cmath> 
#include <random> 
#include <chrono>

Enemy* Enemy::instance = nullptr;

namespace {
    const int ENEMY_INITIAL_HP = 100; // �G�̏���HP
    const float SHAKE_DURATION_SECONDS = 0.55f; // �h��鎞�� (�b)
    const float SHAKE_MAGNITUDE = 5.0f;       // �h�ꕝ (�s�N�Z��)
    const float SHAKE_FREQUENCY = 10.0f;      // �h��̎��g��

    // �G�̕`��ʒu
    const Vector2D ENEMY_BASE_DRAW_POS = { 400, 0 };

    
    const Vector2D HEAD_RELATIVE_POS = { 215, 60 }; 
    const Vector2D BODY_RELATIVE_POS = { 215, 180 }; 
    const Vector2D LEGS_RELATIVE_POS = { 215, 420 }; 
    const Vector2D DEBUG_RELATIVE_POS = { 215, 490 }; 

    // ���ʂ̃T�C�Y
    const int HEAD_WIDTH = 52;
    const int HEAD_HEIGHT = 52;
    const int BODY_WIDTH = 52;
    const int BODY_HEIGHT = 58;
    const int LEGS_WIDTH = 52;
    const int LEGS_HEIGHT = 52;
    const int DEBUG_WIDTH = 52;
    const int DEBUG_HEIGHT = 52;

    // ���ʂ̃_���[�W�͈�
    const int HEAD_DMG_MIN = 10;
    const int HEAD_DMG_MAX = 20;
    const int BODY_DMG_MIN = 7;
    const int BODY_DMG_MAX = 12;
    const int LEGS_DMG_MIN = 1;
    const int LEGS_DMG_MAX = 6;
    const int DEBUG_DMG_MIN = 100;
    const int DEBUG_DMG_MAX = 100; // �f�o�b�O�p

    // �\���֘A
    const int HP_DISPLAY_X = 700;
    const int HP_DISPLAY_Y = 80;
    const int SELECTED_PART_NAME_X = 100;
    const int SELECTED_PART_NAME_Y = 80;
   

    // �G�t�F�N�g�`��ʒu�����p
    const int ATTACK_EFFECT_FRAME_WIDTH = 192;
    const int ATTACK_EFFECT_FRAME_HEIGHT = 192;
    const int ATTACK_EFFECT_WIDTH_HALF = 192 / 2;
    const int ATTACK_EFFECT_HEIGHT_HALF = 192 / 2;

    const Vector2D ATTACK_EFFECT_FINE_TUNE_OFFSET = { 420, 0 };
}

Enemy::Enemy() :
    hit_point(ENEMY_INITIAL_HP),
    selectedPartIndex(0),
    is_destroyed(false),
    image(-1),
    is_shaking(false),
    shake_timer(0.0f),
    shake_duration(SHAKE_DURATION_SECONDS),
    shake_magnitude(SHAKE_MAGNITUDE),
    base_pos(ENEMY_BASE_DRAW_POS),
    isHpHidden(false)
{
  
}

Enemy::~Enemy()
{
}

void Enemy::Initialize() {
    ResourceManager* rm = ResourceManager::GetInstance();

    part_sound = rm->GetSoundResource("Resource/Sound/�J�[�\���ړ�8.mp3");

    image = rm->GetImageResource("Resource/Images/Doll.png")[0];
    if (image == -1) {
        DrawString(500, 100, "�摜�̓ǂݍ��݂Ɏ��s���܂���", GetColor(255, 0, 0));
    }

    hit_point = ENEMY_INITIAL_HP;
    isHpHidden = false;

    bodyParts.clear();
    bodyParts.emplace_back("Head", HEAD_RELATIVE_POS, HEAD_WIDTH, HEAD_HEIGHT, HEAD_DMG_MIN, HEAD_DMG_MAX);
    bodyParts.emplace_back("Body", BODY_RELATIVE_POS, BODY_WIDTH, BODY_HEIGHT, BODY_DMG_MIN, BODY_DMG_MAX);
    bodyParts.emplace_back("Legs", LEGS_RELATIVE_POS, LEGS_WIDTH, LEGS_HEIGHT, LEGS_DMG_MIN, LEGS_DMG_MAX);
    bodyParts.emplace_back("Debug", DEBUG_RELATIVE_POS, DEBUG_WIDTH, DEBUG_HEIGHT, DEBUG_DMG_MIN, DEBUG_DMG_MAX);


    //base_pos = { 400, 0 };


}

void Enemy::Update() {
    InputManager* input = InputManager::GetInstance();
    TurnManager* turnManager = TurnManager::GetInstance();
    float deltaTime = 1.0f / 60.0f;

    // �^�[���؂�ւ����o���͑���ł��Ȃ�
    if (turnManager->ShowTurnMessage()) {
        return;
    }

    if (is_shaking) {
        shake_timer -= deltaTime;
        if (shake_timer <= 0.0f) {
            is_shaking = false;
        }
    }

    // ���ʑI���i�����L�[�j
    if (input->GetKeyState(KEY_INPUT_UP) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_DPAD_UP) == eInputState::Pressed)
    {
        PlaySoundMem(part_sound, DX_PLAYTYPE_BACK);
        SelectPreviousPart();
    }
    else if (input->GetKeyState(KEY_INPUT_DOWN) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::Pressed)
    {
        PlaySoundMem(part_sound, DX_PLAYTYPE_BACK);
        SelectNextPart();
    }

}

void Enemy::Draw() const {

    Vector2D draw_pos = base_pos;

    // �h��G�t�F�N�g�̓K�p
    if (is_shaking) {
        
        float current_shake_progress = 1.0f - (shake_timer / SHAKE_DURATION_SECONDS); 
       
        float offset_x = shake_magnitude * std::sin(current_shake_progress * SHAKE_FREQUENCY);
        

        draw_pos.x += offset_x;
        // draw_pos.y += offset_y;
    }

    DrawGraph(static_cast<int>(draw_pos.x), static_cast<int>(draw_pos.y), image, TRUE);

    if (!IsDestroyed()) {
        //DrawGraph(400, 0, image, TRUE);

        for (int i = 0; i < bodyParts.size(); ++i) {
            const auto& part = bodyParts[i];
            unsigned int color;

            // �I�𒆂̃p�[�c���ǂ����𔻒f
            if (i == selectedPartIndex) {
                // �I�𒆂̃p�[�c�͐F��Ԃɂ���
                color = GetColor(255, 0, 0);

                int arrowX = static_cast<int>(part.GetPosition().x + part.GetWidth() / 2) - 85;
                int arrowY = static_cast<int>(part.GetPosition().y) - 20;


            }
            else {
                // ��I���̃p�[�c�͌��̐F
                color = GetColor(255, 255, 255);
            }

            // �p�[�c�̋�`��`��
            DrawBox(static_cast<int>(draw_pos.x + part.GetPosition().x),
                static_cast<int>(draw_pos.y + part.GetPosition().y),
                static_cast<int>(draw_pos.x + part.GetPosition().x + part.GetWidth()),
                static_cast<int>(draw_pos.y + part.GetPosition().y + part.GetHeight()),
                color, FALSE);
        }

        if (!isHpHidden) {
        DrawFormatString(HP_DISPLAY_X, HP_DISPLAY_Y, GetColor(255, 255, 255), "HP: %d", hit_point);
    }

        if (selectedPartIndex >= 0 && selectedPartIndex < bodyParts.size()) {
            const BodyPart& selectedPart = bodyParts[selectedPartIndex];

            if (selectedPartIndex >= 0 && selectedPartIndex < bodyParts.size()) {
                const BodyPart& selectedPart = bodyParts[selectedPartIndex];
                DrawFormatString(SELECTED_PART_NAME_X, SELECTED_PART_NAME_Y, GetColor(0, 255, 255), "�I�𕔈�: %s", selectedPart.GetName().c_str());
            }
        }

    }

}

void Enemy::Finalize() {

}

void Enemy::TakeDamage(int damage) {
    hit_point -= damage;
    is_shaking = true;
    shake_timer = shake_duration;
    if (hit_point <= 0 && !IsDestroyed()) {
        hit_point = 0;  

    }
    if (hit_point <= ENEMY_INITIAL_HP / 2) {
        isHpHidden = true;
    }
    // �h��G�t�F�N�g���J�n
    is_shaking = true;
    shake_timer = shake_duration;
}

bool Enemy::IsDestroyed() const {
    if(hit_point <= 0) {
       
        
        return true;
    }
    return false;
}

int Enemy::GetHitPoint() const {
    return hit_point;
}

void Enemy::SelectNextPart() {
    selectedPartIndex = (selectedPartIndex + 1) % bodyParts.size();
}

void Enemy::SelectPreviousPart() {
    selectedPartIndex = (selectedPartIndex - 1 + bodyParts.size()) % bodyParts.size();
}

Vector2D Enemy::GetSelectedPartPosition() const
{
    if (selectedPartIndex < 0 || selectedPartIndex >= bodyParts.size())
        return Vector2D(0, 0);

    const BodyPart& part = bodyParts[selectedPartIndex];

    // �p�[�c�̒��S���擾
    Vector2D center = part.GetPosition() + Vector2D(part.GetWidth() / 2, part.GetHeight() / 2);

    return center - Vector2D(ATTACK_EFFECT_WIDTH_HALF, ATTACK_EFFECT_HEIGHT_HALF) + ATTACK_EFFECT_FINE_TUNE_OFFSET;

   // Vector2D effectOffset = Vector2D(192 / 2, 192 / 2);

    //return center - effectOffset;
}

void Enemy::Heal(int amount) {
    hit_point += amount;
    if (hit_point > 100) { hit_point = 100; }
}

int Enemy::GetSelectedPartDamage() const {
    if (selectedPartIndex < 0 || selectedPartIndex >= bodyParts.size())
        return 0;

    return bodyParts[selectedPartIndex].GetRandomDamage();
}

Enemy* Enemy::GetInstance() {
    if (!instance) {
        instance = new Enemy();
    }
    return instance;
}
