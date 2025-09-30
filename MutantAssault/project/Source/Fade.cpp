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
	// �t�F�[�h�A�E�g�����ifadeTime �̓t���[�����j
	for (int i = 0; i <= fadeTime; ++i)
	{
		// ������`�𔼓����ŕ`��
		int alpha = 255 * i / fadeTime;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// ��ʂ��X�V
		ScreenFlip();

		// �����҂�
		WaitTimer(16); // ��60FPS
	}
	fadeIn = true;
}
