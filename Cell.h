#pragma once

class Cell
{
public:
	Cell();
	Cell(float x, float y, float w, float h);
	virtual bool Click(float x, float y) = 0;
protected:
	float X;
	float Y;
	float Width;
	float Height;
};
