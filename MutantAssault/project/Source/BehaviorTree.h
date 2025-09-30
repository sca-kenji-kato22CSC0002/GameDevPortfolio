#pragma once
#include "DxLib.h"
#include <vector>
#include <memory>

//���
enum class NodeStatus { Success, Failure, Running };

//���m�[�h
class BTNode {
public:
	virtual NodeStatus Tick() = 0;
	virtual ~BTNode() {}
};

//�Z���N�^�[
class Selector : public BTNode {
	std::vector<std::unique_ptr<BTNode>> children;
public:
	void AddChild(std::unique_ptr<BTNode> node) {
		children.push_back(std::move(node));
	}

	NodeStatus Tick() override {
		for (auto& child : children) {
			NodeStatus status = child->Tick();
			if (status != NodeStatus::Failure) {
				return status;
			}
		}
		return NodeStatus::Failure;
	}
};

//�V�[�P���X
class Sequence : public BTNode {
	std::vector<std::unique_ptr<BTNode>> children;
public:
	void AddChild(std::unique_ptr<BTNode> node) {
		children.push_back(std::move(node));
	}

	NodeStatus Tick() override {
		for (auto& child : children) {
			NodeStatus status = child->Tick();
			if (status != NodeStatus::Success) {
				return status;
			}
		}
		return NodeStatus::Success;
	}
};