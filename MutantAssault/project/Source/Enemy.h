#pragma once
#include "../Library/GameObject.h"
#include "animation.h"
#include "../Library/csvReader.h"
#include "Player.h"
#include "Stage.h"
#include "BehaviorTree.h"

/// <summary>
/// 敵の基底クラス
/// </summary>
class Enemy : public GameObject {
public:
	Enemy() {};
	virtual ~Enemy() {};
	virtual void TakeDamage(int damage) = 0;
	virtual void Attack() = 0;

	bool IsDeath() 
	{
		return isDeath;
	}
	VECTOR Position() 
	{
		return position;
	}
	bool HitCheck(VECTOR playerPos, VECTOR weaponLine1, VECTOR weaponLine2)
	{
		MV1SetupCollInfo(hModel, -1, 4, 4, 4, -1);
		MV1RefreshCollInfo(hModel);
		bool hit = false;
		for (int i = 1; i <= 4; i++) {
			VECTOR p1 = (weaponLine1 - lastLine1) * (i / 4.0f) + lastLine1;
			VECTOR p2 = (weaponLine2 - lastLine2) * (i / 4.0f) + lastLine2;
			MV1_COLL_RESULT_POLY res = MV1CollCheck_Line(hModel, -1, p1, p2);
			if (res.HitFlag) // 当たっている
			{
				hit = true;
			}
		}

		// ４本線のために、線情報を保存する
		lastLine1 = weaponLine1;
		lastLine2 = weaponLine2;
		return hit;
	}
protected:
	VECTOR position;
	VECTOR rotation;
	int hModel;
	int HP_MAX;
	int HP;
	int attack;
	int defense;
	bool isAttack;
	bool isDeath = false;

	//hitcheckに使う
	VECTOR lastLine1; // 前回の位置
	VECTOR lastLine2;
};
