#pragma once
#include "../Library/SceneBase.h"
#include "../Library/GameObject.h"
#include "Player.h"
#include "Screen.h"

/// <summary>
/// ���U���g�V�[��
/// 
/// �X�R�A��\���A�L�[����������^�C�g���Ɉړ�
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

	//pad����
	DINPUT_JOYSTATE inputPad;

	//�X�R�A�Ȃ�
	int panel;
	int hitDamegeCount;
	int minutes;
	int seconds;

	//�X�^���v
	int stampImage;
	bool stamped;
	int size;
	int stampSE;
};
