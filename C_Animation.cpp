#include "C_Animation.h"
#include "Object.h"

void C_Animation::Awake()
{
	sprite = owner->GetComponent<C_Sprite>();
	direction = owner->GetComponent<C_Direction>();
}

void C_Animation::Update(float deltaTime)
{
	SetAnimationDirection(direction->Get());

	if (currentAnimation.first != AnimationState::None)
	{
		bool newFrame = currentAnimation.second->UpdateFrame(deltaTime);

		if (newFrame)
		{
			const FrameData* data = currentAnimation.second->GetCurrentFrame(); //Changed Framedata& to const Framedata* to fix error
			sprite->Load(data->id); // Make sure we have the right texture loaded   
			
			sprite->SetTextureRect(data->x, data->y, data->width, data->height);
		}
	}
}

void C_Animation::AddAnimation(AnimationState state, AnimationList& animationList)
{
	animations.insert(std::make_pair(state, animationList));

	/*if (currentAnimation.first == AnimationState::None)
	{
		SetAnimationState(AnimationState::Idle);
	}*/
}

//state: AnimationState we want to add an action to, (Idle, Walk, or Projectile currently)
//dir: Direction owner is facing
//frame: An integer between 0 and the animations frame count - 1
//action: The AnimationAction to add
void C_Animation::AddAnimationAction(AnimationState state, FacingDirection dir, int frame, AnimationAction action)
{
	auto animationList = animations.find(state);

	if (animationList != animations.end()) //Only adds action if entity has animation for current state and direction
	{
		auto animation = animationList->second.find(dir);

		if (animation != animationList->second.end())
		{
			animation->second->AddFrameAction(frame, action);
		}
	}
}

void C_Animation::SetAnimationState(AnimationState state)
{
	// We only set an animation of it is not already our current animation.
	if (currentAnimation.first == state)
	{
		return;
	}
	auto animationList = animations.find(state);
	if (animationList != animations.end())
	{
		auto animation = animationList->second.find(currentDirection);

		if (animation != animationList->second.end())
		{
			currentAnimation.first = animationList->first;
			currentAnimation.second = animation->second;
			currentAnimation.second->Reset();
		}
	}
}

const AnimationState& C_Animation::GetAnimationState() const
{
	// Returns out current animation state. We can use this to compare the objects current state to a desired state.
	return currentAnimation.first;
}

void C_Animation::SetAnimationDirection(FacingDirection dir)
{
	if (dir != currentDirection)
	{
		currentDirection = dir;
		auto animationList = animations.find(currentAnimation.first);
		if (animationList != animations.end())
		{
			auto animation = animationList->second.find(currentDirection);

			if (animation != animationList->second.end())
			{
				currentAnimation.second = animation->second;
				currentAnimation.second->Reset();
			}
		}
	}
}

bool C_Animation::isAnimationDone()
{
	if (currentAnimation.first == AnimationState::None) {
		return false;
	}

	return currentAnimation.second->IsDoneAnimating();
}
