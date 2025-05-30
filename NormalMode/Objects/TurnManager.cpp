#include "TurnManager.h"

TurnManager* TurnManager::instance = nullptr;

TurnManager::TurnManager() : currentTurn(Turn::Player1), turnChanged(false), displayTimer(0.0f)
{
}

TurnManager* TurnManager::GetInstance() {
    if (!instance) {
        instance = new TurnManager();
    }
    return instance;
}

void TurnManager::Update(float deltaTime) {
    if (turnChanged) {
        displayTimer -= deltaTime;
        if (displayTimer <= 0.0f) {
            turnChanged = false;
        }
    }
}

Turn TurnManager::GetCurrentTurn() const {
    return currentTurn;
}

bool TurnManager::ShowTurnMessage() const {
    return turnChanged;
}

const std::string& TurnManager::GetTurnMessage() const {
    return turnMessage;
}

float TurnManager::GetDisplayTimer() const {
    return displayTimer;
}

void TurnManager::NextTurn() {
    if (currentTurn == Turn::Player1) currentTurn = Turn::Player2;
    else currentTurn = Turn::Player1;

    turnChanged = true;
    displayTimer = 1.0f;
    turnMessage = (currentTurn == Turn::Player1) ? "1Pのターン" : "2Pのターン";
}