#ifndef CGUIManager_H
#define CGUIManager_H

#include "CManager.h"
#include "Pool/CVectorPool.h"
#include "GUI/Window/CTabbedWindow.h"
#include "CSingleton.h"
#include <Commctrl.h>

LRESULT CALLBACK				WndProc_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HBRUSH							createSolidBrush2(COLORREF colour, LPNMCUSTOMDRAW item);

class CGUIManager : public CManager, public CSingleton<CGUIManager>, public CVectorPool<CTabbedWindow*>
{
public:
	CGUIManager(void);

	void						init(void);
	void						uninit(void);
	
	CWindow*					addWindow(CVector2i32& vecWindowPosition = CVector2i32(0,0), CVector2ui32& vecWindowSize = CVector2ui32(800,600));
	CTabbedWindow*				addTabbedWindow(CVector2i32& vecWindowPosition = CVector2i32(0, 0), CVector2ui32& vecWindowSize = CVector2ui32(800, 600));

	template <class WindowClass>
	WindowClass*				addTemplatedTabbedWindow(CVector2i32& vecWindowPosition = CVector2i32(0, 0), CVector2ui32& vecWindowSize = CVector2ui32(800, 600));

	void						processWindows(void);

	void						onMouseDown(CVector2ui32& vecCursorPosition);
	void						onMouseUp(CVector2ui32& vecCursorPosition);
	void						onMouseMove(CVector2ui32& vecCursorPosition);
	void						onDoubleLeftClick(CVector2ui32& vecCursorPosition);
	void						onCharDown(uint8 uiCharCode);
	void						onKeyDown(uint8 uiCharCode);
	void						onKeyUp(void);

	void						render(void);
	void						onRender(void);
	void						clearBackground(void);

	void						setActiveWindow(CTabbedWindow *pActiveWindow) { m_pActiveWindow = pActiveWindow; }
	CTabbedWindow*				getActiveWindow(void) { return m_pActiveWindow; }

private:
	bool						createWindow(CWindow *pWindow);

private:
	CTabbedWindow*				m_pActiveWindow;
};

template <class WindowClass>
WindowClass*					CGUIManager::addTemplatedTabbedWindow(CVector2i32& vecWindowPosition, CVector2ui32& vecWindowSize)
{
	WindowClass *pWindow = new WindowClass;
	pWindow->setPosition(CVector2ui32(vecWindowPosition.m_x, vecWindowPosition.m_y)); // todo - send directly
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