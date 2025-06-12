#include "Enemy.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/InputManager.h"
#include "BodyPart.h"
#include "../Player/Player.h"
#include "../TurnManager.h"
#include "DxLib.h"

Enemy* Enemy::instance = nullptr;

Enemy::Enemy() : hit_point(100), selectedPartIndex(0)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize() {
    ResourceManager* rm = ResourceManager::GetInstance();


    image = rm->GetImageResource("Resource/Images/Doll.png")[0];
    if (image == -1) {
        DrawString(500, 100, "画像の読み込みに失敗しました", GetColor(255, 0, 0));
    }

    bodyParts.emplace_back("Head", Vector2D(615, 60), 52, 52, 10, 20);  // 高ダメージ
    bodyParts.emplace_back("Body", Vector2D(615, 180), 52, 58, 7, 12);   // 中ダメージ
    bodyParts.emplace_back("Legs", Vector2D(615, 420), 52, 52, 1, 6);   // 低ダメージ


    bodyParts.emplace_back("Debug", Vector2D(615, 490), 52, 52, 100, 100);   //デバッグ用


    base_pos = { 400, 0 };

    //attack_effect_image = rm->GetImageResource("Resource/Effects/slash.png")[0];

}

void Enemy::Update() {
    InputManager* input = InputManager::GetInstance();
    TurnManager* turnManager = TurnManager::GetInstance();


    // ターン切り替え演出中は操作できない
    if (turnManager->ShowTurnMessage()) {
        return;
    }

    if (is_shaking) {
        shake_timer--;
        if (shake_timer <= 0) {
            is_shaking = false;
        }
    }

    // 部位選択（←→キー）
    if (input->GetKeyState(KEY_INPUT_UP) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_DPAD_UP) == eInputState::Pressed)
    {
        SelectPreviousPart();
    }
    else if (input->GetKeyState(KEY_INPUT_DOWN) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == eInputState::Pressed)
    {
        SelectNextPart();
    }

}

void Enemy::Draw() const {

    Vector2D draw_pos = base_pos;

    if (is_shaking) {
        int offset = (rand() % 3 - 1) * static_cast<int>(shake_magnitude);  // -1, 0, 1のどれか
        draw_pos.x += offset;
    }

    DrawGraph(static_cast<int>(draw_pos.x), static_cast<int>(draw_pos.y), image, TRUE);

    if (!IsDestroyed()) {
        DrawGraph(400, 0, image, TRUE);

        for (int i = 0; i < bodyParts.size(); ++i) {
            const auto& part = bodyParts[i];
            unsigned int color;

            // 選択中のパーツかどうかを判断
            if (i == selectedPartIndex) {
                // 選択中のパーツは色を赤にする
                color = GetColor(255, 0, 0);

                int arrowX = static_cast<int>(part.GetPosition().x + part.GetWidth() / 2) - 8;
                int arrowY = static_cast<int>(part.GetPosition().y) - 20;
                DrawString(arrowX, arrowY, "→", GetColor(255, 0, 0));
               
            }
            else {
                // 非選択のパーツは元の色
                color = GetColor(255, 255, 255);
            }

            // パーツの矩形を描画
            DrawBox(part.GetPosition().x, part.GetPosition().y,
                part.GetPosition().x + part.GetWidth(), part.GetPosition().y + part.GetHeight(),
                color, FALSE);
        }
       

        DrawFormatString(1100, 50, GetColor(255, 255, 255), "HP: %d", hit_point);

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

    Vector2D effectOffset = Vector2D(192 / 2, 192 / 2);

    return center - effectOffset;
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
