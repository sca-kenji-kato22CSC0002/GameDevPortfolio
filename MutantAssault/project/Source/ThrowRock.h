#pragma once
#include "../Library/GameObject.h"
#include "Player.h"

/// <summary>
/// –‚—‚ª“Š‚°‚éŠâ
/// </summary>
class ThrowRock : public GameObject {
public:
	ThrowRock(VECTOR pos);
	~ThrowRock();
	void Update() override;
	void Draw() override;
	void SetTarget(VECTOR tPos);
private:
	int hRock;
	VECTOR position;
	VECTOR targetPos;

	float destroyTimer;
	bool isAttack;
	bool isThrow;
	Player* pl;
};
