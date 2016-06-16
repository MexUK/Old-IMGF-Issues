#include "CWindowManager.h"
#include "GUI/CGUIManager.h"
#include "GUI/Window/CKGMWindow.h"
#include "GUI/Editors/CIMGEditor.h"

void					CWindowManager::init(void)
{
}

void					CWindowManager::uninit(void)
{
}

// window open/close
CWindow*				CWindowManager::openWindow(void)
{
	CPoint2D vecWindowPosition = CPoint2D(150, 150);
	CSize2D vecWindowSize = CSize2D(1025, 698);
	CKGMWindow *pWindow = CGUIManager::getInstance()->addTemplatedWindow<CKGMWindow>(vecWindowPosition, vecWindowSize);
	
	CIMGEditor *pIMGEditor = new CIMGEditor; // CIMGEditor eventually extends CGUILayer - todo use pWindow->addTempatedGroup<CIMGEditor>() instead?
	pIMGEditor->setWindow(pWindow);
	pWindow->addEntry(pIMGEditor);
	
	pWindow->initTabs();
	pWindow->bindAllEvents();
	
	return pWindow;
}

// window processing
void					CWindowManager::processWindows(void)
{
	CGUIManager::getInstance()->processWindows();
}