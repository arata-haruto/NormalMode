#pragma once
#include "../SceneBase.h"
//#include "../../Objects/BackImageBase.h"

enum eSelectMenu
{
	eNONE,
	ePLAY,
	eHELP,
	eEXIT
};

class TitleScene : public SceneBase
{
public:
	/*std::vector<int> UI_num;
	std::vector<int> UI_string;
	std::vector<int> UI_coin;*/

	int pickup_color;
	int dropoff_color;
	int num_image;
	int string_image[26];
	int back_ground_image;	//�w�i�摜
	eSelectMenu select_menu;

	int cursor_sound;
	int decision_sound;

	int title_font;

	virtual void Initialize() override;
	virtual eSceneType Update(float delta_second) override;
	virtual void Draw() const override;
	virtual void Finalize() override;
	virtual eSceneType GetNowSceneType() const override;
	void SetUpSelectMenuType();
	void SetDownSelectMenuType();

private:
	int change_font_handle;
};

