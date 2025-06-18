#pragma once
#include "../SceneBase.h"
//#include "../../Objects/BackImageBase.h"

class HelpScene : public SceneBase
{
private:

public:
	int help_back_image;	//ƒwƒ‹ƒv‚Ì”wŒi‰æ‘œ

	int cancel_sound;

	virtual void Initialize() override;
	virtual eSceneType Update(float delta_second) override;
	virtual void Draw() const override;
	virtual void Finalize() override;
	virtual eSceneType GetNowSceneType() const override;

};

