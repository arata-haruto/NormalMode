#pragma once
#include "../Utility/Vector2D.h"
//#include "../Objects/GameObjectManager.h"

enum eSceneType
{
	eTitle,
	eInGame,
	eResult,
	eHelp,
	eExit
};

class SceneBase
{
protected:
	int image;

private:
	Vector2D screen_offset;
	class GameObjectManager* objm;

public:
	/*SceneBase() : screen_offset(0.0f), objm(nullptr), image(0)
	{
		objm = GameObjectManager::GetInstance();
	}*/

	virtual ~SceneBase()
	{
		Finalize();
	}

	virtual void Initialize()
	{

	}

	virtual eSceneType Update(float delta_second)
	{
		//for (GameObject* obj : objm->GetObjectsList())
		//{
		//	//オブジェクトが画面内にあったら更新する
		//	obj->Update(delta_second);
		//}

		//if (GetNowSceneType() != eSceneType::eInGame)
		//{
		//	objm->CheckDstroyObject();
		//}

		return GetNowSceneType();
	}

	virtual void Draw() const
	{
		/*GameObjectManager* game = GameObjectManager::GetInstance();

		if (!game->GetObjectsList().empty())
		{
			for (GameObject* obj : game->GetObjectsList())
			{
				obj->Draw(screen_offset);
			}
		}*/
	}

	virtual void Finalize()
	{

	}

	virtual eSceneType GetNowSceneType() const = 0;

	/*virtual void CheckCollision(GameObject* target, GameObject* partner)
	{

	}*/
};

