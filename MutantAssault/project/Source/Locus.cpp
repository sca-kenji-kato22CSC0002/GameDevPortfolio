#include "Locus.h"
#include <assert.h>
#include <vector>
#include "Player.h"

Locus::Locus()
{
	ObjectManager::SetDrawOrder(this, -100);
	writeP = 0;
	oldestP = 0;

	hImage = LoadGraph("data/Effect/SlashLocus.png");
	assert(hImage > 0);
	
}

Locus::~Locus()
{
	DeleteGraph(hImage);
}

void Locus::Update()
{
}

void Locus::Draw()
{
	Player* pl = ObjectManager::FindGameObject<Player>();
	if (!pl->IsLocus()) {
		return;
	}
	std::vector<VERTEX3D> vertexes;
	std::vector<unsigned short> indexes;

	VERTEX3D v;
	v.norm = VGet(0, 1, 0);
	v.dif = GetColorU8(255, 255, 255, 255);
	v.spc = GetColorU8(255, 255, 255, 255);
	for (int i = oldestP; i != writeP; i = (i + 1) % KeepNum) {
		v.pos = positions[i].root;
		v.u = 0.0f;
		v.v = 0.0f;
		vertexes.push_back(v);
		v.pos = positions[i].tip;
		v.u = 1.0f;
		v.v = 0.0f;
		vertexes.push_back(v);
	}
	//グラデーション(v)を設定する
	float val = 0.0f;
	for (int i = vertexes.size() - 2; i >= 0; i -= 2) {
		vertexes[i].v = val;
		vertexes[i+1].v = val;
		val += 1.0f / KeepNum;
	}
	if (vertexes.size() < 4)
		return;
	// indexesを作る
	for (int n = 0; n < vertexes.size()-2; n += 2) {
		indexes.push_back(n + 0);
		indexes.push_back(n + 1);
		indexes.push_back(n + 2);
		indexes.push_back(n + 1);
		indexes.push_back(n + 3);
		indexes.push_back(n + 2);
	}
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	DrawPolygonIndexed3D(&vertexes[0], vertexes.size(),
		&indexes[0], indexes.size() / 3, hImage, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Locus::Add(VECTOR root, VECTOR tip)
{
	Position p;
	p.root = root;
	p.tip = tip;
	positions[writeP] = p;
	writeP = (writeP + 1) % KeepNum;
	if (writeP == oldestP) {
		oldestP = (oldestP + 1) % KeepNum;
	}
}
