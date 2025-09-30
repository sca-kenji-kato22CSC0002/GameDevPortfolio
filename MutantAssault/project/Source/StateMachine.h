#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>


class StateBase
{
public:
	StateBase() : isNext(false)
	{
	};

	// ƒAƒbƒvƒf[ƒg‚Ì‘O‚ÉŒÄ‚Î‚ê‚é
	virtual void Start() = 0;

	// ƒAƒbƒvƒf[ƒg
	virtual void Update() = 0;

	// ó‘Ô‚ª•Ï‚í‚é‚Æ‚«‚Ìˆ—
	virtual void OnChangeEvent() = 0;

	// Ø‚è‘Ö‚¦‚éğŒ‚ğæ“¾
	bool IsNext()const { return isNext; }

	//Ø‚è‘Ö‚¦‚é
	void chengeNext(bool next) { isNext = next; }

	//Ÿ‚ÌState‚ÉˆÚ“®‚Å‚«‚é‚©
	bool CanNextState() {
		if (IsNext()) {
			OnChangeEvent();
			return true;
		}
		return false;
	}

	//Ÿ‚És‚­ó‘Ô‚ğ•ÏX
	void chengeNextState(std::string name) {
		nextRegisterName = name;
	}

	//Ÿ‚És‚­ó‘Ô‚ğæ“¾
	std::string getNextName() {
		return nextRegisterName;
	}
protected:
	// Ÿ‚És‚­‚½‚ß‚ÌğŒ
	bool isNext;

	//Ÿ‚ÌState‚Ì–¼‘O
	std::string nextRegisterName;

};

// ó‘ÔŠÇ—
class StateMachine
{
public:
	StateMachine() :
		nowState()
	{

	}

	void Update()
	{
		nowState->Update();
		if (nowState->CanNextState())
		{
			auto it = states.find(nowState->getNextName());
			if (it == states.end()) return;

			nowState = it->second;
			nowState->Start();
			nowState->chengeNext(false);	//
		}
	}

	// “o˜^‚·‚é
	void Register(const std::string& name, const std::shared_ptr<StateBase> state)
	{
		states.insert(std::make_pair(name, state));
		std::cout << "StateMachine : " << name << "‚ğ“o˜^" << std::endl;
	}

	// Å‰‚©‚çn‚ß‚éó‘Ô‚ğİ’è
	void SetStartState(const std::string& registerName)
	{
		auto it = states.find(registerName);
		if (it == states.end()) return;

		std::cout << "StateMachine : " << it->first << "‚ğƒXƒ^[ƒgó‘Ô‚Éİ’è" << std::endl;

		nowState = it->second;
		nowState->Start();

	}

	// “o˜^‚µ‚½‚à‚Ì‚ğíœ‚·‚é
	void Deregistration(const std::string& registerName)
	{
		auto it = states.find(registerName);
		if (it == states.end()) return;

		std::cout << "StateMachine : " << it->first << "‚ğ“o˜^‚ğíœ‚·‚é" << std::endl;

		states.erase(it);
	}

	// ‚·‚×‚Ä‚Ì“o˜^‚ğíœ‚·‚é
	void AllDeregistration()
	{
		std::cout << "StateMachine : " << "‚·‚×‚Ä‚Ì“o˜^‚ğíœ‚·‚é" << std::endl;

		states.clear();
	}

private:
	std::unordered_map<std::string, std::shared_ptr<StateBase>> states;
	std::shared_ptr<StateBase> nowState;
};

