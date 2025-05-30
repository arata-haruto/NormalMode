#include "ResultScene.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/ResourceManager.h"
#include "DxLib.h"

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
        DxLib_End();   // DxLib終了処理
        exit(0);       // プログラム終了
    }

    Draw();

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

        SetFontSize(40);
        DrawFormatString(200, 200, GetColor(255, 255, 255), "RESULT");
        SetFontSize(20);
        DrawFormatString(150, 300, GetColor(255, 255, 255), "スペースキー/Aボタンを押して終了");
    }
}

void ResultScene::Finalize()
{
    // 必要ならリソース解放処理
}

eSceneType ResultScene::GetNowSceneType() const
{
    return eSceneType::eResult;
}