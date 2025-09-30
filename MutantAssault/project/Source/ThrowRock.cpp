#include "ThrowRock.h"
#include "../Library/Time.h"


ThrowRock::ThrowRock(VECTOR pos)
{
	targetPos = VGet(0, 0, 0);
	position = pos;
	hRock = MV1LoadModel("data/Stage/Stage_Obj004.mv1");
	destroyTimer = 2.0f;
	isAttack = true;
	isThrow = false;
	pl = ObjectManager::FindGameObject<Player>();
}

ThrowRock::~ThrowRock()
{
	MV1DeleteModel(hRock);
}

void ThrowRock::Update()
{
	if (isThrow) {
		position += VNorm(targetPos - position) * 800 * Time::DeltaTime();
		if (destroyTimer > 0) {
			destroyTimer -= Time::DeltaTime();
		}
		else {
			DestroyMe();
		}
	}

	VECTOR upPos = VGet(position.x, position.y + 300, position.z);
	VECTOR downPos = position;
	if (pl->enemyAttack(upPos, downPos, 50) && isAttack) {
		isAttack = false;
		pl->addDamage(25, position);
	}
}

void ThrowRock::Draw()
{
	float scale = 4;
	MV1SetScale(hRock, VGet(scale, scale, scale));
	MV1SetPosition(hRock, position);
	MV1DrawModel(hRock);
}

void ThrowRock::SetTarget(VECTOR tPos)
{
	targetPos = tPos;
	isThrow = true;
}
