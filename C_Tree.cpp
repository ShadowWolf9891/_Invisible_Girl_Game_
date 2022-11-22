#include "C_Tree.h"
#include "Object.h"

void C_Tree::Start()
{
	auto visitor = [this](BT::TreeNode* node)
	{
		if (auto isOnGroundAction = dynamic_cast<IsOnGround*>(node))
		{
			isOnGroundAction->init(owner);
		}
		if (auto isDoneAnimatingAction = dynamic_cast<IsDoneAnimating*>(node))
		{
			isDoneAnimatingAction->init(owner);
		}
		if (auto isKeyPressedAction = dynamic_cast<IsKeyPressed*>(node))
		{
			isKeyPressedAction->init(owner);
		}
		if (auto isKeyDownAction = dynamic_cast<IsKeyDown*>(node))
		{
			isKeyDownAction->init(owner);
		}
		if (auto isKeyUpAction = dynamic_cast<IsKeyUp*>(node))
		{
			isKeyUpAction->init(owner);
		}
		if (auto getMoveSpeedAction = dynamic_cast<GetMoveSpeed*>(node))
		{
			getMoveSpeedAction->init(owner);
		}
		if (auto triggerEventAction = dynamic_cast<TriggerEvent*>(node))
		{
			triggerEventAction->init(owner);
		}

		//TODO: Add if statments for any more nodes that inherit a reference to the owner of the tree component

	};

	bTree.applyVisitor(visitor);
}

void C_Tree::Update(float deltaTime)
{
	bTree.tickOnce();
	BT::StdCoutLogger logger_cout(bTree);
}
