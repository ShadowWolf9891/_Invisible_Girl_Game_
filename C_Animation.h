#pragma once
#ifndef C_Animation_h
#define C_Animation_h

#include <unordered_map>

#include "Component.h"
#include "Animation.h"
#include "C_Sprite.h"
#include "C_Direction.h"

enum class AnimationState
{
	None,
	Idle,
	Walk,
	Run,
	BowDraw,
	BowSheath,
	BowParry,
	BowDodge,
	BowHurt,
	BowDead,
	BowIdle,
	BowMove,
	BowCrouch,
	BowRetreat,
	BowLunge,
	BowAttack,
	BowShootStraight	//Probably wont be used
};

using AnimationList = std::unordered_map<FacingDirection, std::shared_ptr<Animation>>;

class C_Animation : public Component
{
public:

	C_Animation(Object* owner) : Component(owner), currentAnimation(AnimationState::None, nullptr), currentDirection(FacingDirection::Down)
	{
		
	};

	virtual ~C_Animation() = default;

	void Awake() override;

	void Update(float deltaTime) override;

	void AddAnimation(AnimationState state, AnimationList& animationList); // Add animation to object. We need its state as well so that we can switch to it.

	void AddAnimationAction(AnimationState state, FacingDirection dir, int frame, AnimationAction action);

	void SetAnimationState(AnimationState state);// Set current animation states.
	const AnimationState& GetAnimationState() const;// Returns current animation state.

	void SetAnimationDirection(FacingDirection dir);

	bool isAnimationDone();
	
private:
	std::unordered_map<AnimationState, AnimationList> animations;
	std::pair<AnimationState, std::shared_ptr<Animation>> currentAnimation;
	FacingDirection currentDirection;// We store a reference to the current animation so we can quickly update and draw it.
	std::shared_ptr<C_Sprite> sprite; //Animation component requires that an object has a sprite component
	std::shared_ptr<C_Direction> direction; //Will probably be moved
};

#endif /* C_Animation_h */