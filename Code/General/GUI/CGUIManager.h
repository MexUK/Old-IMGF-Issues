#ifndef CGUIManager_H
#define CGUIManager_H

#include "CManager.h"
#include "Pool/CVectorPool.h"
#include "GUI/Window/CWindow.h"
#include "Event/CEventBinder.h"
#include "CSingleton.h"
#include <Commctrl.h>

#ifndef RGBA
#define RGBA(r,g,b,a)        ((COLORREF)( (((DWORD)(BYTE)(a))<<24) |     RGB(r,g,b) ))
#endif

LRESULT CALLBACK				WndProc_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class CGraphicsLibrary;
class CGUIStyles;

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

	CWindow*					getWindowByHwnd(HWND hWnd);

	static CGUIStyles*			createStyles(void);

	void						setGraphicsLibrary(CGraphicsLibrary* pGraphicsLibrary) { m_pGraphicsLibrary = pGraphicsLibrary; }
	CGraphicsLibrary*			getGraphicsLibrary(void) { return m_pGraphicsLibrary; }

	void						setActiveWindow(CWindow *pActiveWindow) { m_pActiveWindow = pActiveWindow; }
	CWindow*					getActiveWindow(void) { return m_pActiveWindow; }

	void						setThemeDesignerModeEnabled(bool bThemeDesignerModeEnabled) { m_bThemeDesignerModeEnabled = bThemeDesignerModeEnabled; }
	bool						isThemeDesignerModeEnabled(void) { return m_bThemeDesignerModeEnabled; }

private:
	bool						createWindow(CWindow *pWindow);

private:
	CGraphicsLibrary*			m_pGraphicsLibrary;
	CWindow*					m_pActiveWindow;
	uint8						m_bThemeDesignerModeEnabled		: 1;
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