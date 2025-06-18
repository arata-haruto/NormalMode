#pragma once
#include "../SceneBase.h"
#include "../../Objects/TurnManager.h"
#include <vector>
//#include "../../Objects/GameObjectManager.h"

class GameObjectManager;
class Player;
class Enemy;

enum class GameState {
	Initializing,       // 初期化中 (アセットロードなど)
	//ReadyToStart,       // 開始待ち (STARTボタン入力待ち)
	TurnDeciding,       // 先攻後攻決定中 (ランダム決定と表示)
	Playing,            // ゲームプレイ中
	TransitionToResult, // リザルトへの遷移演出中
	Result,             // リザルト画面
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
	int back_ground_image;	//背景画像
	int heal_icon_image;	
	int pass_icon_image;	

	int iconWidth = 70;
	int iconHeight =70;

	int mainbgm;//ゲームメインのBGＭ
	int warning_sound;
	int turn_start_sound;

	bool result_reserved = false;
	const float warning_duration = 2.0f;
	float warning_timer = 0.0f;
	bool is_warning = false;
	bool warning_effect_active = false;
	bool go_to_result = false;

	GameState currentGameState; // 現在のゲーム状態

	bool initialTurnDecided;          // 先攻後攻が既に決定されたか
	float initialTurnDecisionTimer;   // 先攻後攻決定演出用タイマー
	Turn firstPlayer;                 // 先攻になったプレイヤー
	// リザルト遷移演出用
	bool isGameOver;          // ゲームオーバーか (true:ゲームオーバー, false:勝利)
	int transitionAlpha;      // 遷移演出の透明度 (0-255)
	int transitionTimer;      // 遷移演出のタイマー (フレーム数)

public:
	InGameScene();
	virtual void Initialize() override;
	virtual eSceneType Update(float delta_second) override;
	virtual void Draw() const override;
	virtual void Finalize() override;
	virtual eSceneType GetNowSceneType() const override;

	void StartResultTransition(bool gameOver); // リザルト遷移演出を開始する
	void UpdateTransition(float delta_second); // 遷移演出の更新
	void DrawTransition() const;             // 遷移演出の描画

private:
	void DeleteObject();
};

