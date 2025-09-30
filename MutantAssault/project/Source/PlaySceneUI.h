#pragma once
#include "../Library/GameObject.h"
#include "Player.h"

/// <summary>
/// �v���C�V�[����UI��\������
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
	/// �v���C���[��HP��\������
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
	/// �v���C���[�̃X�^�~�i��\������
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

	//�A�C�e��
	int itemImage;
	//�������
	int explanationImage;
public:
	/// <summary>
	/// �G�̏����Z�b�g����
	/// </summary>
	/// <param name="hpMax">HP�̍ő�l</param>
	/// <param name="hpMin">HP�̍ŏ��l</param>
	/// <param name="hpCurrent">���݂�HP</param>
	/// <param name="name">���O</param>
	void SetEnemyHp(int hpMax, int hpMin, int hpCurrent, std::string name);
};
