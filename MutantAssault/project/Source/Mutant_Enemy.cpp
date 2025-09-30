#include "Mutant_Enemy.h"
#include "MutantRock.h"
#include "PlaySceneUI.h"
#include "TargetManager.h"
#include "ScoreManager.h"

Mutant_Enemy::Mutant_Enemy()
{
	SetDrawOrder(10);
	hModel = MV1LoadModel("data/Character/Mutant/Mutant.mv1");
	position = VGet(GetRand(2000), 200, 1000);
	rotation = VGet(0, 0, 0);
	HP_MAX = 200;
	HP = HP_MAX;
	attack = 15;
	defense = 2;
	isAttack = false;
	isDeath = false;
	attackMove = false;
	jumpA = false;
	jumpPower = 0;
	if (TargetManager::IsEnemyIntroPlaying()) {
		isEnemyIntroPlaying = true;
		TargetManager::SetIsEnemyIntroPlaying(false);
	}
	else {
		isEnemyIntroPlaying = false;
	}
	SE_Roar = LoadSoundMem("data/sound/SE/Mutant/SE_Roar.mp3");
	SE_JumpAttack = LoadSoundMem("data/sound/SE/Mutant/SE_JumpAttack.mp3");

	std::string folder = ("data/Character/Mutant/");
	std::string filename[] = {
		"Mutant_Anim_Stop",
		"Mutant_Anim_Walk",
		"Mutant_Anim_Attack",
		"Mutant_Anim_Attack2",
		"Mutant_Anim_Attack3",
		"Mutant_Anim_JumpAttack",
		"Mutant_Anim_Death",
		"Mutant_Anim_Down",
		"Mutant_Anim_StandUp",
		"Mutant_Anim_Roaning",
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
	setAnimation(A_Stop, true);

	pl = ObjectManager::FindGameObject<Player>();
	ShadowInit();
	PlaySceneUI* ui = ObjectManager::FindGameObject<PlaySceneUI>();
	if (ui != nullptr) {
		ui->SetEnemyHp(HP_MAX, 0, HP, "MUTANT");
	}

	//AI（ビヘイビアツリー）
	root = std::make_unique<Selector>();
	//攻撃1
	attack1Seq = std::make_unique<Sequence>();
	attack1Seq->AddChild(make_unique<Attack1Check>(this));
	attack1Seq->AddChild(make_unique<Attack1>(this));
	//攻撃2
	attack2Seq = std::make_unique<Sequence>();
	attack2Seq->AddChild(make_unique<Attack2Check>(this));
	attack2Seq->AddChild(make_unique<Attack2>(this));
	//ジャンプ攻撃
	jumpAttackSeq = std::make_unique<Sequence>();
	jumpAttackSeq->AddChild(make_unique<JumpAttackCheck>(this));
	jumpAttackSeq->AddChild(make_unique<JumpAttack>(this));
	//歩き
	moveSeq = std::make_unique<Sequence>();
	moveSeq->AddChild(make_unique<MoveCheck>(this));
	moveSeq->AddChild(make_unique<Move>(this));
	//死亡
	deathSeq = std::make_unique<Sequence>();
	deathSeq->AddChild(make_unique<DeathCheck>(this));
	deathSeq->AddChild(make_unique<Death>(this));
	//何もしない状態
	idleSeq = std::make_unique<Sequence>();
	idleSeq->AddChild(make_unique<Idle>(this));

	//判定する順番に並べる
	root->AddChild(move(deathSeq));
	root->AddChild(move(jumpAttackSeq));
	root->AddChild(move(attack2Seq));
	root->AddChild(move(attack1Seq));
	root->AddChild(move(moveSeq));
	root->AddChild(move(idleSeq));
}

Mutant_Enemy::~Mutant_Enemy()
{
	MV1DeleteModel(hModel);
	for (int i = 0; i < Max; i++) {
		MV1DeleteModel(info[i].hAnim);
	}
	DeleteGraph(shadowImage);
	DeleteSoundMem(SE_Roar);
	DeleteSoundMem(SE_JumpAttack);
}

void Mutant_Enemy::Update()
{
	animation->SetFrameSpeed(0.75f);
	//登場演出
	if (isEnemyIntroPlaying) {
		setAnimation(A_Roaning, false);
		Camera* cam = ObjectManager::FindGameObject<Camera>();
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
	}
	else {
		root->Tick();
	}
	playAnimation();
	Attack();
	onGround();
	//敵の体にめり込まないようにする
	for (Enemy* e : TargetManager::getEnemy()) {
		float enemyDistance = VSize(e->Position() - position);
		if (enemyDistance < 100 && e != this) {
			position += VNorm(position - e->Position()) * (100 - enemyDistance);
		}
	}
}

void Mutant_Enemy::Draw()
{
	MV1SetPosition(hModel, position);
	MV1SetRotationXYZ(hModel, rotation);
	MV1SetScale(hModel, VGet(2.0f, 2.0f, 2.0f));
	MV1DrawModel(hModel);

	ShadowDraw();
}

/// <summary>
/// 攻撃がプレイヤーに当たったかチェック
/// </summary>
void Mutant_Enemy::Attack()
{
	//攻撃判定の座標更新
	MATRIX mWeapon = MV1GetFrameLocalWorldMatrix(hModel, 13);
	oldArmPos = newArmPos;
	newArmPos = VGet(0, 0, 0) * mWeapon;

	//プレイヤーに攻撃が当たったらプレイヤーにダメージを与える
	if (!isAttack) { return; }
	if (pl != nullptr) {
		if (pl->enemyAttack(oldArmPos, newArmPos, 100.0f)) {
			isAttack = false;
			pl->addDamage(attack,position);
		}
	}
	else {
		pl = ObjectManager::FindGameObject<Player>();
	}
}

/// <summary>
/// ダメージを受ける
/// </summary>
/// <param name="damage">ダメージ数</param>
void Mutant_Enemy::TakeDamage(int damage)
{
	PlaySceneUI* ui = ObjectManager::FindGameObject<PlaySceneUI>();
	HP -= (damage - defense);
	if (HP <= 0) {
		HP = 0;
	}
	ui->SetEnemyHp(HP_MAX, 0, HP, "MUTANT");
}

/// <summary>
/// 地面に立つ
/// </summary>
void Mutant_Enemy::onGround()
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
void Mutant_Enemy::ShadowInit()
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
void Mutant_Enemy::ShadowDraw()
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
void Mutant_Enemy::setAnimation(ANIM_ID id, bool loop)
{
	animation->Play(info[id].hAnim, loop);
	animID = id;
}

/// <summary>
/// アニメーションの更新
/// タイムライン確認
/// </summary>
void Mutant_Enemy::playAnimation()
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
			if (t.command == "SE_Roar") {
				PlaySoundMem(SE_Roar, DX_PLAYTYPE_BACK, true);
			}
			if (t.command == "SE_JumpAttack") {
				PlaySoundMem(SE_JumpAttack, DX_PLAYTYPE_BACK, true);
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
				new MutantRock(VGet(position.x, position.y, position.z + 230));
				new MutantRock(VGet(position.x, position.y, position.z - 230));
				new MutantRock(VGet(position.x + 230, position.y, position.z));
				new MutantRock(VGet(position.x - 230, position.y, position.z));
				new MutantRock(VGet(position.x - 230 / 2, position.y, position.z + 230 / 2));
				new MutantRock(VGet(position.x - 230 / 2, position.y, position.z - 230 / 2));
				new MutantRock(VGet(position.x + 230 / 2, position.y, position.z + 230 / 2));
				new MutantRock(VGet(position.x + 230 / 2, position.y, position.z - 230 / 2));
			}
			if (t.command == "StopAnimation") {
				animation->SetFrameSpeed(0);
			}
			if (t.command == "StartAnimation") {
				animation->SetFrameSpeed(0.75);
			}
			if (t.command == "AttackMoveStart") {
				attackMove = true;
			}
			if (t.command == "AttackMoveEnd") {
				attackMove = false;
			}
		}
	}
}

/// <summary>
/// プレイヤーのほうを向く
/// </summary>
void Mutant_Enemy::LookPlayer()
{
	VECTOR pPos = pl->Position();
	//内積
	//TODO:敵がプルプルするから修正する
	VECTOR forPlayer = pPos - position;
	VECTOR right = VGet(1, 0, 0) * MGetRotY(rotation.y);
	float dot = VDot(right, forPlayer);
	if (dot > 0) {
		rotation.y -= DegToRad(3.0f);
	}
	if (dot < 0) {
		rotation.y += DegToRad(3.0f);
	}
}

/// <summary>
/// 何もしない状態
/// </summary>
Idle::Idle(Mutant_Enemy* mutant)
{
	mu = mutant;
}

NodeStatus Idle::Tick()
{
	mu->setAnimation(mu->A_Stop, true);
	return NodeStatus::Success;
}

/// <summary>
/// 移動
/// </summary>
MoveCheck::MoveCheck(Mutant_Enemy* mutant)
{
	mu = mutant;
}

NodeStatus MoveCheck::Tick() 
{
	if (200 < VSize(mu->pl->Position() - mu->position) < 1500) {
		return NodeStatus::Success;
	}
	return NodeStatus::Failure;
}

Move::Move(Mutant_Enemy* mutant)
{
	mu = mutant;
}

NodeStatus Move::Tick()
{
	mu->setAnimation(mu->A_Walk, true);
	VECTOR pPos = mu->pl->Position();
	mu->position += VNorm(pPos - mu->position) * 2;
	mu->LookPlayer();
	return NodeStatus::Success;
}

/// <summary>
/// 攻撃1
/// </summary>
Attack1Check::Attack1Check(Mutant_Enemy* mutant)
{
	mu = mutant;
	isAttacking = false;
}

NodeStatus Attack1Check::Tick()
{
	if (VSize(mu->pl->Position() - mu->position) <= 450) {
		isAttacking = true;
		return NodeStatus::Success;
	}
	if (mu->animation->IsEnd()) {
		isAttacking = false;
	}
	if (isAttacking) {
		return NodeStatus::Success;
	}
	return NodeStatus::Failure;
}

Attack1::Attack1(Mutant_Enemy* mutant)
{
	mu = mutant;
}

NodeStatus Attack1::Tick()
{
	if (mu->isAttack) {
		mu->LookPlayer();
	}
	mu->setAnimation(mu->A_Attack, false);
	if (mu->animation->IsEnd()) {
		mu->setAnimation(mu->A_Stop, false);
		return NodeStatus::Success;
	}
	return NodeStatus::Running;
}

/// <summary>
/// 攻撃2
/// </summary>
Attack2Check::Attack2Check(Mutant_Enemy* mutant)
{
	mu = mutant;
	isAttacking = false;
}

NodeStatus Attack2Check::Tick() 
{
	if (450 < VSize(mu->pl->Position() - mu->position) &&
		VSize(mu->pl->Position() - mu->position) <= 700) {
		isAttacking = true;
		return NodeStatus::Success;
	}
	if (mu->animation->IsEnd()) {
		isAttacking = false;
	}
	if (isAttacking) {
		return NodeStatus::Success;
	}
	return NodeStatus::Failure;
}

Attack2::Attack2(Mutant_Enemy* mutant)
{
	mu = mutant;
}

NodeStatus Attack2::Tick()
{
	if (mu->attackMove) {
		VECTOR pPos = mu->pl->Position();
		mu->position += VNorm(pPos - mu->position) * 200 * Time::DeltaTime();
		mu->LookPlayer();
	}
	mu->setAnimation(mu->A_Attack2, false);
	if (mu->animation->IsEnd()) {
		mu->setAnimation(mu->A_Stop, false);
		return NodeStatus::Success;
	}
	return NodeStatus::Running;
}

/// <summary>
/// ジャンプ攻撃
/// </summary>
JumpAttackCheck::JumpAttackCheck(Mutant_Enemy* mutant)
{
	mu = mutant;
	isAttacking = false;
}

NodeStatus JumpAttackCheck::Tick()
{
	if (700 < VSize(mu->pl->Position() - mu->position) &&
		VSize(mu->pl->Position() - mu->position) <= 900) {
		isAttacking = true;
		return NodeStatus::Success;
	}
	if (mu->animation->IsEnd()) {
		isAttacking = false;
	}
	if (isAttacking) {
		return NodeStatus::Success;
	}
	return NodeStatus::Failure;
}

JumpAttack::JumpAttack(Mutant_Enemy* mutant)
{
	mu = mutant;
}

NodeStatus JumpAttack::Tick()
{
	if (mu->jumpA) {
		VECTOR pPos = mu->pl->Position();
		mu->position += VNorm(pPos - mu->position) * 500 * Time::DeltaTime();
		mu->LookPlayer();
	}

	mu->setAnimation(mu->A_JumpAttack, false);
	if (mu->animation->IsEnd()) {
		mu->setAnimation(mu->A_Stop, false);
		return NodeStatus::Success;
	}
	return NodeStatus::Running;
}

/// <summary>
/// 死亡
/// </summary>
DeathCheck::DeathCheck(Mutant_Enemy* mutant)
{
	mu = mutant;
}

NodeStatus DeathCheck::Tick()
{
	if (mu->HP <= 0) {
		return NodeStatus::Success;
	}
	return NodeStatus::Failure;
}

Death::Death(Mutant_Enemy* mutant)
{
	mu = mutant;
}

NodeStatus Death::Tick()
{
	mu->setAnimation(mu->A_Death, false);
	if (mu->animation->IsEnd()) {
		mu->isDeath = true;
	}
	return NodeStatus::Success;
}
