#pragma once
#include "../Library/SceneBase.h"
#include "../Library/GameObject.h"
#include "Player.h"
#include "Screen.h"

/// <summary>
/// リザルトシーン
/// 
/// スコアを表示、キーを押したらタイトルに移動
/// </summary>
class ResultScene : public SceneBase {
public:
	ResultScene();
	~ResultScene();
	void Update() override;
	void Draw() override;
	
private:
	int BGimage;
	int resultBGM;
	float timer;

	//pad入力
	DINPUT_JOYSTATE inputPad;

	//スコアなど
	int panel;
	int hitDamegeCount;
	int minutes;
	int seconds;

	//スタンプ
	int stampImage;
	bool stamped;
	int size;
	int stampSE;
};
