#pragma once
#include "../Library/GameObject.h"

/// <summary>
/// ���\��
/// </summary>
class SkyDoom : public GameObject
{
public:
	SkyDoom();
	~SkyDoom();
	void Update() override;
	void Draw() override;
private:
	int hModel;
	VECTOR position;
};
