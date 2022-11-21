#pragma once

#ifndef C_Tree_h
#define C_Tree_h

#include "Component.h"
#include "BT_ActionNodes.h"
#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/loggers/bt_cout_logger.h"

class C_Tree : public Component
{
public:
	C_Tree(Object* owner) : Component(owner)
	{

	};
	virtual ~C_Tree() = default;

	void Start() override;

	//void SetTree(std::string xmlPath);
	
	//std::shared_ptr<BT::Tree> GetTree();

	void Update(float deltaTime) override;
	
	BT::Tree bTree;
	
};

#endif // C_Tree_h

