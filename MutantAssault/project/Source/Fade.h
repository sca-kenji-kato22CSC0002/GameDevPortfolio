#pragma once
#include "DxLib.h"
#include "../Library/GameObject.h"

class Fade : public GameObject {
public:
	Fade();
	~Fade();
	void Update() override;
	void Draw() override;

	void FadeOut(float fadeTime);

private:
	bool fadeIn;
};