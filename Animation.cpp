#include "Animation.h"

Animation::Animation() : frames(0), currentFrameIndex(0), currentFrameTime(0.f), releaseFirstFrame(true), isLooped(true), isDoneAnimating(false)
{

}
void Animation::AddFrame(int textureID, int x, int y, int width, int height, float frameTime)
{
	FrameData data{textureID, x, y, width, height, frameTime}; //Changed initializing each variable individually to when it is constructed i.e. data.x = x
	frames.push_back(data);
}
void Animation::AddFrameAction(unsigned int frame, AnimationAction action)
{
	// If the frame is larger than the number of animation frames then this request is ignored.
	if (frame < frames.size())
	{
		auto actionKey = actions.find(frame);
		if (actionKey == actions.end())
		{
			//We set the bit at the frame position wheneverwe first add an action at that position.
			framesWithActions.SetBit(frame); 

			// If there is not an existing entry for this frame we create one.
			actions.insert(std::make_pair(frame, std::vector<AnimationAction>{action}));
		}
		else
		{
			// An existing entry was found so we add the action to the vector
			actionKey->second.emplace_back(action);
		}
	}
}
const FrameData* Animation::GetCurrentFrame() const
{
	if (frames.size() > 0)
	{
		return &frames[currentFrameIndex];
	}
	return nullptr;
}
bool Animation::UpdateFrame(float deltaTime)
{
	//TODO: Find another way to do this
	if (releaseFirstFrame)
	{
		RunActionForCurrentFrame();
		releaseFirstFrame = false;
		return true;
	}
	if (frames.size() > 1 && (isLooped || currentFrameIndex < frames.size() - 1))
	{
		currentFrameTime += deltaTime;
		if (currentFrameTime >= frames[currentFrameIndex].displayTimeSeconds)
		{
			currentFrameTime = 0.f;
			IncrementFrame();
			RunActionForCurrentFrame();
			return true;
		}
	}
	else 
	{
		isDoneAnimating = true;
	}
	return false;
}
void Animation::Reset()
{
	currentFrameIndex = 0;
	currentFrameTime = 0.f;
	releaseFirstFrame = true;
	isDoneAnimating = false;
}
//Private
void Animation::IncrementFrame()
{
	currentFrameIndex = (currentFrameIndex + 1) % frames.size();
}
void Animation::RunActionForCurrentFrame()
{
	if (actions.size() > 0)
	{
		if (framesWithActions.GetBit(currentFrameIndex))
		{
			auto actionsToRun = actions.at(currentFrameIndex);
			for (auto f : actionsToRun)
			{
				f();
			}
			
		}
	}
}
void Animation::SetLooped(bool looped)
{
	isLooped = looped;
}
bool Animation::IsLooped()
{
	return isLooped;
}

bool Animation::IsDoneAnimating()
{
	return isDoneAnimating;
}