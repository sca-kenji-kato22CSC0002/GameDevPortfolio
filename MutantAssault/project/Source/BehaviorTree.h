#pragma once
#include "DxLib.h"
#include <vector>
#include <memory>

//状態
enum class NodeStatus { Success, Failure, Running };

//基底ノード
class BTNode {
public:
	virtual NodeStatus Tick() = 0;
	virtual ~BTNode() {}
};

//セレクター
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

//シーケンス
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