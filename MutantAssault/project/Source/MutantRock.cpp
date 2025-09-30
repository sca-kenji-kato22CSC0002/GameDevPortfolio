#include "MutantRock.h"
#include "../Library/Time.h"
#include "Player.h"

MutantRock::MutantRock(VECTOR pos)
{
	tergetPos = pos;
	position = VGet(pos.x,pos.y-200,pos.z);
	hRock = MV1LoadModel("data/Stage/Stage_Obj020.mv1");
	MV1SetFrameUserLocalMatrix(hRock, 0, MGetRotX(-DX_PI_F));
	destroyTimer = 2.0f;
	isAttack = true;

}

MutantRock::~MutantRock()
{
	MV1DeleteModel(hRock);
}

void MutantRock::Update()
{
	if(VSize(position - tergetPos) > 100)
	position += VNorm(tergetPos - position) * Time::DeltaTime() * 800;
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
		pl->addDamage(10,position);
	}

}

void MutantRock::Draw()
{
	float scale = 1.4;
	MV1SetScale(hRock, VGet(scale,scale,scale));
	MV1SetPosition(hRock, position);
	MV1DrawModel(hRock);
}
