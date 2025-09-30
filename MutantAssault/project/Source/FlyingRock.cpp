#include "FlyingRock.h"
#include "../Library/Time.h"
#include "Player.h"

FlyingRock::FlyingRock(VECTOR pos , float rotationY ,VECTOR parPos)
{
	position = VGet(pos.x, pos.y+150, pos.z);
	rotation = VGet(0, rotationY, 0);
	VECTOR parP = parPos;
	parP.y = parP.y + 150;
	dir = (parP - position)*MGetRotY(rotationY);
	hRock = MV1LoadModel("data/Stage/Stage_Obj020.mv1");
	MV1SetFrameUserLocalMatrix(hRock, 0, MGetRotX(DX_PI_F / 2));
	destroyTimer = 2.0f;
	isAttack = true;
	pl = ObjectManager::FindGameObject<Player>();
}

FlyingRock::~FlyingRock()
{
	MV1DeleteModel(hRock);
}

void FlyingRock::Update()
{
	position += VNorm(dir) * Time::DeltaTime() * 800;

	if (destroyTimer > 0) {
		destroyTimer -= Time::DeltaTime();
	}
	else {
		DestroyMe();
	}

	VECTOR upPos = VGet(position.x, position.y + 300, position.z);
	VECTOR downPos = position;
	if (pl->enemyAttack(upPos, downPos, 50) && isAttack) {
		isAttack = false;
		pl->addDamage(15, position);
	}
}

void FlyingRock::Draw()
{
	float scale = 0.8;
	MV1SetScale(hRock, VGet(scale , scale, scale));
	MV1SetPosition(hRock, position);
	MV1SetRotationXYZ(hRock, rotation);
	MV1DrawModel(hRock);
}
