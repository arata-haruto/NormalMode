#pragma once
#include "../GameObject.h"
#include "../Enemy/Enemy.h"
#include <vector>

struct AttackParticle {
    Vector2D pos;       // 位置
    Vector2D vel;       // 速度
    int life;           // 寿命 (フレーム数)
    int maxLife;        // 最大寿命 (アルファ値計算用)
    unsigned int color; // 色
    int size;           // サイズ (一辺の長さ)
};

// 回復パーティクルの構造体
struct HealParticle {
    Vector2D pos;       // 位置
    Vector2D vel;       // 速度
    int life;           // 寿命 (フレーム数)
    int maxLife;        // 最大寿命
    unsigned int color; // 色
    int size;           // サイズ
};

class Player : public GameObject {
public:
    enum class PlayerID {
        Player1,
        Player2
    };

private:
    PlayerID playerID;
    int attack_power;
    int selectedPartIndex;
    int healCount = 1;
    int passCount = 2;

    int heal_image;
    int pass_image;

    int heal_sound;
    int pass_sound;
    int attack_sound;
    int beep_sound;

    int effect_image = -1; // エフェクト画像
    bool isHealEffectPlaying = false;
    int healEffectTimer = 0;
    int healEffectFrame = 0;
    Vector2D healEffectPosition = { 0, 0 }; // 描画位置

    int attack_effect_image = -1;
    bool isAttackEffectPlaying = false;
    int attackEffectFrame = 0;
    int attackEffectTimer = 0;
    Vector2D attackEffectPosition;

    const float attack_effect_duration = 10.7f;

    std::vector<AttackParticle> attackParticles;
    std::vector<HealParticle> healParticles;

public:
    Player(PlayerID id);
    virtual ~Player();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Draw() const override;

    void Attack();
    void Heal(int amount);
    void Pass();

    int GetPassCount() const 
    { return passCount; }
    int GetHealCount() const 
    { return healCount; }

    bool IsDestroyed() const;

    void Control();
};