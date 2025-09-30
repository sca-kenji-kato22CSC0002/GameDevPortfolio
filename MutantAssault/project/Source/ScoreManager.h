#pragma once

/// <summary>
/// ÉXÉRÉAÇÃä«óù
/// </summary>
enum class Difficulty {
	Easy,
	Normal,
	Hard,
	VeryHard,
};
namespace ScoreManager {
	void InitScore();
	void Update();
	void TimeCountStart();
	void TimeCountEnd();
	void AddHitDamageCount();
	void Clear();

	float ClearTime();
	int HitDamageCount();
	bool ClearCheck();

	Difficulty GetDifficulty();
	void SetDifficulty(Difficulty dif);
}
