#pragma once

enum class Turn {
    Player1,
    Player2
};

class TurnManager {
private:
    static TurnManager* instance;
    Turn currentTurn;

    TurnManager();

public:
    static TurnManager* GetInstance();
    Turn GetCurrentTurn() const;
    void NextTurn();
};