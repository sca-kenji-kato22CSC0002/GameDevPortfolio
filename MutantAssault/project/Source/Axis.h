#pragma once
#include "../Library/GameObject.h"
/// <summary>
/// ���W���킩��₷�����邽�ߐ�������
/// </summary>
class Axis : public GameObject
{
public:
	Axis();
	~Axis();
	void Update() override;
	void Draw() override;
};