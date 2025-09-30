#pragma once
#include "../Library/GameObject.h"
/// <summary>
/// À•W‚ğ‚í‚©‚è‚â‚·‚­‚·‚é‚½‚ßü‚ğˆø‚­
/// </summary>
class Axis : public GameObject
{
public:
	Axis();
	~Axis();
	void Update() override;
	void Draw() override;
};