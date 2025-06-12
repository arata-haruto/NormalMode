#pragma once
#include <string>

enum class Turn {
    Player1,
    Player2
};

class TurnManager {
private:
    static TurnManager* instance;
    Turn currentTurn;
    bool turnChanged;
    float displayTimer;
    std::string turnMessage;

    int turnMessageAlpha;     // ���b�Z�[�W�̓����x (0-255)
    int turnMessageY;         // ���b�Z�[�W��Y���W (�A�j���[�V�����p)
    int turnMessageState;     // 0:��\��, 1:�t�F�[�h�C��, 2:�Œ�\��, 3:�t�F�[�h�A�E�g
    int turnMessageAnimationTimer; // �A�j���[�V�����̃^�C�}�[

    TurnManager();

public:
    static TurnManager* GetInstance();
    Turn GetCurrentTurn() const;
    void NextTurn();

    void Update(float deltaTime);
    bool ShowTurnMessage() const;
    const std::string& GetTurnMessage() const;
    float GetDisplayTimer() const;

    void UpdateTurnMessageAnimation(float deltaTime); // deltaTime���g���悤�ɕύX
    void DrawTurnMessageOverlay() const; // �I�[�o�[���C�ƃ��b�Z�[�W�̕`��

    void SetInitialTurn(Turn initialPlayer);
};