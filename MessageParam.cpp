#include "MessageParam.h"
#include "resource.h"

void MessageParam::SelectActiveField(Field** FieldArray)
{
	for (size_t i = 0; i < FIELDARRSIZE; i++)
	{
		if (FieldArray[i]->Click(this->GLCoordinates)) { this->SelectedFieldArrNum = i; return; }
	}
	this->SelectedFieldArrNum = -1;
}
