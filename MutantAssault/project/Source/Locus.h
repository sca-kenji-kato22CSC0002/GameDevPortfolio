#pragma once
#include "../Library/GameObject.h"
#include "Player.h"

/// <summary>
/// ���̋O��
/// </summary>

class Locus : public GameObject {
public:
	Locus();
	~Locus();
	void Update();
	void Draw();

	/// <summary>
	/// ���̈ʒu��ǉ�����
	/// </summary>
	/// <param name="root">���̍��W</param>
	/// <param name="tip">����̍��W</param>
	void Add(VECTOR root, VECTOR tip);
private:
	static const int KeepNum = 20;
	struct Position {
		VECTOR root;
		VECTOR tip;
	};
	Position positions[KeepNum];
	int writeP;  // ���ɏ����ꏊ
	int oldestP; // ��ԌÂ��ꏊ

	int hImage; // �摜�t�@�C��

};