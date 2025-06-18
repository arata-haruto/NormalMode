#include "TurnManager.h"
#include "DxLib.h"

TurnManager* TurnManager::instance = nullptr;

TurnManager::TurnManager() : currentTurn(Turn::Player1), turnChanged(false), displayTimer(0.0f), turnMessageAlpha(0),
turnMessageY(200),
turnMessageState(0), // 0:非表示
turnMessageAnimationTimer(0)
{
}

TurnManager* TurnManager::GetInstance() {
    if (!instance) {
        instance = new TurnManager();
    }
    return instance;
}

void TurnManager::Reset() {
    currentTurn = Turn::Player1;       
    turnChanged = false;              
    displayTimer = 0.0f;               
    turnMessage = "";                 
    turnMessageAlpha = 0;
    turnMessageState = 0;      
    turnMessageAnimationTimer = 0.0f;  
}

void TurnManager::Update(float deltaTime) {
    if (turnChanged) {
        displayTimer -= deltaTime;
        if (displayTimer <= 0.0f) {
            turnChanged = false;
        }
    }

    UpdateTurnMessageAnimation(deltaTime);
}

Turn TurnManager::GetCurrentTurn() const {
    return currentTurn;
}

bool TurnManager::ShowTurnMessage() const {
    return turnMessageState != 0;
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

    turnMessageState = 1; 
    turnMessageAlpha = 0;
    turnMessageY = 200;   
    turnMessageAnimationTimer = 60; 

    turnMessage = (currentTurn == Turn::Player1) ? "1Pのターン" : "2Pのターン";
}

void TurnManager::SetInitialTurn(Turn initialPlayer) {
    currentTurn = initialPlayer;
    turnChanged = true; 
    displayTimer = 1.0f; 

    
    turnMessageState = 1; 
    turnMessageAlpha = 0;
    turnMessageY = 200;
    turnMessageAnimationTimer = 60; 

    turnMessage = (currentTurn == Turn::Player1) ? "1Pの先攻！" : "2Pの先攻！";
   
}

void TurnManager::UpdateTurnMessageAnimation(float deltaTime) {
    if (turnMessageState == 1) { 
        turnMessageAnimationTimer--;
        turnMessageAlpha = static_cast<int>(200 * (1.0f - static_cast<float>(turnMessageAnimationTimer) / 60.0f));
        if (turnMessageAnimationTimer <= 0) {
            turnMessageState = 2; 
            turnMessageAnimationTimer = 90; 
        }
        // スライドインアニメーション
        turnMessageY = 200 + static_cast<int>((static_cast<float>(turnMessageAnimationTimer) / 60.0f) * 100);

    }
    else if (turnMessageState == 2) {
        turnMessageAnimationTimer--;
        if (turnMessageAnimationTimer <= 0) {
            turnMessageState = 3; 
            turnMessageAnimationTimer = 30;
        }
    }
    else if (turnMessageState == 3) { // フェードアウト
        turnMessageAnimationTimer--;
        turnMessageAlpha = static_cast<int>(200 * (static_cast<float>(turnMessageAnimationTimer) / 30.0f));
        if (turnMessageAnimationTimer <= 0) {
            turnMessageState = 0;
            turnMessageAlpha = 0;
            turnMessageY = 300; 
        }
        // スライドアウトアニメーション
        turnMessageY = 300 - static_cast<int>((static_cast<float>(turnMessageAnimationTimer) / 30.0f) * 50); 
    }
}

void TurnManager::DrawTurnMessageOverlay() const {
    if (turnMessageState != 0) { // アニメーション中のみ描画
        // 半透明の背景オーバーレイ
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, turnMessageAlpha * 0.5); 
        DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE); 
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモードをリセット

        // ターンメッセージの描画
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, turnMessageAlpha); 

        unsigned int textColor = GetColor(255, 255, 255); // デフォルト白
        if (currentTurn == Turn::Player1) {
            textColor = GetColor(0, 255, 255); // 1Pはシアン
        }
        else {
            textColor = GetColor(255, 0, 255); // 2Pはマゼンタ
        }

        SetFontSize(80); 

        int string_width = GetDrawStringWidth(turnMessage.c_str(), strlen(turnMessage.c_str()));
        DrawString(640 - string_width / 2, turnMessageY, turnMessage.c_str(), textColor);

        SetFontSize(16); 
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモードをリセット
    }
}