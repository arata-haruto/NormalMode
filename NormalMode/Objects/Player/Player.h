#pragma once
#include "../GameObject.h"
#include "../Enemy/Enemy.h"
#include <vector>


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

    int effect_image;
    int effectActive;
    int alphaValue;
    int effectX;
    int effectY;

public:
    Player(PlayerID id);
    virtual ~Player();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Draw() const override;

    void Heal(int amount);
    void Pass();

    //void AttackEffect();
    void HealEffect();

    int GetPassCount() const 
    { return passCount; }
    int GetHealCount() const 
    { return healCount; }

    void Attack(Enemy* target);
};