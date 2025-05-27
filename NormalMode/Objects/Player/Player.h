#pragma once
#include "../GameObject.h"
#include "../Enemy/Enemy.h"

class Player : public GameObject {
private:
    //static Player* instance;

    enum class PlayerID {
        Player1,
        Player2
    };

    PlayerID playerID;
    int attack_power;

    int selectedPartIndex;
public:
    Player(PlayerID id);
    virtual ~Player();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Draw() const override;

    void Attack(Enemy* target);
   //static Player* GetInstance();

};