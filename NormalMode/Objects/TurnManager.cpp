#include "TurnManager.h"
#include "DxLib.h"

TurnManager* TurnManager::instance = nullptr;

TurnManager::TurnManager() : currentTurn(Turn::Player1), turnChanged(false), displayTimer(0.0f), turnMessageAlpha(0),
turnMessageY(300), // ����Y���W�i��ʒ����t�߁j
turnMessageState(0), // 0:��\��
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

    turnMessageState = 1; // �t�F�[�h�C���J�n
    turnMessageAlpha = 0; // �ŏ��͓���
    turnMessageY = 200;   // ��ʏ㕔���班�����ɐݒ�
    turnMessageAnimationTimer = 60; // �t�F�[�h�C���̃A�j���[�V�������� (��: 1�b = 60�t���[��)

    turnMessage = (currentTurn == Turn::Player1) ? "1P�̃^�[��" : "2P�̃^�[��";
}

void TurnManager::SetInitialTurn(Turn initialPlayer) {
    currentTurn = initialPlayer;
    turnChanged = true; // �K�v�ł���΁A�Â��V�X�e���Ƃ̌݊����̂���
    displayTimer = 1.0f; // �Â����b�Z�[�W�\���p�^�C�}�[

    // �V�����^�[�����o���J�n
    turnMessageState = 1; // �t�F�[�h�C���J�n
    turnMessageAlpha = 0;
    turnMessageY = 200;
    turnMessageAnimationTimer = 60; // �t�F�[�h�C���̃A�j���[�V�������� (1�b = 60�t���[��)

    turnMessage = (currentTurn == Turn::Player1) ? "1P�̐�U�I" : "2P�̐�U�I";
    // �ŏ��̃^�[���J�nSE��InGameScene�Ŗ炵�Ă���̂ŁA�����ł͖炳�Ȃ�
}

void TurnManager::UpdateTurnMessageAnimation(float deltaTime) {
    if (turnMessageState == 1) { // �t�F�[�h�C��
        turnMessageAnimationTimer--;
        turnMessageAlpha = static_cast<int>(255 * (1.0f - static_cast<float>(turnMessageAnimationTimer) / 60.0f));
        if (turnMessageAnimationTimer <= 0) {
            turnMessageState = 2; // �Œ�\����
            turnMessageAnimationTimer = 90; // �Œ�\������ (��: 1.5�b = 90�t���[��)
        }
        // �X���C�h�C���A�j���[�V����
        turnMessageY = 200 + static_cast<int>((static_cast<float>(turnMessageAnimationTimer) / 60.0f) * 100); // 200����300��

    }
    else if (turnMessageState == 2) { // �Œ�\��
        turnMessageAnimationTimer--;
        if (turnMessageAnimationTimer <= 0) {
            turnMessageState = 3; // �t�F�[�h�A�E�g��
            turnMessageAnimationTimer = 30; // �t�F�[�h�A�E�g���� (��: 0.5�b = 30�t���[��)
        }
    }
    else if (turnMessageState == 3) { // �t�F�[�h�A�E�g
        turnMessageAnimationTimer--;
        turnMessageAlpha = static_cast<int>(255 * (static_cast<float>(turnMessageAnimationTimer) / 30.0f));
        if (turnMessageAnimationTimer <= 0) {
            turnMessageState = 0; // ��\����
            turnMessageAlpha = 0;
            turnMessageY = 300; // �����ʒu�ɖ߂�
        }
        // �X���C�h�A�E�g�A�j���[�V����
        turnMessageY = 300 - static_cast<int>((static_cast<float>(turnMessageAnimationTimer) / 30.0f) * 50); // 300����250��
    }
}

void TurnManager::DrawTurnMessageOverlay() const {
    if (turnMessageState != 0) { // �A�j���[�V�������̂ݕ`��
        // �������̔w�i�I�[�o�[���C
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, turnMessageAlpha * 0.5); // �����x�𔼕��Ɂi��蔖���j
        DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE); // ��ʑS�̂��Â����� (��ʃT�C�Y�ɍ��킹�Ē���)
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // �u�����h���[�h�����Z�b�g

        // �^�[�����b�Z�[�W�̕`��
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, turnMessageAlpha); // ���b�Z�[�W�̓����x��ݒ�

        unsigned int textColor = GetColor(255, 255, 255); // �f�t�H���g��
        if (currentTurn == Turn::Player1) {
            textColor = GetColor(0, 255, 255); // 1P�̓V�A��
        }
        else {
            textColor = GetColor(255, 0, 255); // 2P�̓}�[���^
        }

        // �t�H���g�T�C�Y��ݒ�
        SetFontSize(80); // �傫�ȃt�H���g�T�C�Y

        // �e�L�X�g�̕����擾���Ē���������
        int string_width = GetDrawStringWidth(turnMessage.c_str(), strlen(turnMessage.c_str()));
        DrawString(640 - string_width / 2, turnMessageY, turnMessage.c_str(), textColor);

        SetFontSize(16); // �t�H���g�T�C�Y�����ɖ߂� (�f�t�H���g�T�C�Y)
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // �u�����h���[�h�����Z�b�g
    }
}