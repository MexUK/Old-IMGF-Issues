#include "CWindowManager.h"
#include "GUI/CGUIManager.h"
#include "GUI/Window/CWindow.h"
#include "GUI/SCreens/CIMGScreen.h"

void					CWindowManager::init(void)
{
}

void					CWindowManager::uninit(void)
{
}

// window open/close
CWindow*				CWindowManager::openWindow(void)
{
	CVector2i32 vecWindowPosition = CVector2i32(150, 150);
	CVector2ui32 vecWindowSize = CVector2ui32(1025, 698);
	return CGUIManager::getInstance()->addTemplatedTabbedWindow<CIMGScreen>(vecWindowPosition, vecWindowSize);
}

// window processing
void					CWindowManager::processWindows(void)
{
	CGUIManager::getInstance()->processWindows();
}