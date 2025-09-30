#include "ResultScene.h"
#include "../Library/SceneManager.h"
#include "DebugScreen.h"
#include "Screen.h"
#include "ScoreManager.h"
#include <assert.h>
#include "../Library/Time.h"

ResultScene::ResultScene()
{
	BGimage = LoadGraph("data/image/title/title_BG.jpg");
	resultBGM = LoadSoundMem("data/sound/BGM/JINGLE_stageclear.ogg");
	PlaySoundMem(resultBGM, DX_PLAYTYPE_BACK, true);
	hitDamegeCount = ScoreManager::HitDamageCount();

	panel = LoadGraph("data/image/UI/panel_blue.png");
	assert(panel > 0);
	minutes = (int)ScoreManager::ClearTime() / 60;
	seconds = (int)ScoreManager::ClearTime() % 60;

	float clearTime = ScoreManager::ClearTime();
	if (clearTime < 60 * 1 && hitDamegeCount == 0 && ScoreManager::ClearCheck()) {
		stampImage = LoadGraph("data/image/UI/stamp_s.png");
	}
	else if (clearTime < 60 * 2 && ScoreManager::ClearCheck()) {
		stampImage = LoadGraph("data/image/UI/stamp_a.png");
	}
	else {
		stampImage = LoadGraph("data/image/UI/stamp_b.png");
	}
	stampSE = LoadSoundMem("data/sound/SE/stamp_SE.mp3");
	assert(stampImage > 0);
	stamped = false;
	size = 1;

	if (ScoreManager::ClearCheck()) {
		timer = 5;
	}
	else {
		timer = 3;;
	}
}

ResultScene::~ResultScene()
{
	DeleteGraph(BGimage);
	DeleteGraph(panel);
	DeleteGraph(stampImage);
	DeleteSoundMem(resultBGM);
	DeleteSoundMem(stampSE);
}

void ResultScene::Update()
{
	if (timer > 0) {
		timer -= Time::DeltaTime();
	}

	GetJoypadDirectInputState(DX_INPUT_PAD1, &inputPad);
	if (timer <= 0) {
		if (CheckHitKey(KEY_INPUT_T) || inputPad.Buttons[1]) {
			SceneManager::ChangeScene("TitleScene");
		}
	}

}

void ResultScene::Draw()
{
	DrawGraph(0, 0, BGimage, false);
	SetFontSize(32);
	DrawString(0, 0, "RESULT SCENE", GetColor(255, 255, 255));

	//Score表示
	bool clear = ScoreManager::ClearCheck();
	int width = Screen::WIDTH; //横
	int height = Screen::HEIGHT; //縦
	int defoltSize = GetFontSize();
	DrawExtendGraph(width / 8, height / 8, width / 8 * 7, height / 8 * 7, panel, true);
	if (timer <= 4.5f && clear) {
		SetFontSize(defoltSize * 2);
		DrawString(width / 2 - 195 + 2, height / 8 * 2 + 2, "クリアタイム", GetColor(0, 0, 0));
		DrawString(width / 2 - 195, height / 8 * 2, "クリアタイム", GetColor(255, 255, 255));
		SetFontSize(defoltSize * 1.5f);
		DrawFormatString(width / 2 - 90 + 2, height / 8 * 2 + 80 + 2, GetColor(0, 0, 0), "%.2d : %.2d", minutes, seconds);
		DrawFormatString(width / 2 - 90, height / 8 * 2 + 80, GetColor(255, 255, 255), "%.2d : %.2d", minutes, seconds);
		SetFontSize(defoltSize * 2);
		SetFontSize(defoltSize);
	}
	if (timer <= 3.5f && clear) {
		DrawString(width / 2 - 120 + 2, height / 8 * 3 + 50 + 2, "被ダメージ回数", GetColor(0, 0, 0));
		DrawString(width / 2 - 120, height / 8 * 3 + 50, "被ダメージ回数", GetColor(255, 255, 255));
		SetFontSize(defoltSize * 1.5f);
		DrawFormatString(width / 2 - 30 + 2, height / 8 * 3 + 130 + 2, GetColor(0, 0, 0), "%d回", hitDamegeCount);
		DrawFormatString(width / 2 - 30, height / 8 * 3 + 130, GetColor(255, 255, 255), "%d回", hitDamegeCount);
		SetFontSize(defoltSize);
	}
	if (!clear) {
		SetFontSize(defoltSize * 5);
		DrawString(width / 2 - 375 + 2, height / 8 * 3 + 2, "GAME OVER", GetColor(0, 0, 0));
		DrawString(width / 2 - 375, height / 8 * 3, "GAME OVER", GetColor(255, 255, 255));
		SetFontSize(defoltSize);
	}
	if (timer < 2.5f && !stamped) {
		stamped = true;
		PlaySoundMem(stampSE, DX_PLAYTYPE_BACK);
	}
	if (stamped) {
		int w, h;
		GetGraphSize(stampImage, &w, &h);
		int x = Screen::WIDTH / 2, y = Screen::HEIGHT / 6 * 4;  // スタンプ位置
		DrawExtendGraph(x - (int)(w / size), y - (int)(h / size), x + (int)(w / size), y + (int)(h / size), stampImage, TRUE);
		if (size < 10) {
			size++;
		}
	}
	if (timer <= 0) {
		SetFontSize(32);
		DrawString(width / 6 * 2 + 30, height / 8 * 6 + 50, "×ボタンor[T]Key：タイトルに戻る", GetColor(0, 0, 0));
	}

}
