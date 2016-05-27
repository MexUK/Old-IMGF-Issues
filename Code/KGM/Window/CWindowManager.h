#ifndef CWindowManager_H
#define CWindowManager_H

#include "CManager.h"
#include "Pool/CVectorPool.h"
#include "CTabbedWindow.h"
#include "Main Window/CMainWindow.h"
#include <Commctrl.h>

LRESULT CALLBACK				WndProc_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HBRUSH							createSolidBrush2(COLORREF colour, LPNMCUSTOMDRAW item);

class CWindowManager : public CManager, public CVectorPool<CTabbedWindow*>
{
public:
	CWindowManager(void) :
		m_pMainWindow(nullptr),
		m_pActiveWindow(nullptr)
	{};

	void						init(void);
	void						uninit(void);
	
	CMainWindow*				addMainWindow(void);

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

	void						setMainWindow(CMainWindow *pMainWindow) { m_pMainWindow = pMainWindow; }
	CMainWindow*				getMainWindow(void) { return m_pMainWindow; }

	void						setActiveWindow(CTabbedWindow *pActiveWindow) { m_pActiveWindow = pActiveWindow; }
	CTabbedWindow*				getActiveWindow(void) { return m_pActiveWindow; }
	
private:
	CMainWindow*				m_pMainWindow;
	CTabbedWindow*				m_pActiveWindow;
};

#endif