#pragma once
#include "../SceneBase.h"
//#include "../../Objects/GameObjectManager.h"

class InGameScene : public SceneBase
{
private:
	class GameObjectManager* objm;
	class Player* player;
	class Enemy* enemy;
	bool start_flg;
	bool pause_flg;
	//Player* p;


	/*std::vector<int> UI_num;
	std::vector<int> UI_string;
	std::vector<int>UI_time;*/
	int num_image;
	int num_time;
	int num_world;
	int time_set;
	int time_add;
	int now_count;
	int back_ground_image;	//�w�i�摜


	int mainbgm;//�Q�[�����C����BG�l

public:
	virtual void Initialize() override;
	virtual eSceneType Update(float delta_second) override;
	virtual void Draw() const override;
	virtual void Finalize() override;
	virtual eSceneType GetNowSceneType() const override;
	//virtual void CheckCollision(GameObject* target, GameObject* partner) override;

private:
	void LoadStageMapCSV();
	void DrawBackGroundCSV() const;
	void DeleteObject();
};

