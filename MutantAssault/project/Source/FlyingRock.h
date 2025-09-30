#pragma once
#include "../Library/GameObject.h"
#include "Player.h"

/// <summary>
/// ñÇèóÇ™îÚÇŒÇ∑ä‚
/// </summary>
class FlyingRock : public GameObject {
public:
	FlyingRock(VECTOR pos,float rotationY, VECTOR parPos);
	~FlyingRock();
	void Update() override;
	void Draw() override;
private:
	int hRock;
	VECTOR position;
	VECTOR rotation;
	VECTOR dir;

	float destroyTimer;
	bool isAttack;
	Player* pl;
};
