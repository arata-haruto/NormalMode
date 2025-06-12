#include "TurnManager.h"
#include "DxLib.h"

TurnManager* TurnManager::instance = nullptr;

TurnManager::TurnManager() : currentTurn(Turn::Player1), turnChanged(false), displayTimer(0.0f), turnMessageAlpha(0),
turnMessageY(300), // 初期Y座標（画面中央付近）
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

    turnMessageState = 1; // フェードイン開始
    turnMessageAlpha = 0; // 最初は透明
    turnMessageY = 200;   // 画面上部から少し下に設定
    turnMessageAnimationTimer = 60; // フェードインのアニメーション時間 (例: 1秒 = 60フレーム)

    turnMessage = (currentTurn == Turn::Player1) ? "1Pのターン" : "2Pのターン";
}

void TurnManager::SetInitialTurn(Turn initialPlayer) {
    currentTurn = initialPlayer;
    turnChanged = true; // 必要であれば、古いシステムとの互換性のため
    displayTimer = 1.0f; // 古いメッセージ表示用タイマー

    // 新しいターン演出を開始
    turnMessageState = 1; // フェードイン開始
    turnMessageAlpha = 0;
    turnMessageY = 200;
    turnMessageAnimationTimer = 60; // フェードインのアニメーション時間 (1秒 = 60フレーム)

    turnMessage = (currentTurn == Turn::Player1) ? "1Pの先攻！" : "2Pの先攻！";
    // 最初のターン開始SEはInGameSceneで鳴らしているので、ここでは鳴らさない
}

void TurnManager::UpdateTurnMessageAnimation(float deltaTime) {
    if (turnMessageState == 1) { // フェードイン
        turnMessageAnimationTimer--;
        turnMessageAlpha = static_cast<int>(255 * (1.0f - static_cast<float>(turnMessageAnimationTimer) / 60.0f));
        if (turnMessageAnimationTimer <= 0) {
            turnMessageState = 2; // 固定表示へ
            turnMessageAnimationTimer = 90; // 固定表示時間 (例: 1.5秒 = 90フレーム)
        }
        // スライドインアニメーション
        turnMessageY = 200 + static_cast<int>((static_cast<float>(turnMessageAnimationTimer) / 60.0f) * 100); // 200から300へ

    }
    else if (turnMessageState == 2) { // 固定表示
        turnMessageAnimationTimer--;
        if (turnMessageAnimationTimer <= 0) {
            turnMessageState = 3; // フェードアウトへ
            turnMessageAnimationTimer = 30; // フェードアウト時間 (例: 0.5秒 = 30フレーム)
        }
    }
    else if (turnMessageState == 3) { // フェードアウト
        turnMessageAnimationTimer--;
        turnMessageAlpha = static_cast<int>(255 * (static_cast<float>(turnMessageAnimationTimer) / 30.0f));
        if (turnMessageAnimationTimer <= 0) {
            turnMessageState = 0; // 非表示へ
            turnMessageAlpha = 0;
            turnMessageY = 300; // 初期位置に戻す
        }
        // スライドアウトアニメーション
        turnMessageY = 300 - static_cast<int>((static_cast<float>(turnMessageAnimationTimer) / 30.0f) * 50); // 300から250へ
    }
}

void TurnManager::DrawTurnMessageOverlay() const {
    if (turnMessageState != 0) { // アニメーション中のみ描画
        // 半透明の背景オーバーレイ
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, turnMessageAlpha * 0.5); // 透明度を半分に（より薄く）
        DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE); // 画面全体を暗くする (画面サイズに合わせて調整)
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモードをリセット

        // ターンメッセージの描画
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, turnMessageAlpha); // メッセージの透明度を設定

        unsigned int textColor = GetColor(255, 255, 255); // デフォルト白
        if (currentTurn == Turn::Player1) {
            textColor = GetColor(0, 255, 255); // 1Pはシアン
        }
        else {
            textColor = GetColor(255, 0, 255); // 2Pはマゼンタ
        }

        // フォントサイズを設定
        SetFontSize(80); // 大きなフォントサイズ

        // テキストの幅を取得して中央揃えに
        int string_width = GetDrawStringWidth(turnMessage.c_str(), strlen(turnMessage.c_str()));
        DrawString(640 - string_width / 2, turnMessageY, turnMessage.c_str(), textColor);

        SetFontSize(16); // フォントサイズを元に戻す (デフォルトサイズ)
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモードをリセット
    }
}