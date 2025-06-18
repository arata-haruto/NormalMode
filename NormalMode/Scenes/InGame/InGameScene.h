#pragma once
#include "../SceneBase.h"
#include "../../Objects/TurnManager.h"
#include <vector>
//#include "../../Objects/GameObjectManager.h"

class GameObjectManager;
class Player;
class Enemy;

enum class GameState {
	Initializing,       // �������� (�A�Z�b�g���[�h�Ȃ�)
	//ReadyToStart,       // �J�n�҂� (START�{�^�����͑҂�)
	TurnDeciding,       // ��U��U���蒆 (�����_������ƕ\��)
	Playing,            // �Q�[���v���C��
	TransitionToResult, // ���U���g�ւ̑J�ډ��o��
	Result,             // ���U���g���
};

class InGameScene : public SceneBase
{
private:
	class GameObjectManager* objm;
	//class Player* player;
	//class Enemy* enemy;
	bool start_flg;
	bool pause_flg;
	//Player* p;
	Player* player1;
	Player* player2;

	/*std::vector<int> UI_num;
	std::vector<int> UI_string;
	std::vector<int>UI_time;*/
	int num_image;
	int num_time;
	int back_ground_image;	//�w�i�摜
	int heal_icon_image;	
	int pass_icon_image;	

	int iconWidth = 70;
	int iconHeight =70;

	int mainbgm;//�Q�[�����C����BG�l
	int warning_sound;
	int turn_start_sound;

	bool result_reserved = false;
	const float warning_duration = 2.0f;
	float warning_timer = 0.0f;
	bool is_warning = false;
	bool warning_effect_active = false;
	bool go_to_result = false;

	GameState currentGameState; // ���݂̃Q�[�����

	bool initialTurnDecided;          // ��U��U�����Ɍ��肳�ꂽ��
	float initialTurnDecisionTimer;   // ��U��U���艉�o�p�^�C�}�[
	Turn firstPlayer;                 // ��U�ɂȂ����v���C���[
	// ���U���g�J�ډ��o�p
	bool isGameOver;          // �Q�[���I�[�o�[�� (true:�Q�[���I�[�o�[, false:����)
	int transitionAlpha;      // �J�ډ��o�̓����x (0-255)
	int transitionTimer;      // �J�ډ��o�̃^�C�}�[ (�t���[����)

public:
	InGameScene();
	virtual void Initialize() override;
	virtual eSceneType Update(float delta_second) override;
	virtual void Draw() const override;
	virtual void Finalize() override;
	virtual eSceneType GetNowSceneType() const override;

	void StartResultTransition(bool gameOver); // ���U���g�J�ډ��o���J�n����
	void UpdateTransition(float delta_second); // �J�ډ��o�̍X�V
	void DrawTransition() const;             // �J�ډ��o�̕`��

private:
	void DeleteObject();
};

