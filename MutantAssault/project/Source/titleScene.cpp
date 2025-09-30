#include "TitleScene.h"
#include "../Library/SceneManager.h"
#include "DebugScreen.h"
#include "Screen.h"

TitleScene::TitleScene()
{
	BGimage = LoadGraph("data/image/title/title_BG.jpg");
	black = GetColor(0, 0, 0);
	bgm = LoadSoundMem("data/sound/BGM/BGM_title.ogg");
	SE_Select = LoadSoundMem("data/sound/SE/Title/SE_Select.mp3");
	SE_Decision = LoadSoundMem("data/sound/SE/Title/SE_Enter.mp3");
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP, TRUE);

	//ui
	panel = LoadGraph("data/image/UI/panel_blue.png");
	arrow = LoadGraph("data/image/UI/arrowSilver_right.png");
	selectMenuNum = selectMenuNum_Min;
	selectDiffNum = selectDiffNum_Min;
	selectKey_Up = false;
	selectKey_Down = false;
	spaceKey = false;
	explanationImage_PS = LoadGraph("data/image/title/操作説明_PS.png");
	explanationImage_PC = LoadGraph("data/image/title/操作説明_PC.png");

	operationInstructions = false;

	fadeOut = false;

	tState = MENU;
}

TitleScene::~TitleScene()
{
	DeleteGraph(panel);
	DeleteGraph(arrow);
	DeleteGraph(BGimage);
	DeleteSoundMem(bgm);
	DeleteSoundMem(SE_Select);
	DeleteSoundMem(SE_Decision);
}

void TitleScene::Update()
{
	GetJoypadDirectInputState(DX_INPUT_PAD1, &inputPad);
	switch (tState)
	{
	case TitleScene::MENU:
		MenuUpdate();
		break;
	case TitleScene::EXPLANATION_PS:
		ExplanationUpdatePS();
		break;
	case TitleScene::EXPLANATION_PC:
		ExplanationUpdatePC();
		break;
	case TitleScene::DifficultySelect:
		DifficultySelectUpdate();
		break;
	default:
		tState = MENU;
		break;
	}
}


void TitleScene::Draw()
{
	DrawGraph(0, 0, BGimage, false);

	switch (tState)
	{
	case TitleScene::MENU:
		//タイトル文字
		SetFontSize(180);
		DrawString(1280 / 4 + 3, 720 / 6 + 80 + 3, "Mutant Assault", GetColor(0, 0, 0));
		DrawString(1280 / 4 - 3, 720 / 6 + 80 - 3, "Mutant Assault", GetColor(0, 0, 0));
		DrawString(1280 / 4 - 3, 720 / 6 + 80 + 3, "Mutant Assault", GetColor(0, 0, 0));
		DrawString(1280 / 4 + 3, 720 / 6 + 80 - 3, "Mutant Assault", GetColor(0, 0, 0));
		DrawString(1280 / 4, 720 / 6 + 80, "Mutant Assault", GetColor(255, 255, 255));

		//メニュー
		DrawExtendGraph(320 * 2, 180 * 3, 320 * 4, 180 * 7 - 100, panel, true);
		SetFontSize(32);
		DrawString(0, 0, "TITLE SCENE", GetColor(255, 255, 255));
		DrawString(690, 750 + 75 * -2, "PS4/5のコントローラーで遊べます", GetColor(255, 255, 255));
		DrawString(680, 750 + 75 * -1 - 30, "   WASD：選択　   SPACE：決定", GetColor(255, 255, 255));
		DrawString(680, 750 + 75 * -1, "十字ボタン：選択　×ボタン：決定", GetColor(255, 255, 255));
		DrawString(800, 750 + 75 * 0, "[P]Key：スタート", GetColor(255, 255, 255));
		DrawString(800, 750 + 75 * 1, "[T]Key：操作説明(PS)", GetColor(255, 255, 255));
		DrawString(800, 750 + 75 * 2, "[Y]Key：操作説明(PC)", GetColor(255, 255, 255));
		DrawString(800, 750 + 75 * 3, "[esc]Key：アプリ終了", GetColor(255, 255, 255));
		DrawExtendGraph(750, 750 + 75 * selectMenuNum, 790, 790 + 75 * selectMenuNum, arrow, true);
		break;
	case TitleScene::EXPLANATION_PS:
		//操作説明
		DrawGraph(0, 0, explanationImage_PS, false);
		break;
	case TitleScene::EXPLANATION_PC:
		//操作説明
		DrawGraph(0, 0, explanationImage_PC, false);
		break;
	case TitleScene::DifficultySelect:
		//難易度設定
		DrawExtendGraph(320 * 2, 180 * 3, 320 * 4, 180 * 6 - 00, panel, true);
		SetFontSize(32);
		DrawString(0, 0, "TITLE SCENE", GetColor(255, 255, 255));
		DrawString(690, 750 + 75 * -2, "難易度を選んでください", GetColor(255, 255, 255));
		DrawString(680, 750 + 75 * -1 - 30, "   WASD：選択　   SPACE：決定", GetColor(255, 255, 255));
		DrawString(680, 750 + 75 * -1, "十字ボタン：選択　×ボタン：決定", GetColor(255, 255, 255));
		DrawString(800, 750 + 75 * 0, "NORMAL", GetColor(255, 255, 255));
		DrawString(800, 750 + 75 * 1, "HARD", GetColor(255, 255, 255));
		DrawString(800, 750 + 75 * 2, "VERYHARD", GetColor(255, 255, 255));
		DrawString(800, 750 + 75 * 3, "もどる", GetColor(255, 255, 255));
		DrawExtendGraph(750, 750 + 75 * selectDiffNum, 790, 790 + 75 * selectDiffNum, arrow, true);
		break;
	default:
		break;
	}

	if (fadeOut) {
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
	}
}


void TitleScene::FadeOut(int fadeTime)
{
	for (int i = 0; i <= fadeTime; ++i)
	{
		int alpha = 255 * i / fadeTime;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		ScreenFlip();

		WaitTimer(16);
	}
	fadeOut = true;
}

void TitleScene::MenuUpdate()
{
	if (CheckHitKey(KEY_INPUT_P)) {
		tState = DifficultySelect;
	}
	if (CheckHitKey(KEY_INPUT_T)) {
		if (!tKey) {
			tState = EXPLANATION_PS;
		}
		tKey = true;
	}
	else {
		tKey = false;
	}
	if (CheckHitKey(KEY_INPUT_Y)) {
		if (!tKey) {
			tState = EXPLANATION_PC;
		}
		tKey = true;
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}

	//メニューセレクト
	crossKey = *inputPad.POV;
	if (CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_UP) || crossKey == 18000) {
		if (!selectKey_Up) {
			PlaySoundMem(SE_Select, DX_PLAYTYPE_BACK, true);
			selectMenuNum++;
			if (selectMenuNum > selectMenuNum_Max) {
				selectMenuNum = selectMenuNum_Min;
			}
		}
		selectKey_Up = true;
	}
	else {
		selectKey_Up = false;
	}
	if (CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_DOWN) || crossKey == 0) {
		if (!selectKey_Down) {
			PlaySoundMem(SE_Select, DX_PLAYTYPE_BACK, true);
			selectMenuNum--;
			if (selectMenuNum < selectMenuNum_Min) {
				selectMenuNum = selectMenuNum_Max;
			}
		}
		selectKey_Down = true;
	}
	else {
		selectKey_Down = false;
	}
	//決定
	if (CheckHitKey(KEY_INPUT_SPACE) || inputPad.Buttons[1]) {
		if (!spaceKey) {
			switch (selectMenuNum % 4)
			{
			case 0:
				tState = DifficultySelect;
				break;
			case 1:
				tState = EXPLANATION_PS;
				break;
			case 2:
				tState = EXPLANATION_PC;
				break;
			case 3:
				SceneManager::Exit();
				break;
			default:
				break;
			}
		}
		spaceKey = true;
	}
	else {
		spaceKey = false;
	}
}

void TitleScene::ExplanationUpdatePS()
{
	if (CheckHitKey(KEY_INPUT_T) || inputPad.Buttons[2]) {
		if (!tKey) {
			tState = MENU;
		}
		tKey = true;
	}
	else {
		tKey = false;
	}
}

void TitleScene::ExplanationUpdatePC()
{
	if (CheckHitKey(KEY_INPUT_T) || inputPad.Buttons[2]) {
		if (!tKey) {
			tState = MENU;
		}
		tKey = true;
	}
	else {
		tKey = false;
	}
}

void TitleScene::DifficultySelectUpdate()
{
	//メニューセレクト
	crossKey = *inputPad.POV;
	if (CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_UP) || crossKey == 18000) {
		if (!selectKey_Up) {
			PlaySoundMem(SE_Select, DX_PLAYTYPE_BACK, true);
			selectDiffNum++;
			if (selectDiffNum > selectDiffNum_Max) {
				selectDiffNum = selectDiffNum_Min;
			}
		}
		selectKey_Up = true;
	}
	else {
		selectKey_Up = false;
	}
	if (CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_DOWN) || crossKey == 0) {
		if (!selectKey_Down) {
			PlaySoundMem(SE_Select, DX_PLAYTYPE_BACK, true);
			selectDiffNum--;
			if (selectDiffNum < selectDiffNum_Min) {
				selectDiffNum = selectDiffNum_Max;
			}
		}
		selectKey_Down = true;
	}
	else {
		selectKey_Down = false;
	}
	//決定
	if (CheckHitKey(KEY_INPUT_SPACE) || inputPad.Buttons[1]) {
		PlaySoundMem(SE_Decision, DX_PLAYTYPE_BACK, true);
		if (!spaceKey) {
			switch (selectDiffNum)
			{
			case 0:
				ScoreManager::SetDifficulty(Difficulty::Normal);
				FadeOut(60);
				StopSoundMem(bgm);
				SceneManager::ChangeScene("PlayScene");
				break;
			case 1:
				ScoreManager::SetDifficulty(Difficulty::Hard);
				FadeOut(60);
				StopSoundMem(bgm);
				SceneManager::ChangeScene("PlayScene");
				break;
			case 2:
				ScoreManager::SetDifficulty(Difficulty::VeryHard);
				FadeOut(60);
				StopSoundMem(bgm);
				SceneManager::ChangeScene("PlayScene");
				break;
			case 3:
				tState = MENU;
				break;
			default:
				break;
			}
		}
		spaceKey = true;
	}
	else {
		spaceKey = false;
	}

}
