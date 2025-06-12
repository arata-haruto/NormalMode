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

    int turnMessageAlpha;     // メッセージの透明度 (0-255)
    int turnMessageY;         // メッセージのY座標 (アニメーション用)
    int turnMessageState;     // 0:非表示, 1:フェードイン, 2:固定表示, 3:フェードアウト
    int turnMessageAnimationTimer; // アニメーションのタイマー

    TurnManager();

public:
    static TurnManager* GetInstance();
    Turn GetCurrentTurn() const;
    void NextTurn();

    void Update(float deltaTime);
    bool ShowTurnMessage() const;
    const std::string& GetTurnMessage() const;
    float GetDisplayTimer() const;

    void UpdateTurnMessageAnimation(float deltaTime); // deltaTimeを使うように変更
    void DrawTurnMessageOverlay() const; // オーバーレイとメッセージの描画

    void SetInitialTurn(Turn initialPlayer);
};