#pragma once
#include "../GameObject.h"
#include "../Enemy/Enemy.h"

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
    int passCount = 1;
public:
    Player(PlayerID id);
    virtual ~Player();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Draw() const override;


    void Attack(Enemy* target);
};