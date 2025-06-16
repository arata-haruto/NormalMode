#pragma once
#include "../GameObject.h"
#include "BodyPart.h"
#include <vector>

class Enemy : public GameObject {
private:
    std::vector<BodyPart> bodyParts;
    int selectedPartIndex; // 選択中の部位

private:
    static Enemy* instance;
    int hit_point;
    int attack_power;
    bool is_destroyed;
    int image;

    

    int attack_effect_image;
    int effectActive;
    int alphaValue;
    int effectX;
    int effectY;

     bool is_shaking = false;
    float shake_timer = 0.0f;
    int shake_duration = 15; // 揺れる時間（秒）
    const float shake_magnitude = 5.0f; // 揺れ幅（ピクセル）
    Vector2D base_pos = { 0, 0 };

    bool isHpHidden;

public:
    Enemy();
    virtual ~Enemy();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Draw() const override;
    virtual void Finalize() override;

    void TakeDamage(int attack_power);
    void Heal(int amount);

    void AttackEffect();

    bool IsDestroyed() const;
    int GetHitPoint() const;
    void SelectNextPart();
    void SelectPreviousPart();
    Vector2D GetSelectedPartPosition() const;

    int GetSelectedPartDamage() const;

    static Enemy* GetInstance();

};