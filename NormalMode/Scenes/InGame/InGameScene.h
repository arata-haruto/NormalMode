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
	Player* player1;
	Player* player2;

	/*std::vector<int> UI_num;
	std::vector<int> UI_string;
	std::vector<int>UI_time;*/
	int num_image;
	int num_time;
	int back_ground_image;	//”wŒi‰æ‘œ
	int heal_icon_image;	
	int pass_icon_image;	

	int iconWidth = 90;
	int iconHeight =90;

	int mainbgm;//ƒQ[ƒ€ƒƒCƒ“‚ÌBG‚l
	int warning_sound;


	bool result_reserved = false;
	const float warning_duration = 2.0f;
	float warning_timer = 0.0f;
	bool is_warning = false;
	bool warning_effect_active = false;
	bool go_to_result = false;

public:
	virtual void Initialize() override;
	virtual eSceneType Update(float delta_second) override;
	virtual void Draw() const override;
	virtual void Finalize() override;
	virtual eSceneType GetNowSceneType() const override;
	//virtual void CheckCollision(GameObject* target, GameObject* partner) override;

private:
	void DeleteObject();
};

