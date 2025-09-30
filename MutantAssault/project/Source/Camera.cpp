#include "Camera.h"
#include "Player.h"
#include <math.h>
#include "Enemy.h"
#include "TargetManager.h"

namespace {
	Enemy* lockonEnemy;
}
Camera::Camera()
{
	rotation = VGet(0, 0, 0);

	RStick_X = 0;
	RStick_Y = 0;
	StickPower = 0;
	cameraMovePower = 0.2f;

	LockOn = false;
	LockEnd_X = true;
	LockEnd_Y = true;
	lockonImage = LoadGraph("data/image/UI/lockon.png");

	mouseSensitivity = 0.0005f;
	//pl = ObjectManager::FindGameObject<Player>();
}

Camera::~Camera()
{
	DeleteGraph(lockonImage);
}

void Camera::Update()
{
	//pad
	GetJoypadAnalogInputRight(&RStick_X, &RStick_Y, DX_INPUT_PAD1);
	if (RStick_X != 0) 
	{
		StickPower = RStick_X / 100;
		rotation.y += DegToRad(StickPower * cameraMovePower);
	}
	if (RStick_Y != 0) 
	{
		StickPower = -RStick_Y / 100;
		rotation.x -= DegToRad(StickPower * cameraMovePower);
	}

	//key
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		rotation.y += DegToRad(3.0f);
	}
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		rotation.y -= DegToRad(3.0f);
	}
	if (CheckHitKey(KEY_INPUT_UP) && !LockOn) {
		rotation.x += DegToRad(3.0f);
	}
	if (CheckHitKey(KEY_INPUT_DOWN) && !LockOn) {
		rotation.x -= DegToRad(3.0f);
	}

	//mouse
	GetMousePoint(&mouseX, &mouseY);
	int deltaX = mouseX - centerX;
	int deltaY = mouseY - centerY;
	rotation.y += deltaX * mouseSensitivity;
	rotation.x -= deltaY * mouseSensitivity;
	SetMousePoint(centerX, centerY);

	//制限
	if (rotation.x > DegToRad(60.0f)) {
		rotation.x = DegToRad(60.0f);
	}
	if (rotation.x < DegToRad(-60.0f)) {
		rotation.x = DegToRad(-60.0f);
	}


	Player* pl = ObjectManager::FindGameObject<Player>();
	VECTOR pPos = pl->Position();
	VECTOR pLook= pl->LookTarget();
	//positionとtargetを求める
	//カメラの位置
	VECTOR v = pPos - pLook;
	v.y = 0;
	v = VNorm(v) * 330.0f;
	v.y = 250.0f;// -100.0f; //カメラの高さ
	//targetの位置
	VECTOR center = pPos + VGet(0, 100, 0);//playerキャラの心臓
	MATRIX mRot = MGetRotX(rotation.x) * MGetRotY(rotation.y);
	position = center + v * mRot;	//カメラ座標
	VECTOR lk = pLook - center;
	target = center + lk * mRot;	//注視点
	direction = target - position;
	direction.y = 0;
	direction = VNorm(direction);

	LockOnCamera();	
}

void Camera::Draw()
{
	if (event) {
		//敵の登場演出の名前を仮実装
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		int nameImage;
		nameImage = LoadGraph("data/image/mutant_nameLogo.png");
		DrawGraph(640, 100, nameImage, true);
		//DrawExtendGraph(320 * 2, 180 * 3, 320 * 4, 180 * 6 - 100, nameImage, true);
	}
	else {
		SetCameraPositionAndTarget_UpVecY(position, target);
	}
	if (LockOn && lockonEnemy != nullptr) {
		VECTOR enemyPos = lockonEnemy->Position() + VGet(0, 150, 0);
		enemyPos = ConvWorldPosToScreenPos(enemyPos);	// 3D → 2D 変換
		//TODO:ロックオンマークが隠れちゃう
		DrawRotaGraph(enemyPos.x, enemyPos.y, 0.3f, 0.0f, lockonImage, TRUE);
	}
}

void Camera::LockOnCamera()
{
	Player* pl = ObjectManager::FindGameObject<Player>();
	if (CheckHitKey(KEY_INPUT_B) || (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_12))
	{
		if (!LockOnKey) {
			if (!LockOn) {
				float d = 10000;
				for (Enemy* e : TargetManager::getEnemy()) {
					float vs = VSize(pl->Position() - e->Position());
					if (d > vs) {
						d = vs;
						lockonEnemy = e;
					}
				}
				LockOn = true;
				LockEnd_X = false;
				LockEnd_Y = false;
			}
			else {
				LockOn = false;
				LockEnd_X = false;
				LockEnd_Y = false;
			}
		}
		LockOnKey = true;
	}
	else {
		LockOnKey = false;
	}

	if (LockOn) {
		if (!LockEnd_Y) {
			//Y軸の回転
			VECTOR vec1 = direction;
			VECTOR vec2 = lockonEnemy->Position();

			VECTOR moveVec = vec2 - position;
			moveVec.y = 0;
			float rotateTarget = atan2(moveVec.x, moveVec.z);
			float diff = rotateTarget - rotation.y;
			while (diff > DX_PI)
			{
				diff -= DX_PI * 2.0f;
			}
			while (diff < -DX_PI)
			{
				diff += DX_PI * 2.0f;
			}
			//3.角度差が一定角度（π/4）以内であれば、rotation.y = rotateTarget
			float limit = DX_PI / 20.0f;
			if (diff < limit && diff > -limit)
			{
				rotation.y = rotateTarget;
				//LockOn = false;
				//LockEnd_Y = true;
			}
			else {
				//4．値が+であれば右回転、値が-であれば左回転
				if (diff > 0) {
					rotation.y += limit;
				}
				else {
					rotation.y -= limit;
				}
			}
		}

		if (!LockEnd_X) {

			if (rotation.x > DegToRad(60.0f)) {
				rotation.x = DegToRad(59.0f);
			}
			if (rotation.x < DegToRad(-60.0f)) {
				rotation.x = DegToRad(-59.0f);
			}

			//X軸の移動
			VECTOR vec3 = direction;
			VECTOR vec4 = lockonEnemy->Position();
			//vec4.y = vec4.y+150;

			VECTOR moveVecX = vec4 - position;
			moveVecX.y = 0;
			moveVecX.z = 0;
			float rotateTargetX = atan2(moveVecX.y, moveVecX.z);
			float diffX = rotateTargetX - rotation.x;
			while (diffX > DX_PI)
			{
				diffX -= DX_PI * 2.0f;
			}
			while (diffX < -DX_PI)
			{
				diffX += DX_PI * 2.0f;
			}
			//3.角度差が一定角度（π/4）以内であれば、rotation.y = rotateTarget
			float limitX = DX_PI / 18.0f;
			if (rotation.x > DegToRad(60.0f)) {
				LockEnd_X = true;
				return;
			}
			if (rotation.x < DegToRad(-60.0f)) {
				LockEnd_X = true;
				return;
			}
			if (diffX < limitX && diffX > -limitX)
			{
				rotation.x = rotateTargetX;
				//LockOn = false;
				//LockEnd_X = true;
			}
			else {
				//4．値が+であれば右回転、値が-であれば左回転
				if (diffX > 0) {
					rotation.x += limitX;
					if (rotation.x > DegToRad(60.0f))
						LockEnd_X = true;
					//rotation.x = 0;
				//LockEnd_X = true;
				}
				else {
					rotation.x -= limitX;
					if (rotation.x < DegToRad(-60.0f))
						LockEnd_X = true;
					//rotation.x = 0;
				//LockEnd_X = true;
				}
			}
		}

		if (LockEnd_X && LockEnd_Y) {
			LockOn = false;
		}

	}

}
