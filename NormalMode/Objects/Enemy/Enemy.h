#pragma once
#include "../GameObject.h"
#include "BodyPart.h"
#include <vector>

class Enemy : public GameObject {
private:
    std::vector<BodyPart> bodyParts;
    int selectedPartIndex; // ‘I‘ð’†‚Ì•”ˆÊ

private:
    static Enemy* instance;
    int hit_point;
    int attack_power;
    bool is_destroyed;
    int image;

public:
    Enemy();
    virtual ~Enemy();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Draw() const override;
    virtual void Finalize() override;

    void TakeDamage(int attack_power);
    void Heal(int amount);

    bool IsDestroyed() const;
    int GetHitPoint() const;
    void SelectNextPart();
    void SelectPreviousPart();

    static Enemy* GetInstance();
};

