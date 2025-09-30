#include "Player.h"
#include "../Library/Time.h"
#include "../Library/csvReader.h"
#include "../Library/SceneManager.h"
#include "Stage.h"
#include "Camera.h"
#include "time.h"
#include "Locus.h"
#include "ScoreManager.h"
#include "Enemy.h"
#include "TargetManager.h"

Locus* locus;	
Player::Player() : stateMachine(std::make_unique<StateMachine>())
{
	auto idle = std::make_shared<PlayerIdle>();
	auto move = std::make_shared<PlayerMove>();
	auto avoid = std::make_shared<PlayerAvoid>();
	auto guard = std::make_shared<PlayerGuard>();
	auto attack1 = std::make_shared<PlayerAttack1>();
	auto attack2 = std::make_shared<PlayerAttack2>();
	auto attack3 = std::make_shared<PlayerAttack3>();
	auto attack4 = std::make_shared<PlayerAttack4>();
	auto chargeAttack = std::make_shared<PlayerChargeAttack>();
	auto damage = std::make_shared<PlayerDamage>();
	auto counterP = std::make_shared<PlayerCounterPose>();
	auto counterA = std::make_shared<PlayerCounterAttack>();
	stateMachine->Register(stateName_Idle, idle);
	stateMachine->Register(stateName_Move, move);
	stateMachine->Register(stateName_Avoid, avoid);
	stateMachine->Register(stateName_Guard, guard);
	stateMachine->Register(stateName_Attack1, attack1);
	stateMachine->Register(stateName_Attack2, attack2);
	stateMachine->Register(stateName_Attack3, attack3);
	stateMachine->Register(stateName_Attack4, attack4);
	stateMachine->Register(stateName_ChargeAttack, chargeAttack);
	stateMachine->Register(stateName_Damage,damage);
	stateMachine->Register(stateName_CounterPose,counterP);
	stateMachine->Register(stateName_CounterAttack,counterA);

	stateMachine->SetStartState(stateName_Idle);

	hModel = MV1LoadModel("data/Character/Paladin/Paladin.mv1");
	int root = MV1SearchFrame(hModel, "mixamorig:RightToe_End");
	MV1SetFrameUserLocalMatrix(hModel, 0, MGetRotY(-DX_PI_F));

	std::string folder = ("data/Character/Paladin/");
	std::string filename[] = {
		"Paladin_Anim_Stop",
		"Paladin_Anim_Run",
		"Paladin_Anim_Walk",
		"Paladin_Anim_WalkRight",
		"Paladin_Anim_WalkLeft",
		"Paladin_Anim_Turn",
		"Paladin_Anim_Attack1",
		"Paladin_Anim_Attack2",
		"Paladin_Anim_Attack3",
		"Paladin_Anim_Attack4",
		"Paladin_Anim_CAttack",
		"Paladin_Anim_GuardStart",
		"Paladin_Anim_GuardKeep",
		"Paladin_Anim_GuardEnd",
		"Paladin_Anim_Parry",
		"Paladin_Anim_AttackTest",
		"Paladin_Anim_Roll",
		"Paladin_Anim_newAttack1",
		"Paladin_Anim_newAttack2",
		"Paladin_Anim_newAttack3",
		"Paladin_Anim_Damage",
		"Paladin_Anim_Death",
		"Paladin_Anim_CounterP",
		"Paladin_Anim_CounterA",
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
	setAnimation(A_STOP, true);
	animation->SetFrameSpeed(1);

	position = VGet(0, 0, -1000);
	rotation = VGet(0, 0, 0);

	lookTarget = VGet(0, 100, 0);

	HP = HP_MAX;
	Stamina = Stamina_MAX;

	isJustGuard = false;
	isAvoidance = false;
	isParry = false;
	coolTime_Stamina = 0;
	isNextAction = true;
	attackPower = attackPowerDefault;

	//サウンド
	walkSE_L = LoadSoundMem("data/sound/SE/Paladin/SE_Walk.mp3");
	walkSE_R = LoadSoundMem("data/sound/SE/Paladin/SE_Walk.mp3");
	attackSE = LoadSoundMem("data/sound/SE/Paladin/SE_Attack.mp3");
	ChangeVolumeSoundMem(190,walkSE_L);
	ChangeVolumeSoundMem(190,walkSE_R);

	//影
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

	locus = Instantiate<Locus>();
	isLocus = false;

	potionKey = false;
	potionCoolTime = 0.0f;

	justAvoid = false;
	isInvincible = false;
}

Player::~Player()
{
	MV1DeleteModel(hModel);
	for (int i = 0; i < Max; i++) {
		MV1DeleteModel(info[i].hAnim);
	}
	locus->DestroyMe();
}

void Player::Update()
{
	//死亡確認
	if (HP <= 0) {
		setAnimation(A_Death, false);
		playAnimation();
		if (AnimEnd()) {
			SceneManager::ChangeScene("ResultScene");
		}
		return;
	}

	inputPadButton();
	stateMachine->Update();
	playAnimation();
	AtackCheck();
	
	// 地面に立たせる
	Stage* pStage = ObjectManager::FindGameObject<Stage>();
	if (pStage != nullptr)
	{
		VECTOR hit;
		if (pStage->CollLine(position + VGet(0, 500, 0), position + VGet(0, -500, 0), &hit))
		{
			position = hit;
		}
	}

	Stamina += 1;	//	スタミナ回復
	if (Stamina > Stamina_MAX) {
		Stamina = Stamina_MAX;
	}
	lookTarget.y = position.y;	//カメラの計算に使う


	//軌跡
	MATRIX mWeapon = MV1GetFrameLocalWorldMatrix(hModel, 73);
	VECTOR p1 = VGet(0, 0, 0) * mWeapon;
	VECTOR p2 = VGet(-50, -10, -200) * mWeapon;
	locus->Add(p1, p2);

	//敵の体にめり込まないようにする
	for (Enemy* e : TargetManager::getEnemy()) {
		float enemyDistance = VSize(e->Position() - position);
		if (enemyDistance < 100) {
			position += VNorm(position - e->Position()) * (100 - enemyDistance);
			lookTarget += VNorm(position - e->Position()) * (100 - enemyDistance);
		}
	}

	//アイテム使用
	if (CheckHitKey(KEY_INPUT_F) && potionCoolTime <= 0 || crossKey == 0 && potionCoolTime <= 0) {
		if (!potionKey) {
			potionCoolTime = 5.0f;
			HP += 25;
			if (HP > HP_MAX) {
				HP = HP_MAX;
			}
			potionKey = true;
		}
	}
	else {
		potionKey = false;
		if (potionCoolTime > 0) {
			potionCoolTime -= Time::DeltaTime();
		}
	}

	if (justAvoidTimer > 0) {
		justAvoidTimer -= Time::DeltaTime();
	}
	else {
		animation->SetFrameSpeed(1.5f);
	}
}

void Player::Draw()
{
	MV1SetPosition(hModel, position);
	MV1SetRotationXYZ(hModel, rotation);
	MV1DrawModel(hModel);
	if (charge) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawSphere3D(position + VGet(0, 75, 0), chargePower, 5, GetColor(255, 255, 0), GetColor(255, 255, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}


	MATRIX mWeapon = MV1GetFrameLocalWorldMatrix(hModel, 73);
	VECTOR p1 = VGet(0, 0, 0) * mWeapon;
	VECTOR p2 = VGet(-50, -10, -200) * mWeapon;
	DrawLine3D(p1, p2, GetColor(255, 0, 0));

	//影
	Vertex[0].pos = position + VGet(-60.0f, 5.0f,  60.0f);
	Vertex[1].pos = position + VGet( 60.0f, 5.0f,  60.0f) ;
	Vertex[2].pos = position + VGet(-60.0f, 5.0f, -60.0f);
	Vertex[3].pos = position + VGet( 60.0f, 5.0f, -60.0f);
	Vertex[4].pos = Vertex[2].pos;
	Vertex[5].pos = Vertex[1].pos;
	DrawPolygon3D(Vertex, 2, shadowImage, TRUE);

}

/// <summary>
/// プレイヤーのHPを減らす
/// 敵側がこの関数を呼ぶ
/// </summary>
/// <param name="Damage">ダメージ数</param>
void Player::addDamage(int Damage, VECTOR ePos)
{
	counterTargetPos = ePos;
	if (isCounter) {	//カウンター
		isCounterAttack = true;
		return;
	}
	if (isInvincible) {	//無敵状態
		return;
	}
	if (isJustGuard) {	//ジャストガード
		PlaySound("data/sound/SE/Paladin/justGuard.mp3", DX_PLAYTYPE_BACK);
		return;
	}
	if (justAvoid) {	//ジャスト回避
		justAvoidTimer = 1.0f;
		animation->SetFrameSpeed(0.4f);
		PlaySound("data/sound/SE/Paladin/JustAvoidSE.mp3",DX_PLAYTYPE_BACK);
		justAvoidAttack = true;
		return;
	}
	if (isAvoidance) {	//回避
		return;
	}

	stateMachine->SetStartState(stateName_Damage);
	HP -= Damage;
	ScoreManager::AddHitDamageCount();
}

/// <summary>
/// プレイヤーに敵の攻撃が当たったらtrueを返す
/// 敵側がこの関数を呼ぶ
/// </summary>
/// <param name="_oldPos">カプセルを形成する二点中の一点の座標</param>
/// <param name="_newPos">カプセルを形成する二点中の一点の座標</param>
/// <param name="_radius">カプセルの半径</param>
/// <returns></returns>
bool Player::enemyAttack(VECTOR _oldPos, VECTOR _newPos, float _radius)
{
	MV1SetupCollInfo(hModel, -1, 4, 4, 4, -1);
	MV1RefreshCollInfo(hModel);

	bool hit = false;
	MV1_COLL_RESULT_POLY_DIM res = MV1CollCheck_Capsule(hModel, -1, _oldPos, _newPos, _radius);
	if (res.HitNum != 0) {
		hit = true;
	}
	return hit;
}

/// <summary>
/// アニメーションをセットする
/// </summary>
/// <param name="id">セットしたいアニメーションのIDをいれる</param>
/// <param name="loop">trueがループする、falseがループしない</param>
void Player::setAnimation(ANIM_ID id, bool loop)
{
	animation->Play(info[id].hAnim, loop);
	animID = id;

	canAttack = false;
	canNextAttck = false;
	isAvoidance = false;
	isNextAction = false;
}

/// <summary>
/// アニメーションを更新する
/// タイムラインチェック
/// </summary>
void Player::playAnimation()
{
	float prevFrame = animation->GetCurrentFrame();
	animation->Update();
	float curFrame = animation->GetCurrentFrame();

	std::string folder = "data/sound/SE/";
	for (TimeInfo t : info[animID].timeline) {
		if (t.time > prevFrame && curFrame >= t.time) {
			if (t.command == "SE") {
				/*PlaySound(
					"data/sound/SE/Paladin/walk.mp3",
					DX_PLAYTYPE_BACK);*/
			}
			if (t.command == "SE_Walk") {
				PlaySound(
					"data/sound/SE/Paladin/SE_Walk.mp3",
					DX_PLAYTYPE_BACK);
			}
			if (t.command == "SE_WalkR") {
				PlaySoundMem(walkSE_R, DX_PLAYTYPE_BACK, true);
			}
			if (t.command == "SE_WalkL") {
				PlaySoundMem(walkSE_L, DX_PLAYTYPE_BACK, true);
			}
			if (t.command == "SE_Attack") {
				PlaySoundMem(attackSE, DX_PLAYTYPE_BACK, true);
			}
			if (t.command == "AttackStart") {
				canAttack = true;
			}
			if (t.command == "AttackEnd") {
				canAttack = false;
			}
			if (t.command == "NextAttack") {
				canNextAttck = true;
			}
			if (t.command == "AvoidanceStart") {
				isAvoidance = true;
			}
			if (t.command == "AvoidanceEnd") {
				isAvoidance = false;
			}
			if (t.command == "NextAction") {
				isNextAction = true;
			}
			if (t.command == "ChargeStart") {
				charge = true;
			}
			if (t.command == "LocusStart") {
				isLocus = true;
			}
			if (t.command == "LocusEnd") {
				isLocus = false;
			}
			if (t.command == "JustAvoidStart") {
				justAvoid = true;
			}
			if (t.command == "JustAvoidEnd") {
				justAvoid = false;
			}
			if (t.command == "Attack4MoveStart") {
				attack4Move = true;
			}
			if (t.command == "Attack4MoveEnd") {
				attack4Move = false;
			}
		}
	}
}

/// <summary>
/// ゲームパットの左スティックの入力をvector型で返す
/// </summary>
/// <returns></returns>
VECTOR Player::getLeftStick()
{
	GetJoypadAnalogInput(&LStick_X, &LStick_Y, DX_INPUT_PAD1);
	VECTOR stickInput = VGet(0, 0, 0);
	if (LStick_X > 0 || CheckHitKey(KEY_INPUT_D)) {
		stickInput.x += 1.0f;
	}
	if (LStick_X < 0 || CheckHitKey(KEY_INPUT_A)) {
		stickInput.x += -1.0f;
	}
	if (LStick_Y > 0 || CheckHitKey(KEY_INPUT_S)) {
		stickInput.z += -1.0f;
	}
	if (LStick_Y < 0 || CheckHitKey(KEY_INPUT_W)) {
		stickInput.z += 1.0f;
	}
	return stickInput;
}

/// <summary>
/// ボタンの入力状況を更新
/// </summary>
void Player::inputPadButton()
{
	GetJoypadDirectInputState(DX_INPUT_PAD1, &inputPad);
	attackButton1	= inputPad.Buttons[0] || GetMouseInput() & MOUSE_INPUT_LEFT;
	attackButton2	= inputPad.Buttons[3] || GetMouseInput() & MOUSE_INPUT_RIGHT;
	guardButton		= inputPad.Buttons[7] || CheckHitKey(KEY_INPUT_R);
	parryButton		= inputPad.Buttons[2] || CheckHitKey(KEY_INPUT_Q);
	avoidButton		= inputPad.Buttons[1] || CheckHitKey(KEY_INPUT_SPACE);
	runButton		= inputPad.Buttons[5] || CheckHitKey(KEY_INPUT_LSHIFT);
	lockOnCameraButton = inputPad.Buttons[11];
	crossKey = *inputPad.POV;
}

/// <summary>
/// 攻撃が敵に当たっているかチェックする
/// 当たっていたら敵にダメージを与える
/// </summary>
void Player::AtackCheck()
{
	//攻撃判定
	if (canAttack) {
		MATRIX mWeapon = MV1GetFrameLocalWorldMatrix(hModel, 73);
		VECTOR p1 = VGet(0, 0, 0) * mWeapon;
		VECTOR p2 = VGet(-50, -10, -200) * mWeapon;
		for (Enemy* e : TargetManager::getEnemy()) {
			if (e->HitCheck(position, p1, p2)) {
				e->TakeDamage(attackPower);
				canAttack = false;
			}
		}
	}
}

//何もしていない状態-------------------------------------------------------------------------------
void PlayerIdle::Start()
{
}

void PlayerIdle::Update()
{
	pl->setAnimation(pl->A_STOP, true);

	//移動
	if (VSize(pl->getLeftStick()) != 0) {
		chengeNextState(pl->stateName_Move);
		isNext = true;
	}
	//回避
	if (pl->avoidButton) {
		chengeNextState(pl->stateName_Avoid);
		isNext = true;
	}
	//ガード
	if (pl->guardButton) {
		chengeNextState(pl->stateName_Guard);
		isNext = true;
	}
	//攻撃1
	if (pl->attackButton1) {
		chengeNextState(pl->stateName_Attack1);
		isNext = true;
	}
	//チャージ攻撃
	if (pl->attackButton2) {
		chengeNextState(pl->stateName_ChargeAttack);
		isNext = true;
	}
}

void PlayerIdle::OnChangeEvent()
{
	isNext = false;
}

//移動している状態-------------------------------------------------------------------------------
void PlayerMove::Start()
{
	pl->setAnimation(pl->A_Walk, true);
	moveSpeed = moveSpeed_Min;
}

void PlayerMove::Update()
{
	if (pl->runButton) {
		moveSpeed += Time::DeltaTime() * 500;
		if (moveSpeed > moveSpeed_Max) {
			moveSpeed = moveSpeed_Max;
		}
	}
	else {
		moveSpeed -= Time::DeltaTime() * 500;
		if (moveSpeed < moveSpeed_Min) {
			moveSpeed = moveSpeed_Min;
		}
	}

	if (pl->Stamina <= 10 || coolTime >= 0) {
		moveSpeed = 750.0f;
		if (coolTime <= 0) {
			coolTime = 1.0f;
		}
		else {
			coolTime -= Time::DeltaTime();
		}
	}
	else {
	}

	if (moveSpeed > 750.0f) {
		pl->setAnimation(pl->A_Run, true);
		pl->Stamina -= 2;
	}
	else {
		pl->setAnimation(pl->A_Walk, true);
		
	}

	VECTOR input = pl->getLeftStick();
	VECTOR rotation = pl->Rotation();
	float inputRot = atan2(input.x, input.z);
	if (input.x != 0 || input.z != 0)
	{
		Camera* cam = ObjectManager::FindGameObject<Camera>();
		VECTOR dir = cam->Direction();
		VECTOR moveVec = dir * (moveSpeed * Time::DeltaTime()) * MGetRotY(inputRot);
		pl->position += moveVec;
		pl->lookTarget += moveVec;
		float rotateTarget = atan2(moveVec.x, moveVec.z);
		float diff = rotateTarget - rotation.y;
		while (diff > DX_PI)
		{
			diff -= DX_PI * 2.0f;
		}
		while (diff < -DX_PI)
		{
			diff += DX_PI * 2.0f;
		}
		float limit = DX_PI / 9.0f;
		if (diff < limit && diff > -limit)
		{
			pl->rotation.y = rotateTarget;
		}
		else {
			if (diff > 0) {
				pl->rotation.y += limit;

			}
			else {
				pl->rotation.y -= limit;
			}
		}
	}

	//何もしない
	if (VSize(pl->getLeftStick()) == 0) {
		chengeNextState(pl->stateName_Idle);
		isNext = true;
	}
	//回避
	if (pl->avoidButton) {
		chengeNextState(pl->stateName_Avoid);
		isNext = true;
	}
	//ガード
	if (pl->guardButton) {
		chengeNextState(pl->stateName_Guard);
		isNext = true;
	}
	//攻撃1
	if (pl->attackButton1) {
		chengeNextState(pl->stateName_Attack1);
		isNext = true;
	}
	//チャージ攻撃
	if (pl->attackButton2) {
		chengeNextState(pl->stateName_ChargeAttack);
		isNext = true;
	}
}

void PlayerMove::OnChangeEvent()
{
	isNext = false;
}


//回避している状態-------------------------------------------------------------------------------
void PlayerAvoid::Start()
{
	pl->justAvoidAttack = false;
	pl->setAnimation(pl->A_Roll, false);
	cam = ObjectManager::FindGameObject<Camera>();

	//方向変更
	if (VSize(pl->getLeftStick()) > 0) {
		VECTOR input = pl->getLeftStick();
		float inputRot = atan2(input.x, input.z);
		VECTOR lookVec = cam->Direction() * MGetRotY(inputRot);
		float rotateTarget = atan2(lookVec.x, lookVec.z);
		pl->rotation.y = rotateTarget;
	}

	pl->Stamina -= useStamina;
	pl->isAvoidance = true;
	if (pl->Stamina <= 0) {
		pl->Stamina = 0;
	}
}

void PlayerAvoid::Update()
{
	if (pl->isAvoidance) {
		pl->position += VGet(0, 0, 500) * MGetRotY(pl->rotation.y) * Time::DeltaTime();
		pl->lookTarget += VGet(0, 0, 500) * MGetRotY(pl->rotation.y) * Time::DeltaTime();
	}

	//何もしない
	if (pl->AnimEnd()) {
		chengeNextState(pl->stateName_Idle);
		isNext = true;
	}
	if (pl->animation->GetCurrentFrame() > 62) {
		//移動
		if (VSize(pl->getLeftStick()) != 0) {
			chengeNextState(pl->stateName_Move);
			isNext = true;
		}
		//ガード
		if (pl->guardButton) {
			chengeNextState(pl->stateName_Guard);
			isNext = true;
		}
		//攻撃1
		if (pl->attackButton1) {
			if (pl->justAvoidAttack) {
				chengeNextState(pl->stateName_Attack4);
				isNext = true;
			}
			else {
				chengeNextState(pl->stateName_Attack1);
				isNext = true;
			}
		}
		//チャージ攻撃
		if (pl->attackButton2) {
			chengeNextState(pl->stateName_ChargeAttack);
			isNext = true;
		}
	}
}

void PlayerAvoid::OnChangeEvent()
{
	isNext = false;
}

//ガード-------------------------------------------------------------------------------
void PlayerGuard::Start()
{
	pl->setAnimation(pl->A_GuardStart, false);
	gState = gStart;
}

void PlayerGuard::Update()
{
	switch (gState)
	{
	case PlayerGuard::gStart:
		if (pl->parryButton && pl->guardButton) {
			gState = gParry;
		}
		if (pl->animation->IsEnd()) {
			gState = gKeep;
		}
		//return;
		break;
	case PlayerGuard::gKeep:
		pl->setAnimation(pl->A_GuardKeep, true);
		if (pl->parryButton && pl->guardButton) {
			gState = gParry;
		}
		if (!pl->guardButton) {
			gState = gEnd;
		}
		break;
	case PlayerGuard::gParry:
		pl->setAnimation(pl->A_Parry, false);
		pl->isParry = true;
		if (pl->animation->IsEnd()) {
			gState = gKeep;
			pl->isParry = false;
		}
		//return;
		break;
	case PlayerGuard::gEnd:
		pl->setAnimation(pl->A_GuardEnd, false);
		if (pl->AnimEnd()) {
			chengeNextState(pl->stateName_Idle);
			isNext = true;
		}
		//return;
		break;
	default:
		break;
	}

	//回避
	if (pl->avoidButton) {
		chengeNextState(pl->stateName_Avoid);
		isNext = true;
	}
	//攻撃1
	if (pl->attackButton1) {
		chengeNextState(pl->stateName_Attack1);
		isNext = true;
	}
	//チャージ攻撃
	if (pl->attackButton2) {
		chengeNextState(pl->stateName_ChargeAttack);
		isNext = true;
	}

}

void PlayerGuard::OnChangeEvent()
{
	isNext = false;
}

//攻撃1-------------------------------------------------------------------------------
void PlayerAttack1::Start()
{
	pl->setAnimation(pl->A_Attack1, false);
	attackKey = false;
	cam = ObjectManager::FindGameObject<Camera>();
	//方向変更
	if (VSize(pl->getLeftStick()) > 0) {
		VECTOR input = pl->getLeftStick();
		float inputRot = atan2(input.x, input.z);
		VECTOR lookVec = cam->Direction() * MGetRotY(inputRot);
		float rotateTarget = atan2(lookVec.x, lookVec.z);
		pl->rotation.y = rotateTarget;
	}
}

void PlayerAttack1::Update()
{
	//カウンター
	if (pl->attackButton1 && pl->attackButton2) {
		chengeNextState(pl->stateName_CounterPose);
		isNext = true;
		return;
	}
	//攻撃2
	if (pl->attackButton1) {
		if (!attackKey && pl->canNextAttck) {
			chengeNextState(pl->stateName_Attack2);
			isNext = true;
		}
		attackKey = true;
	}
	else {
		attackKey = false;
	}

	//何もしない
	if (pl->AnimEnd()) {
		chengeNextState(pl->stateName_Idle);
		isNext = true;
	}
	//チャージ攻撃
	if (pl->attackButton2 && pl->canNextAttck) {
		chengeNextState(pl->stateName_ChargeAttack);
		isNext = true;
	}
	//回避
	if (pl->avoidButton && pl->isNextAction) {
		chengeNextState(pl->stateName_Avoid);
		isNext = true;
	}
}

void PlayerAttack1::OnChangeEvent()
{
	pl->canAttack = false;
	isNext = false;
}

//攻撃2-------------------------------------------------------------------------------
void PlayerAttack2::Start()
{
	pl->setAnimation(pl->A_Attack2, false);
	cam = ObjectManager::FindGameObject<Camera>();
	//方向変更
	if (VSize(pl->getLeftStick()) > 0) {
		VECTOR input = pl->getLeftStick();
		float inputRot = atan2(input.x, input.z);
		VECTOR lookVec = cam->Direction() * MGetRotY(inputRot);
		float rotateTarget = atan2(lookVec.x, lookVec.z);
		pl->rotation.y = rotateTarget;
	}
}

void PlayerAttack2::Update()
{
	//攻撃3
	if (pl->attackButton1) {
		if (!attackKey && pl->canNextAttck) {
			chengeNextState(pl->stateName_Attack3);
			isNext = true;
		}
		attackKey = true;
	}
	else {
		attackKey = false;
	}

	//何もしない
	if (pl->AnimEnd()) {
		chengeNextState(pl->stateName_Idle);
		isNext = true;
	}
	//チャージ攻撃
	if (pl->attackButton2 && pl->canNextAttck) {
		chengeNextState(pl->stateName_ChargeAttack);
		isNext = true;
	}
	//回避
	if (pl->avoidButton && pl->isNextAction) {
		chengeNextState(pl->stateName_Avoid);
		isNext = true;
	}
}

void PlayerAttack2::OnChangeEvent()
{
	pl->canAttack = false;
	isNext = false;
}

//攻撃3-------------------------------------------------------------------------------
void PlayerAttack3::Start()
{
	pl->setAnimation(pl->A_Attack3, false);
	cam = ObjectManager::FindGameObject<Camera>();
	//方向変更
	if (VSize(pl->getLeftStick()) > 0) {
		VECTOR input = pl->getLeftStick();
		float inputRot = atan2(input.x, input.z);
		VECTOR lookVec = cam->Direction() * MGetRotY(inputRot);
		float rotateTarget = atan2(lookVec.x, lookVec.z);
		pl->rotation.y = rotateTarget;
	}
}

void PlayerAttack3::Update()
{
	if (pl->animation->GetCurrentFrame() < 72.0f) {
		pl->position += VGet(0, 0, 3) * MGetRotY(pl->rotation.y);
		pl->lookTarget += VGet(0, 0, 3) * MGetRotY(pl->rotation.y);
	}

	//カウンター
	if (pl->attackButton1 && pl->attackButton2) {
		chengeNextState(pl->stateName_CounterPose);
		isNext = true;
		return;
	}
	//何もしない
	if (pl->AnimEnd()) {
		chengeNextState(pl->stateName_Idle);
		isNext = true;
	}
	//回避
	if (pl->avoidButton && pl->isNextAction) {
		chengeNextState(pl->stateName_Avoid);
		isNext = true;
	}
}

void PlayerAttack3::OnChangeEvent()
{
	pl->canAttack = false;
	isNext = false;
}

//チャージ攻撃-------------------------------------------------------------------------------
void PlayerChargeAttack::Start()
{
	pl->setAnimation(pl->A_ChargeAttack, false);
	cam = ObjectManager::FindGameObject<Camera>();
	//方向変更
	if (VSize(pl->getLeftStick()) > 0) {
		VECTOR input = pl->getLeftStick();
		float inputRot = atan2(input.x, input.z);
		VECTOR lookVec = cam->Direction() * MGetRotY(inputRot);
		float rotateTarget = atan2(lookVec.x, lookVec.z);
		pl->rotation.y = rotateTarget;
	}
	pl->chargePower = 0;

	pl->isNextAction = true;
}

void PlayerChargeAttack::Update()
{
	//方向変更
	if (VSize(pl->getLeftStick()) > 0 && pl->charge) {
		VECTOR input = pl->getLeftStick();
		float inputRot = atan2(input.x, input.z);
		VECTOR lookVec = cam->Direction() * MGetRotY(inputRot);
		float rotateTarget = atan2(lookVec.x, lookVec.z);
		pl->rotation.y = rotateTarget;
	}

	if (pl->charge && pl->attackButton2) {
			pl->animation->SetFrameSpeed(0);
			if (pl->chargePower > pl->chargeMax) {
				pl->chargePower = pl->chargeMax;
			}
			else {
				pl->chargePower += 1;
			}
	}
	else {
		pl->attackPower = pl->attackPowerDefault + (pl->chargePower/5);
		pl->animation->SetFrameSpeed(1.5f);
		pl->charge = false;
	}

	//カウンター
	if (pl->attackButton1 && pl->attackButton2) {
		chengeNextState(pl->stateName_CounterPose);
		isNext = true;
		return;
	}
	//何もしない
	if (pl->AnimEnd()) {
		chengeNextState(pl->stateName_Idle);
		isNext = true;
	}
	//回避
	if (pl->avoidButton && pl->isNextAction) {
		chengeNextState(pl->stateName_Avoid);
		isNext = true;
	}
}

void PlayerChargeAttack::OnChangeEvent()
{
	pl->attackPower = pl->attackPowerDefault;
	pl->animation->SetFrameSpeed(1.5f);
	pl->charge = false;
	pl->canAttack = false;
	isNext = false;
}


//攻撃4-------------------------------------------------------------------------------
void PlayerAttack4::Start()
{
	pl->setAnimation(pl->A_Attack4, false);
	cam = ObjectManager::FindGameObject<Camera>();
	//方向変更
	if (VSize(pl->getLeftStick()) > 0) {
		VECTOR input = pl->getLeftStick();
		float inputRot = atan2(input.x, input.z);
		VECTOR lookVec = cam->Direction() * MGetRotY(inputRot);
		float rotateTarget = atan2(lookVec.x, lookVec.z);
		pl->rotation.y = rotateTarget;
	}
}

void PlayerAttack4::Update()
{	
	if (pl->attack4Move) {
		pl->position += VGet(0, 0, 3) * MGetRotY(pl->rotation.y);
		pl->lookTarget += VGet(0, 0, 3) * MGetRotY(pl->rotation.y);
	}

	//何もしない
	if (pl->AnimEnd()) {
		chengeNextState(pl->stateName_Idle);
		isNext = true;
	}
}

void PlayerAttack4::OnChangeEvent()
{
	pl->attack4Move = false;
	isNext = false;
}

//ダメージ------------------------------------------------------------------------------------
void PlayerDamage::Start()
{
	pl->setAnimation(pl->A_Damage, false);
	pl->isInvincible = true;
}

void PlayerDamage::Update()
{
	if (pl->AnimEnd()) {
		pl->isInvincible = false;
		chengeNextState(pl->stateName_Idle);
		isNext = true;
	}
}

void PlayerDamage::OnChangeEvent()
{
	isNext = false;
}

//カウンター構え------------------------------------------------------------------------------------
void PlayerCounterPose::Start()
{
	pl->setAnimation(pl->A_CounterPose, false);
	pl->isCounter = true;
	pl->isCounterAttack = false;
}

void PlayerCounterPose::Update()
{
	if (pl->isCounterAttack) {
		chengeNextState(pl->stateName_CounterAttack);
		isNext = true;
		return;
	}

	//何もしない
	if (pl->AnimEnd()) {
		chengeNextState(pl->stateName_Idle);
		isNext = true;
	}

}

void PlayerCounterPose::OnChangeEvent()
{
	pl->isCounter = false;
	isNext = false;
}

//カウンター攻撃------------------------------------------------------------------------------------
void PlayerCounterAttack::Start()
{
	pl->setAnimation(pl->A_CounterAttack, false);
	cam = ObjectManager::FindGameObject<Camera>();
	//方向変更
	if (VSize(pl->getLeftStick()) > 0) {
		VECTOR input = pl->getLeftStick();
		float inputRot = atan2(input.x, input.z);
		VECTOR lookVec = cam->Direction() * MGetRotY(inputRot);
		float rotateTarget = atan2(lookVec.x, lookVec.z);
		pl->rotation.y = rotateTarget;
	}
}

void PlayerCounterAttack::Update()
{
	//前に進む
	pl->position += VGet(0, 0, 3) * MGetRotY(pl->rotation.y);
	pl->lookTarget += VGet(0, 0, 3) * MGetRotY(pl->rotation.y);

	//何もしない
	if (pl->AnimEnd()) {
		chengeNextState(pl->stateName_Idle);
		isNext = true;
	}
}

void PlayerCounterAttack::OnChangeEvent()
{
	isNext = false;
}
