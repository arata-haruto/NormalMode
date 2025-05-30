#include "ResultScene.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/ResourceManager.h"
#include "DxLib.h"

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
        DxLib_End();   // DxLib�I������
        exit(0);       // �v���O�����I��
    }

    Draw();

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

        SetFontSize(40);
        DrawFormatString(200, 200, GetColor(255, 255, 255), "RESULT");
        SetFontSize(20);
        DrawFormatString(150, 300, GetColor(255, 255, 255), "�X�y�[�X�L�[/A�{�^���������ďI��");
    }
}

void ResultScene::Finalize()
{
    // �K�v�Ȃ烊�\�[�X�������
}

eSceneType ResultScene::GetNowSceneType() const
{
    return eSceneType::eResult;
}