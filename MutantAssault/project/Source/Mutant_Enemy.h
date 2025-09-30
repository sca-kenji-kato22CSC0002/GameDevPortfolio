#pragma once
#include "Enemy.h"
#include "../Library/ObjectManager.h"
#include "Player.h"
#include "../Library/GameObject.h"
#include "../Library/Time.h"
using namespace std;

/// <summary>
/// �G�iMutant�j�̓����S��
/// </summary>
class Mutant_Enemy : public Enemy {
public:
	friend class Idle;
	friend class MoveCheck;
	friend class Move;
	friend class Attack1Check;
	friend class Attack1;
	friend class Attack2Check;
	friend class Attack2;
	friend class JumpAttackCheck;
	friend class JumpAttack;
	friend class DeathCheck;
	friend class Death;
	Mutant_Enemy();
	~Mutant_Enemy();
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
		A_Stop = 0,
		A_Walk,
		A_Attack,
		A_Attack2,
		A_Attack3,
		A_JumpAttack,
		A_Death,
		A_Down,
		A_StandUp,
		A_Roaning,
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

	//�U�������position
	VECTOR oldArmPos;
	VECTOR newArmPos;
	//�U���֘A
	bool attackMove;
	//�W�����v
	bool jumpA;
	float jumpPower;	
	//�e
	int shadowImage;
	VERTEX3D Vertex[6];
	VECTOR shadowAngle;
	VECTOR shadowPos;

	//�G��AI
	unique_ptr<Selector> root;
	unique_ptr<Sequence> idleSeq;
	unique_ptr<Sequence> moveSeq;
	unique_ptr<Sequence> attack1Seq;
	unique_ptr<Sequence> attack2Seq;
	unique_ptr<Sequence> jumpAttackSeq;
	unique_ptr<Sequence> deathSeq;

	Player* pl;
	void LookPlayer();

	//�o�ꉉ�o
	bool isEnemyIntroPlaying;
	//SE
	int SE_Roar;
	int SE_JumpAttack;
};

/// <summary>
/// �������Ȃ����
/// </summary>
class Idle : public BTNode {
public:
	Idle(Mutant_Enemy* mutant);
	NodeStatus Tick() override;
private:
	Mutant_Enemy* mu;
};

/// <summary>
/// �ړ��m�F
/// </summary>
/// <returns></returns>
class MoveCheck : public BTNode {
public:
	MoveCheck(Mutant_Enemy* mutant);
	NodeStatus Tick() override;
private:
	Mutant_Enemy* mu;
};

class Move : public BTNode {
public:
	Move(Mutant_Enemy* mutant);
	NodeStatus Tick() override;
private:
	Mutant_Enemy* mu;
};

/// <summary>
/// �U��1����
/// </summary>
class Attack1Check : public BTNode {
public:
	Attack1Check(Mutant_Enemy* mutant);
	NodeStatus Tick() override;
private:
	Mutant_Enemy*  mu;
	bool isAttacking;
};
/// <summary>
/// �U��1���s
/// </summary>
class Attack1 : public BTNode {
public:
	Attack1(Mutant_Enemy* mutant);
	NodeStatus Tick() override;
private:
	Mutant_Enemy* mu;
};

/// <summary>
/// �U��2����
/// </summary>
class Attack2Check : public BTNode {
public:
	Attack2Check(Mutant_Enemy* mutant);
	NodeStatus Tick() override;
private:
	Mutant_Enemy*  mu;
	bool isAttacking;
};
/// <summary>
/// �U��2���s
/// </summary>
class Attack2 : public BTNode {
public:
	Attack2(Mutant_Enemy* mutant);
	NodeStatus Tick() override;
private:
	Mutant_Enemy* mu;
};

/// <summary>
/// �W�����v�U������
/// </summary>
class JumpAttackCheck : public BTNode
{
public:
	JumpAttackCheck(Mutant_Enemy* mutant);
	NodeStatus Tick() override;
private:
	Mutant_Enemy* mu;
	bool isAttacking;
};
/// <summary>
/// �W�����v�U�����s
/// </summary>
/// <returns></returns>
class JumpAttack : public BTNode
{
public:
	JumpAttack(Mutant_Enemy* mutant);
	NodeStatus Tick() override;
private:
	Mutant_Enemy* mu;
};

/// <summary>
/// ���S�m�F
/// </summary>
class DeathCheck : public BTNode 
{
public:
	DeathCheck(Mutant_Enemy* mutant);
	NodeStatus Tick();
private:
	Mutant_Enemy* mu;
};

/// <summary>
/// ���S
/// </summary>
class Death : public BTNode
{
public:
	Death(Mutant_Enemy* mutant);
	NodeStatus Tick();
private:
	Mutant_Enemy* mu;
};