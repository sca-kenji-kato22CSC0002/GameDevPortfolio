#pragma once
#include "../Library/Time.h"

/// <summary>
/// �A�j���[�V�����̑��x�Ȃ�
/// </summary>
class Animation
{
public:
	Animation();
	~Animation();
	void Update();
	void SetModel(int model);
	void Play(int animModel, bool loop = true);
	float GetCurrentFrame() { return frame; }

	/// <summary>
	/// �Đ����x��ݒ肷��
	/// </summary>
	/// <param name="speed">�ʏ푬�x��1.0</param>
	void SetFrameSpeed(float speed) {
		//frameSpeed = speed / 2.0f;
		frameSpeed = ((speed * 100) * Time::DeltaTime()) / 2.0f;
	}
	bool IsEnd() {
		return !loopMode && frame >= maxFrame;
	}

private:
	int baseModel;
	int attachIndex;
	float frame; // �t���[���𐔂���
	float maxFrame;
	float frameSpeed; // �Đ����x
	bool loopMode; // ���[�v���邩
	int curAnim; // ���̃A�j���[�V����
};
