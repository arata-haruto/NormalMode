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
    TurnManager();

public:
    static TurnManager* GetInstance();
    Turn GetCurrentTurn() const;
    void NextTurn();

    void Update(float deltaTime);
    bool ShowTurnMessage() const;
    const std::string& GetTurnMessage() const;
    float GetDisplayTimer() const;
};