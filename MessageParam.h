#pragma once
#include <Windows.h>
#include "Field.h"

struct MessageParam
{
	POINT WinCoordinates;
	POINT GLCoordinates;
	int SelectedFieldArrNum;
	void SelectActiveField(Field** FieldArray);
};
