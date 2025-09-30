#pragma once
#include "../Library/SceneBase.h"
#include "DxLib.h"
#include "ScoreManager.h"

/// <summary>
/// �^�C�g���V�[��
/// 
/// �^�C�g����\�����āA�L�[����������v���C�V�[���Ɉڍs����B
/// </summary>
class TitleScene : public SceneBase {
public:
	TitleScene();
	~TitleScene();
	void Update() override;
	void Draw() override;
private:
	//BGM,SE
	int BGimage;
	unsigned int black;
	int bgm;
	int SE_Select;
	int SE_Decision;

	//ui�֘A
	int selectMenuNum;
	int selectMenuNum_Max = 3;
	int selectMenuNum_Min = 0;
	int selectDiffNum;
	int selectDiffNum_Max = 3;
	int selectDiffNum_Min = 0;
	bool selectKey_Up;
	bool selectKey_Down;
	int panel;
	int arrow;
	bool operationInstructions;
	bool tKey;
	bool spaceKey;
	int explanationImage_PS;
	int explanationImage_PC;

	//fade
	void FadeOut(int fadeTime);
	bool fadeOut;

	//pad����
	DINPUT_JOYSTATE inputPad;
	int crossKey;

	enum TitleState {
		MENU,
		EXPLANATION_PS,
		EXPLANATION_PC,
		DifficultySelect,
	};
	TitleState tState;

	void MenuUpdate();
	void ExplanationUpdatePS();
	void ExplanationUpdatePC();
	void DifficultySelectUpdate();
};