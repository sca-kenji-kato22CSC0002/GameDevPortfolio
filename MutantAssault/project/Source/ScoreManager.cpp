#include "ScoreManager.h"
#include "../Library/Time.h"

namespace {
	bool clear;
	float clearTime;
	bool timeCount;
	int hitDamageCount;

	Difficulty currentDifficulty = Difficulty::Normal;
}

void ScoreManager::InitScore()
{
	clear = false;
	timeCount = 0.0f;
	hitDamageCount = 0;
}

void ScoreManager::Update()
{
	if (timeCount) {
		clearTime += Time::DeltaTime();
	}
}

void ScoreManager::TimeCountStart()
{
	timeCount = true;
}

void ScoreManager::TimeCountEnd()
{
	timeCount = false;
}

void ScoreManager::AddHitDamageCount()
{
	hitDamageCount++;
}

void ScoreManager::Clear()
{
	clear = true;
}

float ScoreManager::ClearTime() {
	return clearTime;
}
int ScoreManager::HitDamageCount() {
	return hitDamageCount;
}

bool ScoreManager::ClearCheck()
{
	return clear;
}

Difficulty ScoreManager::GetDifficulty()
{
	return currentDifficulty;
}

void ScoreManager::SetDifficulty(Difficulty dif)
{
	currentDifficulty = dif;
}


