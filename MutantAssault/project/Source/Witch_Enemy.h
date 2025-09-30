#pragma once
#include "Enemy.h"
#include "../Library/ObjectManager.h"
#include "BehaviorTree.h"
#include "Player.h"
#include "../Library/GameObject.h"
#include "../Library/Time.h"
#include "Camera.h"
#include "ThrowRock.h"
using namespace std;

/// <summary>
/// �G(Witch)�̓����S��
/// </summary>
class Witch_Enemy : public Enemy {
public:
	friend class WitchIdle;
	friend class WitchMoveCheck;
	friend class WitchMove;
	friend class WitchAttack1Check;
	friend class WitchAttack1;
	friend class WitchAttack2Check;
	friend class WitchAttack2;
	friend class WitchAttack3Check;
	friend class WitchAttack3;
	friend class WitchDeathCheck;
	friend class WitchDeath;
	Witch_Enemy();
	~Witch_Enemy();
	void Update() override;
	void Draw() override;
	void Attack() override;
	void TakeDamage(int damage) override;

private:
	void onGround();
	void ShadowInit();
	void ShadowDraw();

	//animation�֘A----------------------------------------------------------------
	Animation* animation;
	enum ANIM_ID {
		A_Idle = 0,
		A_Walk,
		A_Attack1,
		A_Attack2,
		A_Attack3,
		A_Damage,
		A_Death,
		A_Appearance,
		Max
	};
	void setAnimation(ANIM_ID id, bool loop = true);
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
		// �A�j���[�V�����̃^�C�����C��
	};
	AnimInfo info[Max];
	//�e
	int shadowImage;
	VERTEX3D Vertex[6];
	VECTOR shadowAngle;
	VECTOR shadowPos;
	//�W�����v
	bool jumpA;
	float jumpPower;
	//�o�ꉉ�o
	bool isEnemyIntroPlaying;
	//SE
	int SE_GrowRock;
	int SE_ThrowRock;
	int SE_Voice;

	//�U��
	Player* pl;
	void LookPlayer();
	Camera* cam;
	bool isNextMove;
	ThrowRock* tRock;
	
	//�GAI
	unique_ptr<Selector> root;
	unique_ptr<Sequence> idleSeq;
	unique_ptr<Sequence> moveSeq;
	unique_ptr<Sequence> attack1Seq;
	unique_ptr<Sequence> attack2Seq;
	unique_ptr<Sequence> attack3Seq;
	unique_ptr<Sequence> deathSeq;
};

/// <summary>
/// �������Ȃ����
/// </summary>
class WitchIdle : public BTNode {
public:
	WitchIdle(Witch_Enemy* witch);
	NodeStatus Tick() override;
private:
	Witch_Enemy* wi;
};

/// <summary>
/// �ړ����
/// </summary>
class WitchMoveCheck : public BTNode {
public:
	WitchMoveCheck(Witch_Enemy* witch);
	NodeStatus Tick() override;
private:
	Witch_Enemy* wi;
};

class WitchMove : public BTNode {
public:
	WitchMove(Witch_Enemy* witch);
	NodeStatus Tick() override;
private:
	Witch_Enemy* wi;
};

/// <summary>
/// �U��1���
/// </summary>
class WitchAttack1Check : public BTNode {
public:
	friend class WitchAttack1;
	WitchAttack1Check(Witch_Enemy* witch);
	NodeStatus Tick() override;
private:
	Witch_Enemy* wi;
	bool isAttacking;
};

class WitchAttack1 : public BTNode {
public:
	WitchAttack1(Witch_Enemy* witch);
	NodeStatus Tick() override;
private:
	Witch_Enemy* wi;
};

/// <summary>
/// �U��2���
/// </summary>
class WitchAttack2Check : public BTNode {
public:
	friend class WitchAttack2;
	WitchAttack2Check(Witch_Enemy* witch);
	NodeStatus Tick() override;
private:
	Witch_Enemy* wi;
	bool isAttacking;
};

class WitchAttack2 : public BTNode {
public:
	WitchAttack2(Witch_Enemy* witch);
	NodeStatus Tick() override;
private:
	Witch_Enemy* wi;
};

/// <summary>
/// �U��3���
/// </summary>
class WitchAttack3Check : public BTNode {
public:
	friend class WitchAttack3;
	WitchAttack3Check(Witch_Enemy* witch);
	NodeStatus Tick() override;
private:
	Witch_Enemy* wi;
	bool isAttacking;
};

class WitchAttack3 : public BTNode {
public:
	WitchAttack3(Witch_Enemy* witch);
	NodeStatus Tick() override;
private:
	Witch_Enemy* wi;
};

/// <summary>
/// ���S
/// </summary>
class WitchDeathCheck : public BTNode {
public:
	WitchDeathCheck(Witch_Enemy* witch);
	NodeStatus Tick() override;
private:
	Witch_Enemy* wi;
};

class WitchDeath : public BTNode {
public:
	WitchDeath(Witch_Enemy* witch);
	NodeStatus Tick() override;
private:
	Witch_Enemy* wi;
};
