#include "Witch_Enemy.h"
#include "PlaySceneUI.h"
#include "TargetManager.h"
#include "ScoreManager.h"
#include "Camera.h"
#include "GrowRock.h"
#include "FlyingRock.h"

Witch_Enemy::Witch_Enemy()
{
	hModel = MV1LoadModel("data/Character/Witch/Witch.mv1");
	position = VGet(GetRand(2000), 100, 1000);
	rotation = VGet(0, 0, 0);
	HP_MAX = 200;
	HP = HP_MAX;
	attack = 10;
	defense = 2;
	isAttack = false;
	isDeath = false;
	jumpA = false;
	jumpPower = 0;
	if (TargetManager::IsEnemyIntroPlaying()) {
		isEnemyIntroPlaying = true;
		TargetManager::SetIsEnemyIntroPlaying(false);
	}
	else {
		isEnemyIntroPlaying = false;
	}

	std::string folder = ("data/Character/Witch/");
	std::string filename[] = {
		"Witch_Anim_Idle",
		"Witch_Anim_Walk",
		"Witch_Anim_Attack1",
		"Witch_Anim_Attack2",
		"Witch_Anim_Attack3",
		"Witch_Anim_Damage",
		"Witch_Anim_Death",
		"Witch_Anim_Appearance",
	};
	for (int i = 0; i < Max; i++) {
		info[i].hAnim = MV1LoadModel((folder + filename[i] + ".mv1").c_str());
		//タイムライン
		CsvReader* csv = new CsvReader(folder + filename[i] + ".csv");
		for (int line = 0; line < csv->GetLines(); line++) {
			TimeInfo ti;
			ti.time = csv->GetFloat(line, 0);
			ti.command = csv->GetString(line, 1);
			ti.filename = csv->GetString(line, 2);
			info[i].timeline.push_back(ti);
		}
		delete csv;
	}
	animation = new Animation();
	animation->SetModel(hModel);
	animation->SetFrameSpeed(0.75f);
	setAnimation(A_Idle, true);

	pl = ObjectManager::FindGameObject<Player>();
	ShadowInit();
	PlaySceneUI* ui = ObjectManager::FindGameObject<PlaySceneUI>();
	if (ui != nullptr) {
		ui->SetEnemyHp(HP_MAX, 0, HP, "WITCH");
	}
	isNextMove = true;

	//AI（ビヘイビアツリー）
	root = std::make_unique<Selector>();
	//攻撃1
	attack1Seq = std::make_unique<Sequence>();
	attack1Seq->AddChild(make_unique<WitchAttack1Check>(this));
	attack1Seq->AddChild(make_unique<WitchAttack1>(this));
	//攻撃2
	attack2Seq = std::make_unique<Sequence>();
	attack2Seq->AddChild(make_unique<WitchAttack2Check>(this));
	attack2Seq->AddChild(make_unique<WitchAttack2>(this));
	//攻撃3
	attack3Seq = std::make_unique<Sequence>();
	attack3Seq->AddChild(make_unique<WitchAttack3Check>(this));
	attack3Seq->AddChild(make_unique<WitchAttack3>(this));
	//歩き
	moveSeq = std::make_unique<Sequence>();
	moveSeq->AddChild(make_unique<WitchMoveCheck>(this));
	moveSeq->AddChild(make_unique<WitchMove>(this));
	//死亡
	deathSeq = std::make_unique<Sequence>();
	deathSeq->AddChild(make_unique<WitchDeathCheck>(this));
	deathSeq->AddChild(make_unique<WitchDeath>(this));
	//何もしない状態
	idleSeq = std::make_unique<Sequence>();
	idleSeq->AddChild(make_unique<WitchIdle>(this));

	//判定する順番に並べる
	root->AddChild(move(deathSeq));
	root->AddChild(move(attack3Seq));
	root->AddChild(move(attack2Seq));
	root->AddChild(move(attack1Seq));
	root->AddChild(move(moveSeq));
	root->AddChild(move(idleSeq));

	//SE
	SE_GrowRock = LoadSoundMem("data/sound/SE/Mutant/SE_JumpAttack.mp3");
	SE_ThrowRock = LoadSoundMem("data/sound/SE/Witch/ThrowRock_SE.mp3");
	SE_Voice = LoadSoundMem("data/sound/SE/Witch/Witch_Voice.mp3");
}

Witch_Enemy::~Witch_Enemy()
{
	MV1DeleteModel(hModel);
	for (int i = 0; i < Max; i++) {
		MV1DeleteModel(info[i].hAnim);
	}
	DeleteGraph(shadowImage);
	DeleteSoundMem(SE_GrowRock);
}

void Witch_Enemy::Update()
{
	animation->SetFrameSpeed(0.75f);
	onGround();
	//登場演出
	if (isEnemyIntroPlaying) {
		setAnimation(A_Appearance, false);
		if (cam == nullptr) {
			cam = ObjectManager::FindGameObject<Camera>();
		}
		cam->Event(true);
		// カメラ設定
		VECTOR camPos = position + VGet(0, 200, -500);
		VECTOR target = position + VGet(0, 300, 0);
		SetCameraPositionAndTarget_UpVecY(camPos, target);

		if (animation->IsEnd()) {
			isEnemyIntroPlaying = false;
			cam->Event(false);
			ScoreManager::TimeCountStart();
		}
		playAnimation();
		return;
	}
	else {
		root->Tick();
	}
	playAnimation();
	LookPlayer();

	//敵の体にめり込まないようにする
	for (Enemy* e : TargetManager::getEnemy()) {
		float enemyDistance = VSize(e->Position() - position);
		if (enemyDistance < 100 && e != this) {
			position += VNorm(position - e->Position()) * (100 - enemyDistance);
		}
	}
}

void Witch_Enemy::Draw()
{
	MV1SetPosition(hModel, position);
	MV1SetRotationXYZ(hModel, rotation);
	MV1SetScale(hModel, VGet(1.5f, 1.5f, 1.5f));
	MV1DrawModel(hModel);
	ShadowDraw();
}

void Witch_Enemy::Attack()
{
}

/// <summary>
/// ダメージを受ける
/// </summary>
/// <param name="damage">ダメージ数</param>
void Witch_Enemy::TakeDamage(int damage)
{
	PlaySceneUI* ui = ObjectManager::FindGameObject<PlaySceneUI>();
	HP -= (damage - defense);
	if (HP <= 0) {
		HP = 0;
	}
	ui->SetEnemyHp(HP_MAX, 0, HP, "WITCH");
}

/// <summary>
/// 地面に立つ
/// </summary>
void Witch_Enemy::onGround()
{
	//地面に立たせる
	Stage* pStage = ObjectManager::FindGameObject<Stage>();
	if (jumpPower <= 0) {
		if (pStage != nullptr)
		{
			VECTOR hit;
			if (pStage->CollLine(position + VGet(0, 100, 0), position + VGet(0, -100, 0), &hit))
			{
				if (!jumpA) {
					position = hit;
					jumpPower = 0;
					animation->SetFrameSpeed(0.75f);
				}
				else {
					jumpA = false;
				}
			}
			else {
				position.y -= 9.8f * Time::DeltaTime();	//重力
			}
		}
	}
	VECTOR sHit;
	if (pStage->CollLine(position + VGet(0, 10000, 0), position + VGet(0, -10000, 0), &sHit))
	{
		shadowPos = sHit;
	}
	else {
		//jumpPower -= 9.8f * Time::DeltaTime();
	}

	position.y += jumpPower * Time::DeltaTime();
}

/// <summary>
/// 影の初期化
/// </summary>
void Witch_Enemy::ShadowInit()
{
	//影
	shadowPos = position;
	shadowImage = LoadGraph("data/Character/Shadow.png");
	Vertex[0].dif = GetColorU8(255, 255, 255, 255);
	Vertex[0].spc = GetColorU8(0, 0, 0, 0);
	Vertex[0].u = 0.0f;
	Vertex[0].v = 0.0f;
	Vertex[0].su = 0.0f;
	Vertex[0].sv = 0.0f;

	Vertex[1].dif = GetColorU8(255, 255, 255, 255);
	Vertex[1].spc = GetColorU8(0, 0, 0, 0);
	Vertex[1].u = 1.0f;
	Vertex[1].v = 0.0f;
	Vertex[1].su = 0.0f;
	Vertex[1].sv = 0.0f;

	Vertex[2].dif = GetColorU8(255, 255, 255, 255);
	Vertex[2].spc = GetColorU8(0, 0, 0, 0);
	Vertex[2].u = 0.0f;
	Vertex[2].v = 1.0f;
	Vertex[2].su = 0.0f;
	Vertex[2].sv = 0.0f;

	Vertex[3].dif = GetColorU8(255, 255, 255, 255);
	Vertex[3].spc = GetColorU8(0, 0, 0, 0);
	Vertex[3].u = 1.0f;
	Vertex[3].v = 1.0f;
	Vertex[3].su = 0.0f;
	Vertex[3].sv = 0.0f;

	Vertex[4] = Vertex[2];
	Vertex[5] = Vertex[1];

	shadowAngle.x = 0.0f;
	shadowAngle.y = 0.0f;
	shadowAngle.z = 0.0f;
}

/// <summary>
/// 影の表示
/// </summary>
void Witch_Enemy::ShadowDraw()
{
	//影
	Vertex[0].pos = shadowPos + VGet(-100.0f, 10.0f, 100.0f);
	Vertex[1].pos = shadowPos + VGet(100.0f, 10.0f, 100.0f);
	Vertex[2].pos = shadowPos + VGet(-100.0f, 10.0f, -100.0f);
	Vertex[3].pos = shadowPos + VGet(100.0f, 10.0f, -100.0f);
	Vertex[4].pos = Vertex[2].pos;
	Vertex[5].pos = Vertex[1].pos;
	DrawPolygon3D(Vertex, 2, shadowImage, TRUE);
}

/// <summary>
/// アニメーションをセットする
/// </summary>
/// <param name="id"></param>
/// <param name="loop"></param>
void Witch_Enemy::setAnimation(ANIM_ID id, bool loop)
{
	animation->Play(info[id].hAnim, loop);
	animID = id;
}

/// <summary>
/// アニメーションの更新
/// タイムライン確認
/// </summary>
void Witch_Enemy::playAnimation()
{
	float prevFrame = animation->GetCurrentFrame();
	animation->Update();
	float curFrame = animation->GetCurrentFrame();

	std::string folder = "data/sound/SE/";
	for (TimeInfo t : info[animID].timeline) {
		if (t.time > prevFrame && curFrame >= t.time) {
			if (t.command == "SE") {
				PlaySound(
					"data/sound/SE/pladin/walk.mp3",
					DX_PLAYTYPE_BACK);
			}
			if (t.command == "AttackStart") {
				isAttack = true;
			}
			if (t.command == "AttackEnd") {
				isAttack = false;
			}
			if (t.command == "JumpStart") {
				jumpA = true;
				jumpPower = 300.0f;
			}
			if (t.command == "JumpEnd") {
				jumpPower = -500.0f;
			}
			if (t.command == "JumpAttackEnd") {
				jumpA = false;
			}
			if (t.command == "RockAttack") {
			}
			if (t.command == "StopAnimation") {
				animation->SetFrameSpeed(0);
			}
			if (t.command == "StartAnimation") {
				animation->SetFrameSpeed(0.75);
			}
			if (t.command == "GrowRock") {	//下から生えてくる岩
				new GrowRock(pl->Position());
				PlaySoundMem(SE_GrowRock, DX_PLAYTYPE_BACK, true);
			}
			if (t.command == "CreateThrowRock") {	//投げる岩を生成
				tRock = new ThrowRock(VGet(position.x, position.y + 400, position.z));

			}
			if (t.command == "ThrowRock") {	//投げる
				if (tRock != nullptr) {
					tRock->SetTarget(pl->Position());
					PlaySoundMem(SE_ThrowRock, DX_PLAYTYPE_BACK, true);
				}
			}
			if (t.command == "FlyingRockL") {
				VECTOR p = VGet(position.x - 10, position.y, position.z + 50);
				new FlyingRock(p, rotation.y - (DX_PI_F / 4), position);
			}
			if (t.command == "FlyingRock") {
				VECTOR p = VGet(position.x, position.y, position.z + 50);
				new FlyingRock(p, rotation.y, position);
			}
			if (t.command == "FlyingRockR") {
				VECTOR p = VGet(position.x + 10, position.y, position.z + 50);
				new FlyingRock(p, rotation.y + (DX_PI_F / 4), position);
			}
			if (t.command == "Voice") {
				PlaySoundMem(SE_Voice, DX_PLAYTYPE_BACK, true);
			}
		}
	}
}

/// <summary>
/// プレイヤーのほうを向く
/// </summary>
void Witch_Enemy::LookPlayer()
{
	VECTOR pPos = pl->Position();
	//内積
	//TODO:敵がプルプルするから修正する
	VECTOR forPlayer = pPos - position;
	VECTOR right = VGet(1, 0, 0) * MGetRotY(rotation.y);
	float dot = VDot(right, forPlayer);
	if (dot > 10) {
		rotation.y -= DegToRad(3.0f);
	}
	else if (dot < 10) {
		rotation.y += DegToRad(3.0f);
	}
}

/// <summary>
/// 何もしない状態
/// </summary>
WitchIdle::WitchIdle(Witch_Enemy* witch)
{
	wi = witch;
}

NodeStatus WitchIdle::Tick()
{
	wi->setAnimation(wi->A_Idle);
	return NodeStatus::Success();
}

/// <summary>
/// 移動状態
/// </summary>
/// <param name="witch"></param>
WitchMoveCheck::WitchMoveCheck(Witch_Enemy* witch)
{
	wi = witch;
}

NodeStatus WitchMoveCheck::Tick()
{
	if (200 < VSize(wi->pl->Position() - wi->Position()) < 1500) {
		return NodeStatus::Success;
	}
	return NodeStatus::Failure;
}

WitchMove::WitchMove(Witch_Enemy* witch)
{
	wi = witch;
}

NodeStatus WitchMove::Tick()
{
	wi->setAnimation(wi->A_Walk, true);
	VECTOR pPos = wi->pl->Position();
	wi->position += VNorm(pPos - wi->position) * 2;
	return NodeStatus::Success;
}

/// <summary>
/// 攻撃1状態
/// </summary>
/// <param name="witch"></param>
WitchAttack1Check::WitchAttack1Check(Witch_Enemy* witch)
{
	wi = witch;
	isAttacking = false;
}

NodeStatus WitchAttack1Check::Tick()
{
	if (isAttacking) {
		if (wi->animation->IsEnd()) {
			isAttacking = false;
			wi->isNextMove = true;
			return NodeStatus::Failure;
		}
		return NodeStatus::Success;
	}

	if (wi->isNextMove) {
		if (VSize(wi->pl->Position() - wi->Position()) < 450) {
			isAttacking = true;
			wi->isNextMove = false;
			return NodeStatus::Success;
		}
	}

	return NodeStatus::Failure;
}

WitchAttack1::WitchAttack1(Witch_Enemy* witch)
{
	wi = witch;
}

NodeStatus WitchAttack1::Tick()
{
	wi->setAnimation(wi->A_Attack1, false);
	if (wi->animation->IsEnd()) {
		wi->setAnimation(wi->A_Idle, false);
		return NodeStatus::Success;
	}
	return NodeStatus::Running;
}

/// <summary>
/// 攻撃2状態
/// </summary>
/// <param name="witch"></param>
WitchAttack2Check::WitchAttack2Check(Witch_Enemy* witch)
{
	wi = witch;
	isAttacking = false;
}

NodeStatus WitchAttack2Check::Tick()
{
	if (isAttacking) {
		if (wi->animation->IsEnd()) {
			isAttacking = false;
			wi->isNextMove = true;
			return NodeStatus::Failure;
		}
		return NodeStatus::Success;
	}

	if (wi->isNextMove) {
		if (VSize(wi->pl->Position() - wi->Position()) < 700) {
			isAttacking = true;
			wi->isNextMove = false;
			return NodeStatus::Success;
		}
	}

	return NodeStatus::Failure;
}

WitchAttack2::WitchAttack2(Witch_Enemy* witch)
{
	wi = witch;
}

NodeStatus WitchAttack2::Tick()
{
	wi->setAnimation(wi->A_Attack2, false);
	if (wi->animation->IsEnd()) {
		wi->setAnimation(wi->A_Idle, false);
		return NodeStatus::Success;
	}
	return NodeStatus::Running;
}

/// <summary>
/// 攻撃3状態
/// </summary>
/// <param name="witch"></param>
WitchAttack3Check::WitchAttack3Check(Witch_Enemy* witch)
{
	wi = witch;
	isAttacking = false;
}

NodeStatus WitchAttack3Check::Tick()
{
	if (isAttacking) {
		if (wi->animation->IsEnd()) {
			isAttacking = false;
			wi->isNextMove = true;
			return NodeStatus::Failure;
		}
		return NodeStatus::Success;
	}

	if (wi->isNextMove) {
		if (VSize(wi->pl->Position() - wi->Position()) <= 900) {
			isAttacking = true;
			wi->isNextMove = false;
			return NodeStatus::Success;
		}
	}

	return NodeStatus::Failure;
}

WitchAttack3::WitchAttack3(Witch_Enemy* witch)
{
	wi = witch;
}

NodeStatus WitchAttack3::Tick()
{
	wi->setAnimation(wi->A_Attack3, false);
	if (wi->animation->IsEnd()) {
		wi->setAnimation(wi->A_Idle, false);
		return NodeStatus::Success;
	}
	return NodeStatus::Running;
}

/// <summary>
/// 死亡
/// </summary>
/// <param name="witch"></param>
WitchDeathCheck::WitchDeathCheck(Witch_Enemy* witch)
{
	wi = witch;
}

NodeStatus WitchDeathCheck::Tick()
{
	if (wi->HP <= 0) {
		return NodeStatus::Success;
	}
	return NodeStatus::Failure;
}

WitchDeath::WitchDeath(Witch_Enemy* witch)
{
	wi = witch;
}

NodeStatus WitchDeath::Tick()
{
	wi->setAnimation(wi->A_Death, false);
	if (wi->animation->IsEnd()) {
		wi->isDeath = true;
	}
	return NodeStatus::Success;
}
