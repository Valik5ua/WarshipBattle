#pragma once
#include "Field.h"

extern unsigned const int SoundButtonW;
extern unsigned const int SoundButtonH;

class SoundButton : public Field
{
public:
	SoundButton(const unsigned int StartX, const unsigned int StartY);
public:
	bool Click(POINT& coordinates) override;
	void Draw() override;
public:
	enum STATE { On, Off } State;
};
