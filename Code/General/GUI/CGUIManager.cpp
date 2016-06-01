#include "CGUIManager.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "String/CStringUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/Controls/CWindowControl_Button.h"
#include "GUI/Controls/CWindowControl_List.h"
#include "GUI/CGUIUtility.h" // temp
#include <Windowsx.h>

using namespace std;

bool g_bWindowRenderHasOccurred = false; // temp

CGUIManager::CGUIManager(void) :
	m_pActiveWindow(nullptr)
{
}

void						CGUIManager::init(void)
{
}

void						CGUIManager::uninit(void)
{
}

// add window
CWindow*					CGUIManager::addWindow(CVector2i32& vecWindowPosition, CVector2ui32& vecWindowSize)
{
	/*
	todo
	CWindow *pWindow = new CWindow;
	pWindow->setPosition(CVector2ui32(150, 150));
	pWindow->setSize(CVector2ui32(1025, 698));
	pWindow->setTitleBarHeight(35);
	addEntry(pWindow);
	if (!createWindow(pWindow))
	{
		return nullptr;
	}
	return pWindow;
	*/
	return nullptr;
}

CTabbedWindow*				CGUIManager::addTabbedWindow(CVector2i32& vecWindowPosition, CVector2ui32& vecWindowSize)
{
	CTabbedWindow *pTabbedWindow = new CTabbedWindow;
	pTabbedWindow->setPosition(CVector2ui32(vecWindowPosition.m_x, vecWindowPosition.m_y)); // todo - send directly
	pTabbedWindow->setSize(vecWindowSize);
	pTabbedWindow->setTitleBarHeight(35);
	if (!createWindow(pTabbedWindow))
	{
		return nullptr;
	}
	addEntry(pTabbedWindow);
	return pTabbedWindow;
}

// create window (internal)
bool						CGUIManager::createWindow(CWindow *pWindow)
{
	// register the window
	WNDCLASSEX wc = { 0 };

	char *szClassName = "KGM_Window";
	HINSTANCE hInstance = GetModuleHandle(NULL);

	wc.cbSize = sizeof(wc);
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WndProc_Window;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wc))
	{
		pWindow->unload();
		delete pWindow;
		return false;
	}

	// create the window
	HWND hwndWindow = CreateWindowEx(
		0,
		szClassName,
		"KGM",
		WS_VISIBLE,// todo | WS_MAXIMIZE,
		pWindow->getPosition().m_x, pWindow->getPosition().m_y, pWindow->getSize().m_x, pWindow->getSize().m_y,
		NULL, NULL, hInstance, NULL);
	if (hwndWindow == NULL)
	{
		pWindow->unload();
		delete pWindow;
		return false;
	}
	pWindow->setWindowHandle(hwndWindow);

	// remove title bar and border
	SetWindowLong(hwndWindow, GWL_STYLE, 0);

	// show the window
	ShowWindow(hwndWindow, SW_SHOW);

	return true;
}

// process windows
void					CGUIManager::processWindows(void)
{
	MSG msg;
	
	PeekMessage(&msg, 0, WM_USER, WM_USER, PM_NOREMOVE);

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

// window input
LRESULT CALLBACK			WndProc_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CGUIManager *pGUIManager = CGUIManager::getInstance();
	// todo CWindow *pWindow = getWindowByHwnd(hwnd);
	CWindow *pWindow = pGUIManager->getEntryByIndex(0);
	if (!pWindow)
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		pGUIManager->onMouseDown(CVector2ui32(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_LBUTTONUP:
		pGUIManager->onMouseUp(CVector2ui32(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_MOUSEMOVE:
		pGUIManager->onMouseMove(CVector2ui32(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_LBUTTONDBLCLK:
		pGUIManager->onDoubleLeftClick(CVector2ui32(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_KEYDOWN:
		pGUIManager->onKeyDown(wParam);
		break;
	case WM_KEYUP:
		pGUIManager->onKeyUp();
		break;
	case WM_CHAR:
		pGUIManager->onCharDown(wParam);
		break;
	case WM_PAINT:
		// todo if (pGUIManager->getMainWindow() != nullptr)
		//{
			pGUIManager->onRender();
		//}
		break;
	case WM_SIZE:
		/*
		todo
		if (pGUIManager->getMainWindow() != nullptr)
		{
			pGUIManager->getMainWindow()->setSize(CVector2ui32(LOWORD(lParam), HIWORD(lParam)));
			pGUIManager->getMainWindow()->setMarkedToRedraw(true);
			pGUIManager->render();
		}
		*/
		break;
	case WM_NCCALCSIZE:
	{
		/*
		if (wParam == FALSE)
		{
		RECT *pRect = (RECT*)lParam;
		pRect->left = vecMainWindowNextPosition.m_x;
		pRect->top = vecMainWindowNextPosition.m_y;
		return WVR_REDRAW;
		}
		break;
		*/
	}
	case WM_ERASEBKGND:
		if (pGUIManager->getEntryCount() > 0) // todo
		{
			//pMainWindowManager->clearBackground();
		}
		return 1;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void					CGUIManager::onMouseDown(CVector2ui32& vecCursorPosition)
{
	for (CWindow *pWindow : getEntries())
	{
		pWindow->onMouseDown(vecCursorPosition);
	}

	CEventManager::getInstance()->setEventHogged(false);

	render();
}

void					CGUIManager::onMouseUp(CVector2ui32& vecCursorPosition)
{
	for (CWindow *pWindow : getEntries())
	{
		pWindow->onMouseUp(vecCursorPosition);
	}

	CEventManager::getInstance()->setEventHogged(false);

	render();
}

void					CGUIManager::onMouseMove(CVector2ui32& vecCursorPosition)
{
	for (CWindow *pWindow : getEntries())
	{
		pWindow->onMouseMove(vecCursorPosition);
	}

	render();

	CEventManager::getInstance()->setEventHogged(false);
	CEventManager::getInstance()->setLastCursorPosition(vecCursorPosition);
}

void					CGUIManager::onDoubleLeftClick(CVector2ui32& vecCursorPosition)
{
	CEventManager::getInstance()->setEventHogged(false);
}

void					CGUIManager::onCharDown(uint8 uiCharCode)
{
	for (CWindow *pWindow : getEntries())
	{
		pWindow->onCharDown(uiCharCode);
	}

	render();

	CEventManager::getInstance()->setEventHogged(false);
}

void					CGUIManager::onKeyDown(uint8 uiCharCode)
{
	for (CWindow *pWindow : getEntries())
	{
		pWindow->onKeyDown(uiCharCode);
	}

	render();

	CEventManager::getInstance()->setEventHogged(false);
}

void					CGUIManager::onKeyUp(void)
{
	CEventManager::getInstance()->setEventHogged(false);
}

// window render
void					CGUIManager::render(void)
{
	bool bMarkedToRedraw = getEntryByIndex(0)->isMarkedToRedraw();

	if (bMarkedToRedraw)
	{
		//UpdateWindow(m_pMainWindow->getWindowHandle());
		//RedrawWindow(m_pMainWindow->getWindowHandle(), NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);

		RedrawWindow(getEntryByIndex(0)->getWindowHandle(), NULL, NULL, RDW_INVALIDATE);

		/*
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = 1025;
		rect.bottom = 698;
		RedrawWindow(m_pMainWindow->getWindowHandle(), &rect, NULL, RDW_INVALIDATE);
		*/
	}

	onRender();
}

void					CGUIManager::onRender(void)
{
	// prepare
	RECT rect;
	GetClientRect(getEntryByIndex(0)->getWindowHandle(), &rect);

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(getEntryByIndex(0)->getWindowHandle(), &ps);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);

	HANDLE hOld = SelectObject(hdcMem, hbmMem);

	// store hdc
	// todo m_pMainWindow->setHDC(hdcMem);
	for (CWindow *pWindow : getEntries())
	{
		pWindow->setHDC(hdcMem);
	}
	CGDIPlusUtility::setHDC(hdcMem);

	// clear background
	clearBackground(); // todo - move to like CGDIPlusUtility::clearRect(CVector4ui32(x,y,w,h), CVector3ui8(r,g,b));

	// render to memory
	for (CWindow *pWindow : getEntries())
	{
		pWindow->render();
	}

	// render to screen
	BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);

	// finalize
	SelectObject(hdcMem, hOld);
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);

	EndPaint(getEntryByIndex(0)->getWindowHandle(), &ps);

	getEntryByIndex(0)->setMarkedToRedraw(false);
}

void					CGUIManager::clearBackground(void)
{
	RECT clientRect;
	GetClientRect(getEntryByIndex(0)->getWindowHandle(), &clientRect);
	HBRUSH bkgBrush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(getEntryByIndex(0)->getHDC(), &clientRect, bkgBrush);
	DeleteObject(bkgBrush);
}

// utility
HBRUSH					createSolidBrush2(COLORREF colour, LPNMCUSTOMDRAW item) // todo - move to CGDIPlusUtility?
{
	HBRUSH Brush = NULL;
	HDC hdcmem = CreateCompatibleDC(item->hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(item->hdc, item->rc.right - item->rc.left, item->rc.bottom - item->rc.top);
	SelectObject(hdcmem, hbitmap);

	Brush = CreateSolidBrush(colour);
	RECT temp;
	temp.left = 0;
	temp.top = 0;
	temp.right = item->rc.right - item->rc.left;
	temp.bottom = item->rc.bottom;
	FillRect(hdcmem, &temp, Brush);
	DeleteObject(Brush);

	HBRUSH hPattern = CreatePatternBrush(hbitmap);

	DeleteDC(hdcmem);
	DeleteObject(Brush);
	DeleteObject(hbitmap);

	return hPattern;
}