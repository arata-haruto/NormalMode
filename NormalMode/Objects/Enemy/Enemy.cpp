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
    DrawString(100, 100, "画像の読み込みに失敗しました", GetColor(255, 0, 0));
}

   bodyParts.emplace_back("Head", Vector2D(295, 60), 52, 52);
   bodyParts.emplace_back("Body", Vector2D(295, 180), 52, 58);
   bodyParts.emplace_back("Legs", Vector2D(295, 420), 52, 52);

   //attack_effect_image = rm->GetImageResource("Resource/Effects/slash.png")[0];

}

void Enemy::Update() {
    InputManager* input = InputManager::GetInstance();
    TurnManager* turnManager = TurnManager::GetInstance();


    // ターン切り替え演出中は操作できない
    if (turnManager->ShowTurnMessage()) {
        return;
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

    // 攻撃処理
   /* if (input->GetKeyState(KEY_INPUT_Z) == eInputState::Pressed) {
        TakeDamage(10);
        printfDx("%sに攻撃！ Enemy HP: %d\n", bodyParts[selectedPartIndex].GetName().c_str(), hit_point);
    }*/
}

void Enemy::Draw() const {

    if (!IsDestroyed()) {
        DrawGraph(80, 0, image, TRUE);

        for (int i = 0; i < bodyParts.size(); ++i) {
            const auto& part = bodyParts[i];
            unsigned int color = (i == selectedPartIndex) ? GetColor(255, 0, 0) : GetColor(255, 255, 255);
            DrawBox(part.GetPosition().x, part.GetPosition().y,
                part.GetPosition().x + part.GetWidth(), part.GetPosition().y + part.GetHeight(),
                color, FALSE);
        }

        DrawFormatString(400, 50, GetColor(255, 255, 255), "HP: %d", hit_point);

    }

}

void Enemy::Finalize() {
  
}

void Enemy::TakeDamage(int damage) {
    hit_point -= damage;
    if (hit_point < 0) hit_point = 0;
}

bool Enemy::IsDestroyed() const {
    return hit_point <= 0;
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

    return bodyParts[selectedPartIndex].GetPosition();
}

void Enemy::Heal(int amount) {
    hit_point += amount;
    if (hit_point > 100) { hit_point = 100; }
}

Enemy* Enemy::GetInstance() {
    if (!instance) {
        instance = new Enemy();
    }
    return instance;
}
