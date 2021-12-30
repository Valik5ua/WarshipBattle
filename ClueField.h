#pragma once
#include "Field.h"

class ClueField : public Field
{
public:
	ClueField(const unsigned int StartX, const unsigned int StartY);
	void Draw() override;
	bool Click(POINT& coordinates) override { return false; }
public:
	float startX;
	float startY;
private:
	struct SearchingIcon
	{
		float StartX;
		float MaxPosX;
		bool Direction;
	} searchingIcon;

	struct DisconnectingDots
	{
		float StartX;
		unsigned short int NumOfDots;
		const unsigned short int MaxNumOfDots = 3;
		unsigned short int FrameCounter;
		const unsigned short int MaxFrames = 30;
	} disconnectingDots;
};
