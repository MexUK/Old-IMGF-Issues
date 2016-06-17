#include "CGUIManager.h"
#include "String/CStringUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/GraphicsLibraries/CGraphicsLibrary_GDIPlus.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "GUI/Controls/CButtonControl.h"
#include "GUI/Controls/CListControl.h"
#include "GUI/CGUIUtility.h" // temp
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include <Windowsx.h>

using namespace std;

bool g_bWindowRenderHasOccurred = false; // temp

auto pOnMouseMove_GUIManager		= [](void *pGUIManager, void *pTriggerArg) { ((CGUIManager*) pGUIManager)->onMouseMove(*(CPoint2D*) pTriggerArg); };

CGUIManager::CGUIManager(void) :
	m_pGraphicsLibrary(nullptr),
	m_pActiveWindow(nullptr),
	m_bThemeDesignerModeEnabled(false)
{
	m_pGraphicsLibrary = new CGraphicsLibrary_GDIPlus;
}

void						CGUIManager::init(void)
{
	getGraphicsLibrary()->init();
	CEventManager::getInstance()->bindEvent(EVENT_onToolReady, [](void *pData) { ((CGUIManager*) pData)->bindEvents(); }, this);
}

void						CGUIManager::uninit(void)
{
	unbindEvents();
	getGraphicsLibrary()->uninit();
}

// event binding
void						CGUIManager::bindEvents(void)
{
	storeEventBoundFunction(getEntryByIndex(0)->bindEvent(EVENT_onMouseMove, pOnMouseMove_GUIManager, this, -1000000)); // bind first
	storeEventBoundFunction(getEntryByIndex(1)->bindEvent(EVENT_onMouseMove, pOnMouseMove_GUIManager, this, -1000000)); // bind first
}

// serialization
void						CGUIManager::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	uint32 uiWindowCount = pDataReader->readUint32();
	for (uint32 i = 0; i < uiWindowCount; i++)
	{
		CWindow *pWindow = new CWindow;
		pWindow->unserialize();
		addEntry(pWindow);
	}
}

void						CGUIManager::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->writeUint32(getEntryCount()); // window count
	for (CWindow *pWindow : getEntries())
	{
		pWindow->serialize();
	}
}

// add window
CWindow*					CGUIManager::addWindow(CPoint2D& vecWindowPosition, CSize2D& vecWindowSize)
{
	CWindow *pWindow = new CWindow;
	pWindow->setPosition(vecWindowPosition);
	pWindow->setSize(vecWindowSize);
	pWindow->setTitleBarHeight(35);
	if (!createWindow(pWindow))
	{
		return nullptr;
	}
	addEntry(pWindow);
	return pWindow;
}

// create window (internal)
bool						CGUIManager::createWindow(CWindow *pWindow)
{
	char *szClassName = "KGM_Window";
	HINSTANCE hInstance = GetModuleHandle(NULL);

	// register the window
	if (getEntryCount() == 0)
	{
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_DBLCLKS;
		wc.lpfnWndProc = WndProc_Window;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = szClassName;
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		if (!RegisterClassEx(&wc))
		{
			pWindow->unload();
			delete pWindow;
			return false;
		}
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
void						CGUIManager::processWindows(void)
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
	CGUIManager
		*pGUIManager = CGUIManager::getInstance();
	CWindow
		*pWindow = pGUIManager->getWindowByHwnd(hwnd);

	if (!pWindow)
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	CEventManager
		*pEventManager = CEventManager::getInstance();
	uint32
		uiEventTypeId,
		uiEventTypeIndex;
	eEvent
		eEventId = EVENT_UNKNOWN;
	void
		*pTriggerArgument = nullptr;

	if (pWindow == pGUIManager->getEntryByIndex(1) && pGUIManager->isThemeDesignerModeEnabled()) // check if event is for main window and if theme designer mode is enabled
	{
 		uiEventTypeId = EVENT_TYPE_THEME_DESIGNER;
	}
	else
	{
		uiEventTypeId = EVENT_TYPE_WINDOW;
	}
	uiEventTypeIndex = (uint32) pWindow->getWindowHandle();

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		eEventId = EVENT_onLeftMouseDown;
		pTriggerArgument = &CPoint2D(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONUP:
		eEventId = EVENT_onLeftMouseUp;
		pTriggerArgument = &CPoint2D(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_MOUSEMOVE:
		eEventId = EVENT_onMouseMove;
		pTriggerArgument = &CPoint2D(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONDBLCLK:
		eEventId = EVENT_onLeftMouseDoubleClick;
		pTriggerArgument = &CPoint2D(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_KEYDOWN:
		eEventId = EVENT_onKeyDown;
		pTriggerArgument = &wParam;
		break;
	case WM_KEYUP:
		eEventId = EVENT_onKeyUp;
		pTriggerArgument = &wParam;
		break;
	case WM_CHAR:
		eEventId = EVENT_onCharacterDown;
		pTriggerArgument = &wParam;
		break;
	case WM_PAINT:
		pWindow->onRenderFromWMPaint();
		break;
	case WM_ERASEBKGND:
		return 1;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	if (eEventId != EVENT_UNKNOWN)
	{
		pEventManager->triggerEvent(uiEventTypeId, uiEventTypeIndex, eEventId, pTriggerArgument);
		
		// redraw each window if needed
		for (CWindow *pWindow2 : pGUIManager->getEntries())
		{
			pWindow2->checkToRender();
		}
	}

	pWindow->setMarkedToRedraw(false);
	pEventManager->setEventHogged(false);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void						CGUIManager::onMouseMove(CPoint2D& vecCursorPosition)
{
	CEventManager *pEventManager = CEventManager::getInstance();

	// store cursor position relative to window - latest and previous
	pEventManager->setPreviousCursorPosition(pEventManager->getLatestCursorPosition());
	pEventManager->setLatestCursorPosition(vecCursorPosition);

	// store cursor position relative to screen - latest and previous
	POINT point;
	GetCursorPos(&point);
	CPoint2D vecPoint(point.x, point.y);
	pEventManager->setPreviousScreenCursorPosition(pEventManager->getLatestScreenCursorPosition());
	pEventManager->setLatestScreenCursorPosition(vecPoint);

	// store cursor move difference relative to screen - latest
	pEventManager->setScreenCursorMoveDifference(vecPoint - pEventManager->getPreviousScreenCursorPosition());
}

// window render
void						CGUIManager::render(void)
{
	for (CWindow *pWindow : getEntries())
	{
		pWindow->checkToRender();
	}
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

// styles
CGUIStyles*					CGUIManager::createStyles(void)
{
	return new CGUIStyles;
}