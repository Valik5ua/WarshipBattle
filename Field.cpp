#include "Field.h"
#include "resource.h"
#include "GL/glew.h"
#include "GL/freeglut.h"

Field::Field(bool DV, bool DH) :Draw_Vertical(DV), Draw_Horizontal(DH)
{
}

bool Field::Click(float x, float y)
{
    return false;
}

void Field::Draw()
{
    if (Draw_Vertical)
    {
        for (int i{}; i < Width; i++)
        {
            glBegin(GL_LINES);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex2f(0, i);
            glVertex2f(Height, i);
        }
    }
    if (Draw_Horizontal)
    {
        for (int i{}; i < Height; i++)
        {
            glBegin(GL_LINES);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex2f(i, 0);
            glVertex2f(i, Width);
        }
    }
}
