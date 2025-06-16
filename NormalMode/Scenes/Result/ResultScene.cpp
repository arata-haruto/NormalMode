#include "ResultScene.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/ResourceManager.h"
#include "DxLib.h"

bool ResultScene::s_isGameOver = false;
Turn ResultScene::s_winningPlayer = Turn::Player1;

// UIレイアウト定数
namespace {
    const int RESULT_TITLE_FONT_SIZE = 60; // "RESULT" または "GAME OVER" / "WIN!" のフォントサイズ
    const int MESSAGE_FONT_SIZE = 28;      // メッセージのフォントサイズ
    const int TITLE_POS_Y = 200;           // 結果タイトルのY座標
    const int MESSAGE_POS_Y = 350;         // メッセージのY座標

    // 画面サイズ (DrawBoxなどで使用)
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
}

void ResultScene::Initialize()
{
    ResourceManager* rm = ResourceManager::GetInstance();

    // 背景画像を読み込む場合（リソース名は適宜変更してください）
    // result_back_image = rm->GetImageResource("Resource/Images/Result.png")[0];
}

eSceneType ResultScene::Update(float delta_second)
{
    InputManager* input = InputManager::GetInstance();

    // スペースキーかAボタンでタイトル画面に戻る
    if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed ||
        input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
    {
        return eSceneType::eTitle;
    }

    //Draw();

    return GetNowSceneType();
}

void ResultScene::Draw() const
{
    // 背景画像があれば描画
    if (result_back_image != 0)
    {
        DrawGraph(0, 0, result_back_image, TRUE);
    }
    else
    {
        // 背景画像がなければ画面をクリアして文字だけ表示
        ClearDrawScreen();
    }
    const char* resultTitle;
    unsigned int titleColor;

    if (s_isGameOver) { // 静的変数 s_isGameOver の値を使用
        resultTitle = "GAME OVER";
        titleColor = GetColor(255, 50, 50); // 赤色
    }
    else {
        if (s_winningPlayer == Turn::Player1) {
            resultTitle = "1P WIN!";
            titleColor = GetColor(0, 255, 255); // シアン (1Pの色と合わせる)
        }
        else { // Turn::Player2 の場合
            resultTitle = "2P WIN!";
            titleColor = GetColor(255, 0, 255); // マゼンタ (2Pの色と合わせる)
        }
    }

    SetFontSize(RESULT_TITLE_FONT_SIZE);
    // テキストを水平方向の中央に配置
    int title_string_width = GetDrawStringWidth(resultTitle, strlen(resultTitle));
    DrawFormatString(SCREEN_WIDTH / 2 - title_string_width / 2, TITLE_POS_Y, titleColor, resultTitle);

    // 終了メッセージの描画
    const char* exitMessage = "スペースキー / Aボタンを押してタイトルに戻る";
    SetFontSize(MESSAGE_FONT_SIZE);
    // テキストを水平方向の中央に配置
    int message_string_width = GetDrawStringWidth(exitMessage, strlen(exitMessage));
    DrawFormatString(SCREEN_WIDTH / 2 - message_string_width / 2, MESSAGE_POS_Y, GetColor(255, 255, 255), exitMessage);

    SetFontSize(16);
}

void ResultScene::Finalize()
{
    // 必要ならリソース解放処理
}

eSceneType ResultScene::GetNowSceneType() const
{
    return eSceneType::eResult;
}

void ResultScene::SetGameResult(bool isGameOver, Turn winningPlayer) {
    s_isGameOver = isGameOver;
    s_winningPlayer = winningPlayer; 
}