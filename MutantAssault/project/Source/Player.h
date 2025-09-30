#pragma once
#include <vector>
#include "../Library/GameObject.h"
#include "animation.h"
#include "StateMachine.h"
#include "Camera.h"
#include "PlayScene.h"
#include "Locus.h"
#include "Enemy.h"
using namespace std;

/// <summary>
/// プレイヤーの操作全般を書いたクラス
/// </summary>
class Player : public GameObject
{
public:
	friend class PlayerIdle;
	friend class PlayerMove;
	friend class PlayerAvoid;
	friend class PlayerGuard;
	friend class PlayerAttack1;
	friend class PlayerAttack2;
	friend class PlayerAttack3;
	friend class PlayerAttack4;
	friend class PlayerChargeAttack;
	friend class PlayerDamage;
	friend class PlayerCounterPose;
	friend class PlayerCounterAttack;
	Player();
	~Player();
	void Update() override;
	void Draw() override;

	VECTOR Position()
	{
		return position;
	}
	VECTOR Rotation() {
		return rotation;
	}
	VECTOR LookTarget() {
		return lookTarget;
	}
	INT NowHP() {
		return HP;
	}
	INT MAX_HP() {
		return HP_MAX;
	}
	INT MIN_HP() {
		return HP_MIN;
	}
	INT NowStamina() {
		return Stamina;
	}
	INT MAX_Stamina() {
		return Stamina_MAX;
	}
	INT MIN_Stamina() {
		return Stamina_MIN;
	}
	BOOL AnimEnd() {
		return animation->IsEnd();
	}
	BOOL IsParry() {
		return isParry;
	}
	BOOL IsLocus() {
		return isLocus;
	}
	FLOAT GetPotionCoolTime(){
		return potionCoolTime;
	}

	/// <summary>
	/// プレイヤーのHPを減らす
	/// 敵側がこの関数を呼ぶ
	/// </summary>
	/// <param name="Damage">ダメージ数</param>
	void addDamage(int Damage,VECTOR ePos);

	/// <summary>
	/// プレイヤーに敵の攻撃が当たったらtrueを返す
	/// 敵側がこの関数を呼ぶ
	/// </summary>
	/// <param name="_oldPos">カプセルを形成する二点中の一点の座標</param>
	/// <param name="_newPos">カプセルを形成する二点中の一点の座標</param>
	/// <param name="_radius">カプセルの半径</param>
	/// <returns></returns>
	bool enemyAttack(VECTOR _oldPos, VECTOR _newPos, float _radius);

private:
	//ステートマシン
	std::unique_ptr<StateMachine> stateMachine;
	const string stateName_Idle = "PlayerIdle";
	const string stateName_Move = "PlayerMove";
	const string stateName_Avoid = "PlayerAvoid";
	const string stateName_Guard = "PlayerGuard";
	const string stateName_Attack1 = "PlayerAttack1";
	const string stateName_Attack2 = "PlayerAttack2";
	const string stateName_Attack3 = "PlayerAttack3";
	const string stateName_Attack4 = "PlayerAttack4";
	const string stateName_ChargeAttack = "PlayerChargeAttack";
	const string stateName_Damage = "PlayerDamage";
	const string stateName_CounterPose = "PlayerCounterPose";
	const string stateName_CounterAttack = "PlayerCounterAttack";

	//ゲームパッド

	/// <summary>
	/// ボタンの入力状況を更新
	/// </summary>
	void inputPadButton();

	/// <summary>
	/// ゲームパットの左スティックの入力をvector型で返す
	/// </summary>
	/// <returns></returns>
	VECTOR getLeftStick();
	DINPUT_JOYSTATE inputPad;
	int LStick_X;
	int LStick_Y;
	int attackButton1;
	int attackButton2;
	int guardButton;
	int parryButton;
	int avoidButton;
	int runButton;
	int lockOnCameraButton;
	int crossKey;

	//ステータス
	int hModel;
	VECTOR position;
	VECTOR rotation;

	int HP;
	int HP_MAX = 100;
	int HP_MIN = 0;
	int Stamina;
	int Stamina_MAX = 1000;
	int Stamina_MIN = 0;
	float coolTime_Stamina;

	//animation関連
	Animation* animation;
	enum ANIM_ID {
		A_STOP = 0,
		A_Run,
		A_Walk,
		A_WalkRight,
		A_WalkLeft,
		A_Turn,
		A_Attack1,
		A_Attack2,
		A_Attack3,
		A_Attack4,
		A_ChargeAttack,
		A_GuardStart,
		A_GuardKeep,
		A_GuardEnd,
		A_Parry,
		A_test,
		A_Roll,
		A_NAttack1,
		A_NAttack2,
		A_NAttack3,
		A_Damage,
		A_Death,
		A_CounterPose,
		A_CounterAttack,
		Max
	};
	int hAnimation[Max];
	ANIM_ID animId;
	/// <summary>
	/// アニメーションをセットする
	/// </summary>
	/// <param name="id">セットしたいアニメーションのIDをいれる</param>
	/// <param name="loop">trueがループする、falseがループしない</param>
	void setAnimation(ANIM_ID id, bool loop = true);

	/// <summary>
	/// アニメーションを更新する
	/// タイムラインチェック
	/// </summary>
	void playAnimation();
	int animID;

	struct TimeInfo
	{
		float time;
		bool used;
		std::string command;
		std::string filename;
	};
	struct AnimInfo {
		int hAnim;
		std::vector<TimeInfo> timeline;
		// アニメーションのタイムライン
	};
	AnimInfo info[Max];

	//攻撃
	bool canAttack;
	bool canNextAttck;
	bool charge;
	float chargePower;
	float chargeMax = 100;
	bool justAvoidAttack;
	VECTOR counterTargetPos;
	bool isCounterAttack;
	bool isCounter;

	/// <summary>
	/// 攻撃が敵に当たっているかチェックする
	/// 当たっていたら敵にダメージを与える
	/// </summary>
	void AtackCheck();
	bool isInvincible;

	//防御
	bool isJustGuard;
	bool isParry;

	//回避
	bool isAvoidance;

	//影
	int shadowImage;
	VERTEX3D Vertex[6];
	VECTOR shadowAngle;

	//とりあえず置くとこ
	VECTOR lookTarget;	//カメラの計算に使う
	bool isNextAction;

	int walkSE_L;
	int walkSE_R;
	int attackSE;
	int attackPower;
	int attackPowerDefault = 10;

	bool isLocus;
	bool potionKey;
	float potionCoolTime;

	bool justAvoid;
	float justAvoidTimer;
	bool attack4Move;
};

/// <summary>
/// プレイヤーが何もしていない状態のクラス
/// </summary>
class PlayerIdle : public StateBase {
public:
	PlayerIdle() {}
	void Start();
	void Update();
	void OnChangeEvent();
private:
	Player* pl = ObjectManager::FindGameObject<Player>();
};

/// <summary>
/// プレイヤーが移動状態のクラス
/// </summary>
class PlayerMove : public StateBase {
public:
	PlayerMove() {}
	void Start();
	void Update();
	void OnChangeEvent();
private:
	Player* pl = ObjectManager::FindGameObject<Player>();
	float moveSpeed;
	float moveSpeed_Min = 500;
	float moveSpeed_Max = 1000;
	float coolTime = 0.0f;
};

/// <summary>
/// プレイヤーが回避状態のクラス
/// </summary>
class PlayerAvoid : public StateBase {
public:
	PlayerAvoid() {}
	void Start();
	void Update();
	void OnChangeEvent();
private:
	Player* pl = ObjectManager::FindGameObject<Player>();
	Camera* cam;
	int useStamina = pl->Stamina_MAX / 5;
};

/// <summary>
/// プレイヤーがガード・パリィ状態のクラス
/// </summary>
class PlayerGuard : public StateBase {
public:
	PlayerGuard() {}
	void Start();
	void Update();
	void OnChangeEvent();
private:
	Player* pl = ObjectManager::FindGameObject<Player>();
	enum GState {
		gStart,
		gKeep,
		gParry,
		gEnd,
	};
	GState gState;
};

/// <summary>
/// プレイヤーが攻撃1状態のクラス
/// </summary>
class PlayerAttack1 : public StateBase {
public:
	PlayerAttack1() {}
	void Start();
	void Update();
	void OnChangeEvent();
private:
	Player* pl = ObjectManager::FindGameObject<Player>();
	Camera* cam = ObjectManager::FindGameObject<Camera>();
	bool attackKey;
};


/// <summary>
/// プレイヤーが攻撃2状態のクラス
/// </summary>
class PlayerAttack2 : public StateBase {
public:
	PlayerAttack2() {}
	void Start();
	void Update();
	void OnChangeEvent();
private:
	Player* pl = ObjectManager::FindGameObject<Player>();
	Camera* cam = ObjectManager::FindGameObject<Camera>();
	bool attackKey;
};


/// <summary>
/// プレイヤーが攻撃3状態のクラス
/// </summary>
class PlayerAttack3 : public StateBase {
public:
	PlayerAttack3() {}
	void Start();
	void Update();
	void OnChangeEvent();
private:
	Player* pl = ObjectManager::FindGameObject<Player>();
	Camera* cam = ObjectManager::FindGameObject<Camera>();
};

/// <summary>
/// プレイヤーが攻撃4状態のクラス
/// </summary>
class PlayerAttack4 : public StateBase {
public:
	PlayerAttack4() {}
	void Start();
	void Update();
	void OnChangeEvent();
private:
	Player* pl = ObjectManager::FindGameObject<Player>();
	Camera* cam = ObjectManager::FindGameObject<Camera>();
};

/// <summary>
/// プレイヤーがチャージ攻撃状態のクラス
/// </summary>
class PlayerChargeAttack : public StateBase {
public:
	PlayerChargeAttack() {}
	void Start();
	void Update();
	void OnChangeEvent();
private:
	Player* pl = ObjectManager::FindGameObject<Player>();
	Camera* cam = ObjectManager::FindGameObject<Camera>();
	bool cherge;
};

/// <summary>
/// 攻撃を受けたときの状態
/// </summary>
class PlayerDamage : public StateBase {
public:
	PlayerDamage() {}	
	void Start();
	void Update();
	void OnChangeEvent();
private:
	Player* pl = ObjectManager::FindGameObject<Player>();
};

/// <summary>
/// カウンター構え状態
/// </summary>
class PlayerCounterPose : public StateBase {
public:
	PlayerCounterPose() {}
	void Start();
	void Update();
	void OnChangeEvent();
private:
	Player* pl = ObjectManager::FindGameObject<Player>();
};

/// <summary>
/// カウンター攻撃状態
/// </summary>
class PlayerCounterAttack : public StateBase {
public:
	PlayerCounterAttack() {}
	void Start();
	void Update();
	void OnChangeEvent();
private:
	Player* pl = ObjectManager::FindGameObject<Player>();
	Camera* cam = ObjectManager::FindGameObject<Camera>();
};
