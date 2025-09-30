#pragma once
#include <DxLib.h>
#include "../Library/GameObject.h"
#include "Screen.h"

/// <summary>
/// �J�����𑀍삷��N���X
/// </summary>
class Camera : public GameObject {
public:
	Camera();
	~Camera();
	void Update() override;
	void Draw() override;
	VECTOR Direction()
	{
		return direction;
	}
	VECTOR Position()
	{
		return position;
	}
	void Event(bool _event) {
		event = _event;
	}
	BOOL GetEventNow() {
		return event;
	}
private:
	VECTOR position; // �J�������W
	VECTOR rotation; // �J��������p�̉�]
	VECTOR target; // �����_
	VECTOR direction;	//�J�����̌����Ă�������̒P��

	int RStick_X;
	int RStick_Y;
	float StickPower;
	float cameraMovePower;

	int centerX = Screen::WIDTH / 2;
	int centerY = Screen::HEIGHT / 2;
	int mouseX;
	int mouseY;
	float mouseSensitivity; //�}�E�X���x


	bool LockOn;
	bool LockOnKey;
	bool LockEnd_X;
	bool LockEnd_Y;
	int lockonImage;

	void LockOnCamera();

	bool event; 
};