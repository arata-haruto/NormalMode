#pragma once
#include "../SceneBase.h"
#include "../../Objects/TurnManager.h"

class ResultScene : public SceneBase
{
private:
    int result_back_image; // ���U���g�w�i�摜�i�K�v�Ȃ�j
    static bool s_isGameOver;
    static Turn s_winningPlayer;
public:
    virtual void Initialize() override;
    virtual eSceneType Update(float delta_second) override;
    virtual void Draw() const override;
    virtual void Finalize() override;
    virtual eSceneType GetNowSceneType() const override;

    static void SetGameResult(bool isGameOver, Turn winningPlayer = Turn::Player1);
};