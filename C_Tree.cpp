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
		if (auto isMousePressedAction = dynamic_cast<IsMousePressed*>(node))
		{
			isMousePressedAction->init(owner);
		}
		if (auto isMouseDownAction = dynamic_cast<IsMouseDown*>(node))
		{
			isMouseDownAction->init(owner);
		}
		if (auto isMouseUpAction = dynamic_cast<IsMouseUp*>(node))
		{
			isMouseUpAction->init(owner);
		}
		if (auto getMoveSpeedAction = dynamic_cast<GetMoveSpeed*>(node))
		{
			getMoveSpeedAction->init(owner);
		}
		if (auto triggerEventAction = dynamic_cast<TriggerEvent*>(node))
		{
			triggerEventAction->init(owner);
		}
		if (auto isInteractingAction = dynamic_cast<IsInteracting*>(node))
		{
			isInteractingAction->init(owner);
		}
		if (auto setIsInteractingAction = dynamic_cast<SetIsInteracting*>(node))
		{
			setIsInteractingAction->init(owner);
		}
		if (auto CheckInteractionAction = dynamic_cast<CheckInteraction*>(node))
		{
			CheckInteractionAction->init(owner);
		}
		//TODO: Add if statments for any more nodes that inherit a reference to the owner of the tree component

	};

	bTree.applyVisitor(visitor);
}

void C_Tree::Update(float deltaTime)
{
	if (_startTree) 
	{
		bTree.tickOnce();
		BT::StdCoutLogger logger_cout(bTree);
	}
	
}

void C_Tree::LateUpdate(float deltaTime) 
{
	startTree();
}
