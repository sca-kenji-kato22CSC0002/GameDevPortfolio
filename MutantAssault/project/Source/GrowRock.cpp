#include "GrowRock.h"
#include "../Library/Time.h"
#include "Player.h"

GrowRock::GrowRock(VECTOR pos)
{
	targetPos = pos;
	position = VGet(pos.x, pos.y - 200, pos.z);
	hRock = MV1LoadModel("data/Stage/Stage_Obj020.mv1");
	MV1SetFrameUserLocalMatrix(hRock, 0, MGetRotX(-DX_PI_F));
	destroyTimer = 2.0f;
	isAttack = true;

}

GrowRock::~GrowRock()
{
	MV1DeleteModel(hRock);
}

void GrowRock::Update()
{
	if (VSize(position - targetPos) > 100)
		position += VNorm(targetPos - position) * Time::DeltaTime() * 800;
	if (destroyTimer > 0) {
		destroyTimer -= Time::DeltaTime();
	}
	else {
		DestroyMe();
	}

	Player* pl = ObjectManager::FindGameObject<Player>();
	VECTOR upPos = VGet(position.x, position.y + 300, position.z);
	VECTOR downPos = position;
	if (pl->enemyAttack(upPos, downPos, 50) && isAttack) {
		isAttack = false;
		pl->addDamage(30, position);
	}
}

void GrowRock::Draw()
{
	float scale = 1.4;
	MV1SetScale(hRock, VGet(scale*3, scale, scale*3));
	MV1SetPosition(hRock, position);
	MV1DrawModel(hRock);
}
