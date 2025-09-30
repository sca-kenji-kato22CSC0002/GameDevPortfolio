#include "TargetManager.h"

namespace 
{
	vector<Enemy*> targets;
	vector<Enemy*> enemys;
	bool isEnemyIntroPlaying;
}

/// <summary>
/// ������
/// </summary>
void TargetManager::TargetmanagerInit()
{
	SetIsEnemyIntroPlaying(true);
	ClearTarget();
	ClearEnemy();
}

/// <summary>
/// �^�[�Q�b�g�����X�^�[��ǉ�
/// </summary>
/// <param name="enemy"></param>
void TargetManager::AddTarget(Enemy* enemy)
{
	targets.push_back(enemy);
	AddEnemy(enemy);
}

/// <summary>
/// Enemy��ǉ�
/// </summary>
/// <param name="enemy"></param>
void TargetManager::AddEnemy(Enemy* enemy)
{
	enemys.push_back(enemy);
}

/// <summary>
/// ���\�[�X�J��
/// </summary>
void TargetManager::ClearTarget()
{/*
	for (Enemy* e : targets) {
		delete e;
	}*/
	targets.clear();
}

void TargetManager::ClearEnemy()
{/*
	for (Enemy* e : enemys) {
		delete e;
	}*/
	enemys.clear();
}

/// <summary>
/// �S�^�[�Q�b�g�����X�^�[���擾
/// </summary>
/// <returns></returns>
vector<Enemy*> TargetManager::getTargets()
{
	return targets;
}

/// <summary>
/// �S�����X�^�[���擾
/// </summary>
/// <returns></returns>
vector<Enemy*> TargetManager::getEnemy()
{
	return enemys;
}

/// <summary>
/// �o�ꉉ�o��bool��Ԃ�
/// </summary>
/// <returns></returns>
bool TargetManager::IsEnemyIntroPlaying()
{
	return isEnemyIntroPlaying;
}

/// <summary>
/// �o�ꉉ�o��bool�ύX
/// </summary>
/// <param name="a"></param>
void TargetManager::SetIsEnemyIntroPlaying(bool a)
{
	isEnemyIntroPlaying = a;
}

