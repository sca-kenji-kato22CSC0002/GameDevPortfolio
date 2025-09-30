#include "PlaySceneUI.h"
#include "Camera.h"

/// <summary>
/// プレイシーンのUIを表示する
/// </summary>
PlaySceneUI::PlaySceneUI()
{
	SetDrawOrder(99);
	ObjectManager::SetDrawOrder(this,100);
	Player* pal = ObjectManager::FindGameObject<Player>();
	HP_MAX = pal->MAX_HP();
	HP_MIN = pal->MIN_HP();
	Stamina_MAX = pal->MAX_Stamina();
	Stamina_MIN = pal->MIN_Stamina();

	itemImage = LoadGraph("data/image/UI/item.png");
	explanationImage = LoadGraph("data/image/UI/plauScene_explanation.png");
}

PlaySceneUI::~PlaySceneUI()
{
	DeleteGraph(itemImage);
}

void PlaySceneUI::Update()
{
}

void PlaySceneUI::Draw()
{
	Camera* cam = ObjectManager::FindGameObject<Camera>();
	if (cam->GetEventNow()) { return; };
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawHPBar(10, 10, 500, 20);
	DrawStaminaBar(10, 30, 500, 20);
	DrawEnemyHPBar(240 * 2, 950, 240 * 4, 20);

	Player* pal = ObjectManager::FindGameObject<Player>();
	if (pal->GetPotionCoolTime() > 0) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	}
	DrawExtendGraph(100, 800, 100+250, 800+250, itemImage, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawGraph(0, 0, explanationImage, true);
}

void PlaySceneUI::DrawHPBar(int x, int y, int width, int height)
{
	// HPバーの背景を描画
	DrawBox(x-1, y-1, x + width+1, y + height+1, GetColor(0, 0, 0), TRUE);

	// HPバーの緑の部分を割合で描画
	Player* pal = ObjectManager::FindGameObject<Player>();
	NowHP = pal->NowHP();
	int HP_width = (int)((float)NowHP / HP_MAX * width);
	DrawBox(x, y, x + HP_width, y + height, GetColor(100, 255, 0), TRUE);


	SetFontSize(16);
	DrawString(x + 5, y, "HP", GetColor(0,0,0));
}

void PlaySceneUI::DrawStaminaBar(int x, int y, int width, int height)
{
	// Staminaバーの背景を描画
	DrawBox(x-1, y-1, x + width+1, y + height+1, GetColor(0, 0, 0), TRUE);

	// Staminaバーの緑の部分を割合で描画
	Player* pal = ObjectManager::FindGameObject<Player>();
	NowStamina = pal->NowStamina();
	int Stamina_width = (int)((float)NowStamina / Stamina_MAX * width);
	DrawBox(x, y, x + Stamina_width, y + height, GetColor(255, 255, 0), TRUE);


	SetFontSize(16);
	DrawString(x + 5, y, "STAMINA", GetColor(0,0,0));
}

void PlaySceneUI::DrawEnemyHPBar(int x, int y, int width, int height)
{
	// HPバーの背景を描画
	DrawBox(x - 1, y - 1, x + width + 1, y + height + 1, GetColor(0, 0, 0), TRUE);

	// HPバーの緑の部分を割合で描画
	int HP_width = (int)((float)enemy_NowHP / enemy_HP_MAX * width);
	DrawBox(x, y, x + HP_width, y + height, GetColor(255, 0, 0), TRUE);

	//名前
	SetFontSize(16);
	DrawFormatString(x + 5, y, GetColor(255, 255, 255), "%s", enemy_Name.c_str());
}

void PlaySceneUI::SetEnemyHp(int hpMax, int hpMin, int hpCurrent, std::string name)
{
	enemy_HP_MAX = hpMax;
	enemy_HP_MIN = hpMin;
	enemy_NowHP = hpCurrent;
	enemy_Name = name;
}
