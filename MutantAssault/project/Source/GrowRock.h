#pragma once
#include "../Library/GameObject.h"

/// <summary>
/// �������g�������琶���Ă����
/// </summary>
class GrowRock : public GameObject {
public:
	GrowRock(VECTOR pos);
	~GrowRock();
	void Update() override;
	void Draw() override;
private:
	int hRock;
	VECTOR position;
	VECTOR targetPos;

	float destroyTimer;
	bool isAttack;
};
