#pragma once
#include "../Utility/Singleton.h"
#include "SceneBase.h"

class SceneManager : public Singleton <SceneManager>
{
public:
	class SceneBase* current_scene;//Œ»İ‚ÌƒV[ƒ“î•ñ

	SceneManager();

	void Initialize();
	bool Update(float delta_second);
	void Finalize();


private:
	bool ChangeScene(enum eSceneType type);

};

