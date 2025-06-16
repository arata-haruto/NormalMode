#include "ResultScene.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/ResourceManager.h"
#include "DxLib.h"

bool ResultScene::s_isGameOver = false;
Turn ResultScene::s_winningPlayer = Turn::Player1;

// UI���C�A�E�g�萔
namespace {
    const int RESULT_TITLE_FONT_SIZE = 60; // "RESULT" �܂��� "GAME OVER" / "WIN!" �̃t�H���g�T�C�Y
    const int MESSAGE_FONT_SIZE = 28;      // ���b�Z�[�W�̃t�H���g�T�C�Y
    const int TITLE_POS_Y = 200;           // ���ʃ^�C�g����Y���W
    const int MESSAGE_POS_Y = 350;         // ���b�Z�[�W��Y���W

    // ��ʃT�C�Y (DrawBox�ȂǂŎg�p)
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
}

void ResultScene::Initialize()
{
    ResourceManager* rm = ResourceManager::GetInstance();

    // �w�i�摜��ǂݍ��ޏꍇ�i���\�[�X���͓K�X�ύX���Ă��������j
    // result_back_image = rm->GetImageResource("Resource/Images/Result.png")[0];
}

eSceneType ResultScene::Update(float delta_second)
{
    InputManager* input = InputManager::GetInstance();

    // �X�y�[�X�L�[��A�{�^���Ń^�C�g����ʂɖ߂�
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
    // �w�i�摜������Ε`��
    if (result_back_image != 0)
    {
        DrawGraph(0, 0, result_back_image, TRUE);
    }
    else
    {
        // �w�i�摜���Ȃ���Ή�ʂ��N���A���ĕ��������\��
        ClearDrawScreen();
    }
    const char* resultTitle;
    unsigned int titleColor;

    if (s_isGameOver) { // �ÓI�ϐ� s_isGameOver �̒l���g�p
        resultTitle = "GAME OVER";
        titleColor = GetColor(255, 50, 50); // �ԐF
    }
    else {
        if (s_winningPlayer == Turn::Player1) {
            resultTitle = "1P WIN!";
            titleColor = GetColor(0, 255, 255); // �V�A�� (1P�̐F�ƍ��킹��)
        }
        else { // Turn::Player2 �̏ꍇ
            resultTitle = "2P WIN!";
            titleColor = GetColor(255, 0, 255); // �}�[���^ (2P�̐F�ƍ��킹��)
        }
    }

    SetFontSize(RESULT_TITLE_FONT_SIZE);
    // �e�L�X�g�𐅕������̒����ɔz�u
    int title_string_width = GetDrawStringWidth(resultTitle, strlen(resultTitle));
    DrawFormatString(SCREEN_WIDTH / 2 - title_string_width / 2, TITLE_POS_Y, titleColor, resultTitle);

    // �I�����b�Z�[�W�̕`��
    const char* exitMessage = "�X�y�[�X�L�[ / A�{�^���������ă^�C�g���ɖ߂�";
    SetFontSize(MESSAGE_FONT_SIZE);
    // �e�L�X�g�𐅕������̒����ɔz�u
    int message_string_width = GetDrawStringWidth(exitMessage, strlen(exitMessage));
    DrawFormatString(SCREEN_WIDTH / 2 - message_string_width / 2, MESSAGE_POS_Y, GetColor(255, 255, 255), exitMessage);

    SetFontSize(16);
}

void ResultScene::Finalize()
{
    // �K�v�Ȃ烊�\�[�X�������
}

eSceneType ResultScene::GetNowSceneType() const
{
    return eSceneType::eResult;
}

void ResultScene::SetGameResult(bool isGameOver, Turn winningPlayer) {
    s_isGameOver = isGameOver;
    s_winningPlayer = winningPlayer; 
}