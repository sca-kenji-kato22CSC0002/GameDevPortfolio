#include "TargetManager.h"

namespace 
{
	vector<Enemy*> targets;
	vector<Enemy*> enemys;
	bool isEnemyIntroPlaying;
}

/// <summary>
/// 初期化
/// </summary>
void TargetManager::TargetmanagerInit()
{
	SetIsEnemyIntroPlaying(true);
	ClearTarget();
	ClearEnemy();
}

/// <summary>
/// ターゲットモンスターを追加
/// </summary>
/// <param name="enemy"></param>
void TargetManager::AddTarget(Enemy* enemy)
{
	targets.push_back(enemy);
	AddEnemy(enemy);
}

/// <summary>
/// Enemyを追加
/// </summary>
/// <param name="enemy"></param>
void TargetManager::AddEnemy(Enemy* enemy)
{
	enemys.push_back(enemy);
}

/// <summary>
/// リソース開放
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
/// 全ターゲットモンスターを取得
/// </summary>
/// <returns></returns>
vector<Enemy*> TargetManager::getTargets()
{
	return targets;
}

/// <summary>
/// 全モンスターを取得
/// </summary>
/// <returns></returns>
vector<Enemy*> TargetManager::getEnemy()
{
	return enemys;
}

/// <summary>
/// 登場演出のboolを返す
/// </summary>
/// <returns></returns>
bool TargetManager::IsEnemyIntroPlaying()
{
	return isEnemyIntroPlaying;
}

/// <summary>
/// 登場演出のbool変更
/// </summary>
/// <param name="a"></param>
void TargetManager::SetIsEnemyIntroPlaying(bool a)
{
	isEnemyIntroPlaying = a;
}

