#pragma once
#include "../Library/GameObject.h"
#include "Player.h"

/// <summary>
/// プレイシーンのUIを表示する
/// </summary>
class PlaySceneUI : public GameObject
{
public:
	PlaySceneUI();
	~PlaySceneUI();
	void Update() override;
	void Draw() override;

private:
	/// <summary>
	/// プレイヤーのHPを表示する
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void DrawHPBar(int x, int y, int width, int height);
	int NowHP;
	int HP_MAX;
	int HP_MIN;
	int HP_width;

	/// <summary>
	/// プレイヤーのスタミナを表示する
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void DrawStaminaBar(int x, int y, int width, int height);
	int NowStamina;
	int Stamina_MAX;
	int Stamina_MIN;
	int Stamina_width;

	void DrawEnemyHPBar(int x, int y, int width, int height);
	int enemy_NowHP;
	int enemy_HP_MAX;
	int enemy_HP_MIN;
	int enemy_HP_width;
	std::string enemy_Name;

	//アイテム
	int itemImage;
	//操作説明
	int explanationImage;
public:
	/// <summary>
	/// 敵の情報をセットする
	/// </summary>
	/// <param name="hpMax">HPの最大値</param>
	/// <param name="hpMin">HPの最小値</param>
	/// <param name="hpCurrent">現在のHP</param>
	/// <param name="name">名前</param>
	void SetEnemyHp(int hpMax, int hpMin, int hpCurrent, std::string name);
};
