#pragma once
#include "../GameObject.h"
#include "../Enemy/Enemy.h"

class Player : public GameObject {
private:
    static Player* instance;

    int attack_power;

    int selectedPartIndex;
public:
    Player();
    virtual ~Player();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Draw() const override;

    void Attack(Enemy* target);
    static Player* GetInstance();

};