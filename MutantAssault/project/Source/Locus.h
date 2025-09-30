#pragma once
#include "../Library/GameObject.h"
#include "Player.h"

/// <summary>
/// 剣の軌跡
/// </summary>

class Locus : public GameObject {
public:
	Locus();
	~Locus();
	void Update();
	void Draw();

	/// <summary>
	/// 剣の位置を追加する
	/// </summary>
	/// <param name="root">柄の座標</param>
	/// <param name="tip">剣先の座標</param>
	void Add(VECTOR root, VECTOR tip);
private:
	static const int KeepNum = 20;
	struct Position {
		VECTOR root;
		VECTOR tip;
	};
	Position positions[KeepNum];
	int writeP;  // 次に書く場所
	int oldestP; // 一番古い場所

	int hImage; // 画像ファイル

};