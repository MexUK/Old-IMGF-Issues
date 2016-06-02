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

auto pOnMouseMove_GUIManager		= [](void *pGUIManager, void *pTriggerArg) { ((CGUIManager*) pGUIManager)->onMouseMove(*(CVector2i32*) pTriggerArg); };

CGUIManager::CGUIManager(void) :
	m_pActiveWindow(nullptr)
{
}

void						CGUIManager::init(void)
{
	bindEvents();
}

void						CGUIManager::uninit(void)
{
	unbindEvents();
}

// event binding
void						CGUIManager::bindEvents(void)
{
	storeEventBoundFunction(CEventManager::getInstance()->bindEvent(EVENT_onMouseMove, pOnMouseMove_GUIManager, this, 1000));
}

// add window
CWindow*					CGUIManager::addWindow(CVector2i32& vecWindowPosition, CVector2ui32& vecWindowSize)
{
	CWindow *pWindow = new CWindow;
	pWindow->setPosition(vecWindowPosition);
	pWindow->setSize(vecWindowSize);
	pWindow->setTitleBarHeight(35);
	addEntry(pWindow);
	if (!createWindow(pWindow))
	{
		return nullptr;
	}
	return pWindow;
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
	CWindow *pWindow = pGUIManager->getWindowByHwnd(hwnd);
	if (!pWindow)
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		pWindow->triggerEvent(EVENT_onLeftMouseDown, &CVector2ui32(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		pWindow->render();
		break;
	case WM_LBUTTONUP:
		pWindow->triggerEvent(EVENT_onLeftMouseUp, &CVector2ui32(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		pWindow->render();
		break;
	case WM_MOUSEMOVE:
		pWindow->triggerEvent(EVENT_onMouseMove, &CVector2ui32(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		pWindow->render();
		break;
	case WM_LBUTTONDBLCLK:
		pWindow->triggerEvent(EVENT_onLeftMouseDoubleClick, &CVector2ui32(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		pWindow->render();
		break;
	case WM_KEYDOWN:
		pWindow->triggerEvent(EVENT_onKeyDown, &wParam);
		pWindow->render();
		break;
	case WM_KEYUP:
		pWindow->triggerEvent(EVENT_onKeyUp, &wParam);
		pWindow->render();
		break;
	case WM_CHAR:
		pWindow->triggerEvent(EVENT_onCharacterDown, &wParam);
		pWindow->render();
		break;
	case WM_PAINT:
		pWindow->onRenderFromWMPaint();
		break;
	case WM_ERASEBKGND: // todo - needed?
		return 1;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	CEventManager::getInstance()->setEventHogged(false);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void					CGUIManager::onMouseMove(CVector2i32& vecCursorPosition)
{
	CEventManager::getInstance()->setLastCursorPosition(vecCursorPosition);
}

// window render
void					CGUIManager::render(void)
{
	for (CWindow *pWindow : getEntries())
	{
		if (pWindow->isMarkedToRedraw())
		{
			pWindow->render();
		}
	}
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
CWindow*					CGUIManager::getWindowByHwnd(HWND hWnd)
{
	for (CWindow *pWindow : getEntries())
	{
		if (hWnd == pWindow->getWindowHandle())
		{
			return pWindow;
		}
	}
	return nullptr;
}

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