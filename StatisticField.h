#pragma once
#include "Field.h"
#include "resource.h" 

extern unsigned const int StatisticFieldW;
extern unsigned const int StatisticFieldH;

class StatisticField : public Field
{
public:
	StatisticField(unsigned const int StartX, unsigned const int StartY);
public:
	void Draw() override;
	bool Click(POINT& coordinates) override { return true; }
private: 
	void DrawTime(unsigned int time);
	void DrawKilledAlive(unsigned int AlivePlayer, unsigned int AliveOpponent);
};
