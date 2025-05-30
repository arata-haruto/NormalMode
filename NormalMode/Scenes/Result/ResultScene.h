#pragma once
#include "../SceneBase.h"

class ResultScene : public SceneBase
{
private:
    int result_back_image; // リザルト背景画像（必要なら）

public:
    virtual void Initialize() override;
    virtual eSceneType Update(float delta_second) override;
    virtual void Draw() const override;
    virtual void Finalize() override;
    virtual eSceneType GetNowSceneType() const override;
};