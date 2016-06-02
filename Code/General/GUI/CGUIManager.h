#ifndef CGUIManager_H
#define CGUIManager_H

#include "CManager.h"
#include "Pool/CVectorPool.h"
#include "GUI/Window/CWindow.h"
#include "Event/CEventBinder.h"
#include "CSingleton.h"
#include <Commctrl.h>

LRESULT CALLBACK				WndProc_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HBRUSH							createSolidBrush2(COLORREF colour, LPNMCUSTOMDRAW item);

class CGUIManager : public CManager, public CSingleton<CGUIManager>, public CVectorPool<CWindow*>, public CEventBinder
{
public:
	CGUIManager(void);

	void						init(void);
	void						uninit(void);

	void						bindEvents(void);

	CWindow*					addWindow(CVector2i32& vecWindowPosition = CVector2i32(0,0), CVector2ui32& vecWindowSize = CVector2ui32(800,600));

	template <class WindowClass>
	WindowClass*				addTemplatedWindow(CVector2i32& vecWindowPosition = CVector2i32(0, 0), CVector2ui32& vecWindowSize = CVector2ui32(800, 600));

	void						processWindows(void);

	void						onMouseMove(CVector2i32& vecCursorPosition);

	void						render(void);
	void						clearBackground(void);

	void						setActiveWindow(CWindow *pActiveWindow) { m_pActiveWindow = pActiveWindow; }
	CWindow*					getActiveWindow(void) { return m_pActiveWindow; }

	CWindow*					getWindowByHwnd(HWND hWnd);

private:
	bool						createWindow(CWindow *pWindow);

private:
	CWindow*					m_pActiveWindow;
};

template <class WindowClass>
WindowClass*					CGUIManager::addTemplatedWindow(CVector2i32& vecWindowPosition, CVector2ui32& vecWindowSize)
{
	WindowClass *pWindow = new WindowClass;
	pWindow->setPosition(CVector2i32(vecWindowPosition.m_x, vecWindowPosition.m_y)); // todo - send directly
	pWindow->setSize(vecWindowSize);
	pWindow->setTitleBarHeight(35);
	if (!createWindow(pWindow))
	{
		return nullptr;
	}
	addEntry(pWindow);
	return pWindow;
}

#endif