#pragma once
#ifndef Animation_h
#define Animation_h

#include <vector>
#include <functional>
#include <map>
#include "Bitmask.h"
struct FrameData
{
	int id; // Texture id (retrieved from our texture allocator).
	int x; // x position of sprite in the texture.
	int y; // y position of sprite in the texture.
	int width; // Width of sprite.
	int height; // Height of sprite.
	float displayTimeSeconds; // How long to display the frame.
};

enum class FacingDirection
{
	None,
	Left,
	Right,
	Up,
	Down
};

using AnimationAction = std::function<void(void)>; //std::function is a "general purpose polymorphic function wrapper", a class that can wrap a callable element

class Animation
{
public:
	Animation();
	void AddFrame(int textureID, int x, int y, int width, int height, float frameTime); //Removed setting isLooped here. Done instead with mutator
	void AddFrameAction(unsigned int frame, AnimationAction action);
	const FrameData* GetCurrentFrame() const;
	bool UpdateFrame(float deltaTime);
	void Reset();
	void SetLooped(bool looped);
	bool IsLooped();

	bool IsDoneAnimating();
	
private:
	void IncrementFrame();
	void RunActionForCurrentFrame();

	std::vector<FrameData> frames; // Stores all frames for our animation.
	std::map<int, std::vector<AnimationAction>> actions; //int: Frame to run actions on, vector: List of animation actions, defined as function<void(void)>
	int currentFrameIndex;// Current frame.
	float currentFrameTime;// We use this to decide when to transition to the next frame.
	bool releaseFirstFrame; //Used for animation with a single frame
	bool isLooped; //If the animation loops or not
	bool isDoneAnimating; //If the animation does not loop and is done animating
	Bitmask framesWithActions; //Check if a specific frame has actions, instead of looping through all actions

};

#endif /* Animation_h */