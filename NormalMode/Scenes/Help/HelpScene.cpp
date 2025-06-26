#include "HelpScene.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/Vector2D.h"

namespace {
	const int SECTION_TITLE_FONT_SIZE = 36; 
	const int CONTENT_FONT_SIZE = 24;      
	const int BACK_BUTTON_FONT_SIZE = 24;   

	const int SECTION_TITLE_OFFSET_X = 150; 
	const int CONTENT_OFFSET_X = 50;        
	const int INITIAL_START_Y = 50;         
	const int LINE_SPACING = 35;          

	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
}

void HelpScene::Initialize()
{
	// 背景画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int> tmp;

    help_font_handle = CreateFontToHandle(
        "Resource/JiyunoTsubasa.ttf", 32, 3,
        DX_FONTTYPE_ANTIALIASING_EDGE, -1, 0, true);

    cancel_sound = rm->GetSoundResource("Resource/Sound/キャンセル5.mp3");

	//help_back_image = rm->GetImageResource("Resource/Images/Help.png")[0];
}

eSceneType HelpScene::Update(float delta_second)
{

	InputManager* input = InputManager::GetInstance();

	if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed ||
		input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Pressed)
	{
        PlaySoundMem(cancel_sound, DX_PLAYTYPE_BACK);
		return eSceneType::eTitle;
	}

	//Draw();

	return GetNowSceneType();
}

void HelpScene::Draw() const
{
	//タイトル背景画像の描画;
	DrawGraph(0, 0, help_back_image, TRUE);

    int current_y = INITIAL_START_Y; // 現在の描画Y座標

    // ゲームの概要
    SetFontSize(SECTION_TITLE_FONT_SIZE);
    DrawFormatStringToHandle(SECTION_TITLE_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "ゲーム概要");
    current_y += SECTION_TITLE_FONT_SIZE + 10;

    SetFontSize(CONTENT_FONT_SIZE);
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "・2人のプレイヤーが交互に敵を攻撃していくゲームです");
    current_y += LINE_SPACING;
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "・ただし、敵のHPを0にしたプレイヤーは敗北となり、");
    current_y += LINE_SPACING;
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "　もう一方のプレイヤーが勝利します。");
    current_y += LINE_SPACING;
   
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "・敵の部位ごとに与えられるダメージが異なります。");
    current_y += LINE_SPACING;
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "頭部>胴体＞脚の順でダメージが大きい");
    current_y += LINE_SPACING * 2;

    // 操作方法
    SetFontSize(SECTION_TITLE_FONT_SIZE);
    DrawFormatStringToHandle(SECTION_TITLE_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "操作方法");
    current_y += SECTION_TITLE_FONT_SIZE + 10;

    SetFontSize(CONTENT_FONT_SIZE);
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "・十字キー↑↓：部位選択");
    current_y += LINE_SPACING;
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "・Bボタン：攻撃");
    current_y += LINE_SPACING;
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "・Yボタン：回復");
    current_y += LINE_SPACING;
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "・Xボタン：パス");
    current_y += LINE_SPACING;
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "・Aボタン：決定");
    current_y += LINE_SPACING * 2;

    
    SetFontSize(BACK_BUTTON_FONT_SIZE);
   
    const char* back_message = "Aボタンを押してタイトル画面に戻る";
    int message_width = GetDrawStringWidth(back_message, strlen(back_message));
    DrawFormatString(SCREEN_WIDTH / 2 - message_width / 2, SCREEN_HEIGHT - 60, GetColor(255, 255, 255), back_message);

   
    SetFontSize(16);
}

void HelpScene::Finalize()
{

}

eSceneType HelpScene::GetNowSceneType() const
{
	return eSceneType::eHelp;
}
