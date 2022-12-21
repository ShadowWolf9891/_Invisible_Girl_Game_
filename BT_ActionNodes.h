#pragma once

#ifndef BT_ActionNodes_h
#define BT_ActionNodes_h

#include "behaviortree_cpp/action_node.h"
#include <any>
#include "Event.h"
#include <stack>
#include "Object.h"
#include "C_Velocity.h"
#include "C_Events.h"
#include "C_ProjectileAttack.h"
#include "C_Observer.h"

class Object;

//Set blackboard values / get bool values
class IsOnGround : public BT::SyncActionNode
{
public:
	IsOnGround(const std::string& name) : BT::SyncActionNode(name, {}) {};

	void init(Object* owner)
	{
		this->owner = owner;
	}

	BT::NodeStatus tick() override
	{
		if (owner->GetIsOnGround()) { return BT::NodeStatus::SUCCESS; };
			
		return BT::NodeStatus::FAILURE;
	}
private:
	Object* owner;
};
class IsDoneAnimating : public BT::SyncActionNode
{
public:
	IsDoneAnimating(const std::string& name) : BT::SyncActionNode(name, {}) {};

	void init(Object* owner)
	{
		this->owner = owner;
		this->eStack = owner->GetComponent<C_Events>();
		this->cAnim = owner->GetComponent<C_Animation>();
	}

	BT::NodeStatus tick() override
	{
		if (cAnim->isAnimationDone()) 
		{
			return BT::NodeStatus::SUCCESS; 
		};
	
		return BT::NodeStatus::FAILURE;
	}
private:
	Object* owner;
	std::shared_ptr <C_Animation> cAnim;
	std::shared_ptr <C_Events> eStack;
};
class IsInteracting : public BT::SyncActionNode
{
public:
	IsInteracting(const std::string& name) : BT::SyncActionNode(name, {}) {};

	void init(Object* owner)
	{
		this->owner = owner;
		this->cInteract = owner->GetComponent<C_InteractWithObjects>();
	}

	BT::NodeStatus tick() override
	{
		if (cInteract->GetIsInteracting())
		{
			return BT::NodeStatus::SUCCESS;
		}
		return BT::NodeStatus::FAILURE;
	}
private:
	Object* owner;
	std::shared_ptr <C_InteractWithObjects> cInteract;
};
class SetIsInteracting : public BT::SyncActionNode
{
public:
	SetIsInteracting(const std::string& name, const BT::NodeConfig& config) : BT::SyncActionNode(name, config) {};

	void init(Object* owner)
	{
		this->owner = owner;
		this->cInteract = owner->GetComponent<C_InteractWithObjects>();
	}
	static BT::PortsList providedPorts()
	{
		return
		{
			BT::InputPort<bool>("isTalking")
		};
	}
	BT::NodeStatus tick() override
	{
		BT::Expected<bool> talking = getInput<bool>("isTalking");

		if (!talking) { throw BT::RuntimeError("Missing required bool [isTalking]: ", talking.error()); }

		cInteract->SetIsInteracting(talking.value());
		
		return BT::NodeStatus::SUCCESS;
		
	}
private:
	Object* owner;
	std::shared_ptr <C_InteractWithObjects> cInteract;
};
class CheckInteraction : public BT::SyncActionNode
{
public:
	CheckInteraction(const std::string& name) : BT::SyncActionNode(name, {}) {};

	void init(Object* owner)
	{
		this->owner = owner;
		this->cInteract = owner->GetComponent<C_InteractWithObjects>();
	}

	BT::NodeStatus tick() override
	{
		if (cInteract->CheckInteraction())
		{
			return BT::NodeStatus::SUCCESS;
		}
		return BT::NodeStatus::FAILURE;
	}
private:
	Object* owner;
	std::shared_ptr <C_InteractWithObjects> cInteract;
};

class IsKeyPressed : public BT::SyncActionNode
{
public:
	IsKeyPressed(const std::string& name, const BT::NodeConfig& config) : BT::SyncActionNode(name, config) {};

	void init(Object* owner)
	{
		this->owner = owner;
	}
	static BT::PortsList providedPorts()
	{
		return
		{
			BT::InputPort<std::string>("Key")
		};
	}
	BT::NodeStatus tick() override
	{
		
		BT::Expected<std::string> key = getInput<std::string>("Key");

		if (!key) { throw BT::RuntimeError("Missing required input [Key]: ", key.error()); }

		Input::Key k = strToKey.at(key.value());
	
		if (owner->context->input->IsKeyPressed(k))
		{
			return BT::NodeStatus::SUCCESS;
		}

		return BT::NodeStatus::FAILURE;
	}
private:
	Object* owner;
};
class IsKeyDown : public BT::SyncActionNode
{
public:
	IsKeyDown(const std::string& name, const BT::NodeConfig& config) : BT::SyncActionNode(name, config) {};

	void init(Object* owner)
	{
		this->owner = owner;
	}
	static BT::PortsList providedPorts()
	{
		return
		{
			BT::InputPort<std::string>("Key")
		};
	}
	BT::NodeStatus tick() override
	{
		BT::Expected<std::string> key = getInput<std::string>("Key");

		if (!key) { throw BT::RuntimeError("Missing required input [Key]: ", key.error()); }

		Input::Key k = strToKey.at(key.value());

		if (owner->context->input->IsKeyDown(k))
		{
			return BT::NodeStatus::SUCCESS;
		}

		return BT::NodeStatus::FAILURE;
	}
private:
	Object* owner;
};
class IsKeyUp : public BT::SyncActionNode
{
public:
	IsKeyUp(const std::string& name, const BT::NodeConfig& config) : BT::SyncActionNode(name, config) {};

	void init(Object* owner)
	{
		this->owner = owner;
	}
	static BT::PortsList providedPorts()
	{
		return
		{
			BT::InputPort<std::string>("Key")
		};
	}
	BT::NodeStatus tick() override
	{
		BT::Expected<std::string> key = getInput<std::string>("Key");

		if (!key) { throw BT::RuntimeError("Missing required input [Key]: ", key.error()); }

		Input::Key k = strToKey.at(key.value());

		if (owner->context->input->IsKeyUp(k))
		{
			return BT::NodeStatus::SUCCESS;
		}

		return BT::NodeStatus::FAILURE;
	}
private:
	Object* owner;
};
class IsMousePressed : public BT::SyncActionNode
{
public:
	IsMousePressed(const std::string& name, const BT::NodeConfig& config) : BT::SyncActionNode(name, config) {};

	void init(Object* owner)
	{
		this->owner = owner;
	}
	static BT::PortsList providedPorts()
	{
		return
		{
			BT::InputPort<std::string>("Mouse")
		};
	}
	BT::NodeStatus tick() override
	{

		BT::Expected<std::string> mouse = getInput<std::string>("Mouse");

		if (!mouse) { throw BT::RuntimeError("Missing required input [Mouse]: ", mouse.error()); }

		Input::Mouse k = strToMouse.at(mouse.value());

		if (owner->context->input->IsMousePressed(k))
		{
			return BT::NodeStatus::SUCCESS;
		}

		return BT::NodeStatus::FAILURE;
	}
private:
	Object* owner;
};
class IsMouseDown : public BT::SyncActionNode
{
public:
	IsMouseDown(const std::string& name, const BT::NodeConfig& config) : BT::SyncActionNode(name, config) {};

	void init(Object* owner)
	{
		this->owner = owner;
	}
	static BT::PortsList providedPorts()
	{
		return
		{
			BT::InputPort<std::string>("Mouse")
		};
	}
	BT::NodeStatus tick() override
	{
		BT::Expected<std::string> mouse = getInput<std::string>("Mouse");

		if (!mouse) { throw BT::RuntimeError("Missing required input [Mouse]: ", mouse.error()); }

		Input::Mouse k = strToMouse.at(mouse.value());

		if (owner->context->input->IsMouseDown(k))
		{
			return BT::NodeStatus::SUCCESS;
		}

		return BT::NodeStatus::FAILURE;
	}
private:
	Object* owner;
};
class IsMouseUp : public BT::SyncActionNode
{
public:
	IsMouseUp(const std::string& name, const BT::NodeConfig& config) : BT::SyncActionNode(name, config) {};

	void init(Object* owner)
	{
		this->owner = owner;
	}
	static BT::PortsList providedPorts()
	{
		return
		{
			BT::InputPort<std::string>("Mouse")
		};
	}
	BT::NodeStatus tick() override
	{
		BT::Expected<std::string> mouse = getInput<std::string>("Mouse");

		if (!mouse) { throw BT::RuntimeError("Missing required input [Mouse]: ", mouse.error()); }

		Input::Mouse k = strToMouse.at(mouse.value());

		if (owner->context->input->IsMouseUp(k))
		{
			return BT::NodeStatus::SUCCESS;
		}

		return BT::NodeStatus::FAILURE;
	}
private:
	Object* owner;
};
class GetMoveSpeed : public BT::SyncActionNode
{
public:
	GetMoveSpeed(const std::string& name, const BT::NodeConfig& config) : BT::SyncActionNode(name, config) {};

	void init(Object* owner) 
	{
		this->owner = owner;
		this->velocity = owner->GetComponent<C_Velocity>();
	}

	static BT::PortsList providedPorts()
	{
		return 
		{
			BT::OutputPort<float>("moveSpeed") 
		};
	}

	BT::NodeStatus tick() override
	{
		float speed = this->velocity->GetDirectionalVelocity();

		setOutput("moveSpeed", speed);
		
		return BT::NodeStatus::SUCCESS;
	}
private:
	std::shared_ptr<C_Velocity> velocity;
	Object* owner;
};

class TriggerEvent : public BT::StatefulActionNode
{
public:
	TriggerEvent(const std::string& name, const BT::NodeConfig& config) : BT::StatefulActionNode(name, config) {};

	void init(Object* owner)
	{
		this->owner = owner;
		this->eStack = owner->GetComponent<C_Events>();
		this->handler = owner->GetComponent<C_Observer>();
		this->cAnim = owner->GetComponent<C_Animation>();
	}
	static BT::PortsList providedPorts()
	{
		return
		{
			BT::InputPort<std::string>("EventType")
		};
	}
	BT::NodeStatus onStart() override
	{
		BT::Expected<std::string> eType = getInput<std::string > ("EventType");
		if (!eType) { throw BT::RuntimeError("Missing required input [Event Type]: ", eType.error()); }

		Event::DescriptorType descriptor = strToEvent.at(eType.value());
		curDescriptor = descriptor;
		std::string name = "Begin " + eType.value() + " Event";

		Event e = Event(e, descriptor, name);
		e.SetTargetID(owner->instanceID->Get());

		if (handler->lastEventHandledType != e.getType())
		{
			eStack->PushEvent(e);
		}

		return BT::NodeStatus::RUNNING;
	}

	BT::NodeStatus onRunning() override
	{
		if (cAnim->isAnimationDone())
		{
			if (eStack->PeekEvent().getType() == curDescriptor)
			{
				auto e = eStack->PopEvent();
				std::cout << curDescriptor << " [Done]" << std::endl;
				return BT::NodeStatus::SUCCESS;
			}
		}

		//std::cout << curDescriptor << " [Running]" << std::endl;

		return BT::NodeStatus::RUNNING;
	}
	void onHalted() override
	{
		if (eStack->PeekEvent().getType() == curDescriptor)
		{
			auto e = eStack->PopEvent();
			std::cout << curDescriptor << " [Aborted]" << std::endl;
		}

	}

private:
	Object* owner;
	std::shared_ptr <C_Events> eStack;
	std::shared_ptr <C_Observer> handler;
	std::shared_ptr <C_Animation> cAnim;
	Event::DescriptorType curDescriptor;
};

#endif //BT_ActionNodes_h
