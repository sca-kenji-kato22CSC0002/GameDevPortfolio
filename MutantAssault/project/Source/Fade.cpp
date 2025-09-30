#include "Fade.h"

Fade::Fade()
{
	fadeIn = false;
}

Fade::~Fade()
{
	DestroyMe();
}

void Fade::Update()
{
}

void Fade::Draw()
{
	if (fadeIn) {
		ClearDrawScreen();
	}
}

void Fade::FadeOut(float fadeTime)
{
	// フェードアウト処理（fadeTime はフレーム数）
	for (int i = 0; i <= fadeTime; ++i)
	{
		// 黒い矩形を半透明で描画
		int alpha = 255 * i / fadeTime;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 画面を更新
		ScreenFlip();

		// 少し待つ
		WaitTimer(16); // 約60FPS
	}
	fadeIn = true;
}
