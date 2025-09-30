#pragma once
#include "../Library/GameObject.h"

/// <summary>
/// mutant‚ªƒWƒƒƒ“ƒvUŒ‚‚·‚é‚Æ‚«‚Éo‚éŠâ
/// </summary>
class MutantRock : public GameObject {
public:
	MutantRock(VECTOR pos);
	~MutantRock();
	void Update() override;
	void Draw() override;
private:
	int hRock;
	VECTOR position;
	VECTOR tergetPos;

	float destroyTimer;
	bool isAttack;
};