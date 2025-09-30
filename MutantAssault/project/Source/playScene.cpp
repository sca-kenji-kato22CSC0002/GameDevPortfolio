#include "PlayScene.h"
#include <DxLib.h>
#include "../Library/SceneManager.h"
#include "../Library/csvReader.h"
#include "../Library/Time.h"
#include "DebugScreen.h"
#include "Screen.h"
#include "Player.h"
#include "Axis.h"
#include "Stage.h"
#include "Camera.h"
#include "SkyDoom.h"
#include "PlaySceneUI.h"
#include "ScoreManager.h"
#include "Mutant_Enemy.h"
#include "Witch_Enemy.h"
#include "TargetManager.h"

/// <summary>
/// プレイシーのオブジェクト生成やscene移動管理など
/// </summary>
PlayScene::PlayScene()
{
	TargetManager::TargetmanagerInit();
	Instantiate<Stage>();
	Instantiate<SkyDoom>();

	Instantiate<Player>();
	Instantiate<PlaySceneUI>();
	if (ScoreManager::GetDifficulty() == Difficulty::Normal) {
		TargetManager::AddTarget(Instantiate<Mutant_Enemy>());
	}
	else if(ScoreManager::GetDifficulty() == Difficulty::Hard){
		TargetManager::AddTarget(Instantiate<Witch_Enemy>());
	}
	else if(ScoreManager::GetDifficulty() == Difficulty::VeryHard){
		TargetManager::AddTarget(Instantiate<Witch_Enemy>());
		TargetManager::AddTarget(Instantiate<Mutant_Enemy>());
	}
	ScoreManager::InitScore();

	//BGM
	bossBGM = LoadSoundMem("data/sound/BGM/BGM_boss.ogg");
	ChangeVolumeSoundMem(255, bossBGM);
	StopSoundMem(bossBGM);
	PlaySoundMem(bossBGM, DX_PLAYTYPE_LOOP, true);
	Camera* cam = Instantiate<Camera>();
	cam->SetDrawOrder(100);
}

PlayScene::~PlayScene()
{
	DeleteSoundMem(bossBGM);
}

void PlayScene::Update()
{
	ScoreManager::Update();
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TitleScene");
	}

	for (Enemy* e : TargetManager::getTargets()) {
		if (!e->IsDeath()) {
			return;
		}
	}
	ScoreManager::Clear();
	SceneManager::ChangeScene("ResultScene");
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
	DrawFormatString(Screen::WIDTH - 200, 0, GetColor(255, 255, 255), "DeltaTime=%2.2fms", (Time::DeltaTime() * 1000.0f));

}
