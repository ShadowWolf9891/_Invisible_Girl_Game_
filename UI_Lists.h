#pragma once

#ifndef UI_Lists_h
#define UI_Lists_h

#include "BaseUserInterface.h"


class UI_VerticleList : public BaseUserInterface
{
public:

	UI_VerticleList(SharedContext* context) : BaseUserInterface(context), childSpacing(20){};

	void UpdateUI() override
	{
		if (GetSomethingChanged())
		{
			if (!GetChildren().empty())
			{
				float totalChildSizeY = 0;

				for (auto& child : GetChildren())
				{
					child->SetAnchor(Anchor::TOPMID);
					totalChildSizeY += child->GetScaledSize().y;
					child->SetOffset(0, totalChildSizeY + GetChildSpacing() );

				}
			}
		}
	};

	void SetChildSpacing(float spacing) { childSpacing = spacing; };
	float GetChildSpacing() { return childSpacing; };


private:

	float childSpacing;

};

class UI_HorizontalList : public BaseUserInterface
{
public:

	UI_HorizontalList(SharedContext* context) : BaseUserInterface(context), childSpacing(20) {};

	void UpdateUI() override
	{

		if (!GetChildren().empty())
		{
			float totalChildSizeX = 0;

			for (auto& child : GetChildren())
				{
				child->SetAnchor(Anchor::MIDLEFT);
				totalChildSizeX += child->GetScaledSize().x;
				child->SetOffset(totalChildSizeX + GetChildSpacing(), 0);
			}
		}
	};

	void SetChildSpacing(float spacing) { childSpacing = spacing; };
	float GetChildSpacing() { return childSpacing; };



private:

	float childSpacing;

};




#endif //UI_Lists_h
