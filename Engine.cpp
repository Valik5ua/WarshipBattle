#include "Engine.h"
#include "resource.h"

Engine::Engine() :Mode(Connecting)
{

}

bool Engine::ConvertPixelsToGL(POINT* Pixels)
{
	Pixels->x /= m_iPixelCellSize;
	if (Pixels->x<0 || Pixels->x>OpenGLWidth) return false;
	Pixels->y /= m_iPixelCellSize;
	if (Pixels->y < 0 || Pixels->y > OpenGLHeight) return false;
	return true;
}
