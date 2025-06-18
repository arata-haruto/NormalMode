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
    const int ENEMY_INITIAL_HP = 100; // 敵の初期HP
    const float SHAKE_DURATION_SECONDS = 0.55f; // 揺れる時間 (秒)
    const float SHAKE_MAGNITUDE = 5.0f;       // 揺れ幅 (ピクセル)
    const float SHAKE_FREQUENCY = 10.0f;      // 揺れの周波数

    // 敵の描画位置
    const Vector2D ENEMY_BASE_DRAW_POS = { 400, 0 };

    
    const Vector2D HEAD_RELATIVE_POS = { 215, 60 }; 
    const Vector2D BODY_RELATIVE_POS = { 215, 180 }; 
    const Vector2D LEGS_RELATIVE_POS = { 215, 420 }; 
    const Vector2D DEBUG_RELATIVE_POS = { 215, 490 }; 

    // 部位のサイズ
    const int HEAD_WIDTH = 52;
    const int HEAD_HEIGHT = 52;
    const int BODY_WIDTH = 52;
    const int BODY_HEIGHT = 58;
    const int LEGS_WIDTH = 52;
    const int LEGS_HEIGHT = 52;
    const int DEBUG_WIDTH = 52;
    const int DEBUG_HEIGHT = 52;

    // 部位のダメージ範囲
    const int HEAD_DMG_MIN = 10;
    const int HEAD_DMG_MAX = 20;
    const int BODY_DMG_MIN = 7;
    const int BODY_DMG_MAX = 12;
    const int LEGS_DMG_MIN = 1;
    const int LEGS_DMG_MAX = 6;
    const int DEBUG_DMG_MIN = 100;
    const int DEBUG_DMG_MAX = 100; // デバッグ用

    // 表示関連
    const int HP_DISPLAY_X = 700;
    const int HP_DISPLAY_Y = 80;
    const int SELECTED_PART_NAME_X = 100;
    const int SELECTED_PART_NAME_Y = 80;
   

    // エフェクト描画位置調整用
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

    part_sound = rm->GetSoundResource("Resource/Sound/カーソル移動8.mp3");

    image = rm->GetImageResource("Resource/Images/Doll.png")[0];
    if (image == -1) {
        DrawString(500, 100, "画像の読み込みに失敗しました", GetColor(255, 0, 0));
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

    // ターン切り替え演出中は操作できない
    if (turnManager->ShowTurnMessage()) {
        return;
    }

    if (is_shaking) {
        shake_timer -= deltaTime;
        if (shake_timer <= 0.0f) {
            is_shaking = false;
        }
    }

    // 部位選択（←→キー）
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

    // 揺れエフェクトの適用
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

            // 選択中のパーツかどうかを判断
            if (i == selectedPartIndex) {
                // 選択中のパーツは色を赤にする
                color = GetColor(255, 0, 0);

                int arrowX = static_cast<int>(part.GetPosition().x + part.GetWidth() / 2) - 85;
                int arrowY = static_cast<int>(part.GetPosition().y) - 20;


            }
            else {
                // 非選択のパーツは元の色
                color = GetColor(255, 255, 255);
            }

            // パーツの矩形を描画
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
                DrawFormatString(SELECTED_PART_NAME_X, SELECTED_PART_NAME_Y, GetColor(0, 255, 255), "選択部位: %s", selectedPart.GetName().c_str());
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
    // 揺れエフェクトを開始
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

    // パーツの中心を取得
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
