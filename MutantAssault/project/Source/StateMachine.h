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

	// �A�b�v�f�[�g�̑O�ɌĂ΂��
	virtual void Start() = 0;

	// �A�b�v�f�[�g
	virtual void Update() = 0;

	// ��Ԃ��ς��Ƃ��̏���
	virtual void OnChangeEvent() = 0;

	// �؂�ւ���������擾
	bool IsNext()const { return isNext; }

	//�؂�ւ���
	void chengeNext(bool next) { isNext = next; }

	//����State�Ɉړ��ł��邩
	bool CanNextState() {
		if (IsNext()) {
			OnChangeEvent();
			return true;
		}
		return false;
	}

	//���ɍs����Ԃ�ύX
	void chengeNextState(std::string name) {
		nextRegisterName = name;
	}

	//���ɍs����Ԃ��擾
	std::string getNextName() {
		return nextRegisterName;
	}
protected:
	// ���ɍs�����߂̏���
	bool isNext;

	//����State�̖��O
	std::string nextRegisterName;

};

// ��ԊǗ�
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

	// �o�^����
	void Register(const std::string& name, const std::shared_ptr<StateBase> state)
	{
		states.insert(std::make_pair(name, state));
		std::cout << "StateMachine : " << name << "��o�^" << std::endl;
	}

	// �ŏ�����n�߂��Ԃ�ݒ�
	void SetStartState(const std::string& registerName)
	{
		auto it = states.find(registerName);
		if (it == states.end()) return;

		std::cout << "StateMachine : " << it->first << "���X�^�[�g��Ԃɐݒ�" << std::endl;

		nowState = it->second;
		nowState->Start();

	}

	// �o�^�������̂��폜����
	void Deregistration(const std::string& registerName)
	{
		auto it = states.find(registerName);
		if (it == states.end()) return;

		std::cout << "StateMachine : " << it->first << "��o�^���폜����" << std::endl;

		states.erase(it);
	}

	// ���ׂĂ̓o�^���폜����
	void AllDeregistration()
	{
		std::cout << "StateMachine : " << "���ׂĂ̓o�^���폜����" << std::endl;

		states.clear();
	}

private:
	std::unordered_map<std::string, std::shared_ptr<StateBase>> states;
	std::shared_ptr<StateBase> nowState;
};

