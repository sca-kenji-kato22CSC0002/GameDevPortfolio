#pragma once
#include "../Library/SceneBase.h"

/// <summary>
/// �v���C�V�[��
/// 
/// �G��|����player�����ʂƃ��U���g�Ɉړ�
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