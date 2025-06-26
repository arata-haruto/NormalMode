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
	// �w�i�摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int> tmp;

    help_font_handle = CreateFontToHandle(
        "Resource/JiyunoTsubasa.ttf", 32, 3,
        DX_FONTTYPE_ANTIALIASING_EDGE, -1, 0, true);

    cancel_sound = rm->GetSoundResource("Resource/Sound/�L�����Z��5.mp3");

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
	//�^�C�g���w�i�摜�̕`��;
	DrawGraph(0, 0, help_back_image, TRUE);

    int current_y = INITIAL_START_Y; // ���݂̕`��Y���W

    // �Q�[���̊T�v
    SetFontSize(SECTION_TITLE_FONT_SIZE);
    DrawFormatStringToHandle(SECTION_TITLE_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "�Q�[���T�v");
    current_y += SECTION_TITLE_FONT_SIZE + 10;

    SetFontSize(CONTENT_FONT_SIZE);
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "�E2�l�̃v���C���[�����݂ɓG���U�����Ă����Q�[���ł�");
    current_y += LINE_SPACING;
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "�E�������A�G��HP��0�ɂ����v���C���[�͔s�k�ƂȂ�A");
    current_y += LINE_SPACING;
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "�@��������̃v���C���[���������܂��B");
    current_y += LINE_SPACING;
   
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "�E�G�̕��ʂ��Ƃɗ^������_���[�W���قȂ�܂��B");
    current_y += LINE_SPACING;
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "����>���́��r�̏��Ń_���[�W���傫��");
    current_y += LINE_SPACING * 2;

    // ������@
    SetFontSize(SECTION_TITLE_FONT_SIZE);
    DrawFormatStringToHandle(SECTION_TITLE_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "������@");
    current_y += SECTION_TITLE_FONT_SIZE + 10;

    SetFontSize(CONTENT_FONT_SIZE);
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "�E�\���L�[�����F���ʑI��");
    current_y += LINE_SPACING;
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "�EB�{�^���F�U��");
    current_y += LINE_SPACING;
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "�EY�{�^���F��");
    current_y += LINE_SPACING;
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "�EX�{�^���F�p�X");
    current_y += LINE_SPACING;
    DrawFormatStringToHandle(CONTENT_OFFSET_X, current_y, GetColor(255, 255, 255), help_font_handle, "�EA�{�^���F����");
    current_y += LINE_SPACING * 2;

    
    SetFontSize(BACK_BUTTON_FONT_SIZE);
   
    const char* back_message = "A�{�^���������ă^�C�g����ʂɖ߂�";
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
