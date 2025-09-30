#pragma once
#include <vector>
#include "Enemy.h"
using namespace std;

namespace TargetManager {
	void TargetmanagerInit();
	void AddTarget(Enemy* enemy);
	void AddEnemy(Enemy* enemy);
	void ClearTarget();
	void ClearEnemy();
	vector<Enemy*> getTargets();
	vector<Enemy*> getEnemy();
	bool IsEnemyIntroPlaying();
	void SetIsEnemyIntroPlaying(bool a);
}
