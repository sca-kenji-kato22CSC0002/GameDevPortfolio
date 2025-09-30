#pragma once
#include "../Library/SceneBase.h"

/// <summary>
/// プレイシーン
/// 
/// 敵を倒すかplayerが死ぬとリザルトに移動
/// </summary>
class PlayScene : public SceneBase {
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
private:
	int bossBGM;
};