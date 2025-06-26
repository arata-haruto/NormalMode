#pragma once
#include "../GameObject.h"
#include "../Enemy/Enemy.h"
#include <vector>

struct AttackParticle {
    Vector2D pos;       // �ʒu
    Vector2D vel;       // ���x
    int life;           // ���� (�t���[����)
    int maxLife;        // �ő���� (�A���t�@�l�v�Z�p)
    unsigned int color; // �F
    int size;           // �T�C�Y (��ӂ̒���)
};

// �񕜃p�[�e�B�N���̍\����
struct HealParticle {
    Vector2D pos;       // �ʒu
    Vector2D vel;       // ���x
    int life;           // ���� (�t���[����)
    int maxLife;        // �ő����
    unsigned int color; // �F
    int size;           // �T�C�Y
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

    int effect_image = -1; // �G�t�F�N�g�摜
    bool isHealEffectPlaying = false;
    int healEffectTimer = 0;
    int healEffectFrame = 0;
    Vector2D healEffectPosition = { 0, 0 }; // �`��ʒu

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