#pragma once
#include <DxLib.h>
#include "../Library/GameObject.h"
#include "Screen.h"

/// <summary>
/// カメラを操作するクラス
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
	VECTOR position; // カメラ座標
	VECTOR rotation; // カメラ操作用の回転
	VECTOR target; // 注視点
	VECTOR direction;	//カメラの向いている方向の単位

	int RStick_X;
	int RStick_Y;
	float StickPower;
	float cameraMovePower;

	int centerX = Screen::WIDTH / 2;
	int centerY = Screen::HEIGHT / 2;
	int mouseX;
	int mouseY;
	float mouseSensitivity; //マウス感度


	bool LockOn;
	bool LockOnKey;
	bool LockEnd_X;
	bool LockEnd_Y;
	int lockonImage;

	void LockOnCamera();

	bool event; 
};