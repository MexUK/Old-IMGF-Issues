#include "CWindowManager.h"
#include "Globals.h"
#include "CKGM.h"
#include "Main Window/CMainWindow.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "String/CStringUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "Window Types/CEntryListWindow.h"
#include "Window Controls/Controls/CWindowControl_Button.h"
#include "Window Controls/Controls/CWindowControl_List.h"
#include "Task/CTaskManager.h"
#include "Task/CTaskDispatchManager.h"
#include "Sort/CSortManager.h"
#include "CSettingsManager.h"
#include "GUI/CGUIUtility.h" // temp
#include <Windowsx.h>

using namespace std;

bool g_bWindowRenderHasOccurred = false; // temp

void						CWindowManager::init(void)
{
}

void						CWindowManager::uninit(void)
{
}

CMainWindow*				CWindowManager::addMainWindow(void)
{
	// add main window
	CMainWindow *pMainWindow = new CMainWindow;
	pMainWindow->setPosition(CVector2ui32(150, 150));
	pMainWindow->setSize(CVector2ui32(1025, 698));
	pMainWindow->setTitleBarHeight(35);

	// register the window
	WNDCLASSEX wc = { 0 };

	wchar_t *szClassName = L"KGM_Window";
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
		pMainWindow->unload();
		delete pMainWindow;
		return nullptr;
	}

	// create the window
	HWND hwndWindow = CreateWindowEx(
		0,
		szClassName,
		L"KGM 1.3 Alpha",
		WS_VISIBLE,// | WS_MAXIMIZE,
		pMainWindow->getPosition().m_x, pMainWindow->getPosition().m_y, pMainWindow->getSize().m_x, pMainWindow->getSize().m_y,
		NULL, NULL, hInstance, NULL);
	if (hwndWindow == NULL)
	{
		pMainWindow->unload();
		delete pMainWindow;
		return nullptr;
	}
	pMainWindow->setWindowHandle(hwndWindow);

	// remove title bar and border
	SetWindowLong(hwndWindow, GWL_STYLE, 0);

	// show the window
	//ShowWindow(hwndWindow, SW_MAXIMIZE);
	ShowWindow(hwndWindow, SW_SHOW);

	// add inner window
	CEntryListWindow *pEntryListWindow = new CEntryListWindow;
	pEntryListWindow->setParentWindow(pMainWindow);
	pEntryListWindow->setBackgroundColour(0x214E67FF);
	pEntryListWindow->setSize(CVector2ui32(1025, 698));
	addEntry(pEntryListWindow);

	// add inner window controls
	CWindowControl_Button *pButton = new CWindowControl_Button;
	pButton->setWindow(pEntryListWindow);
	string strText = "Open";
	pButton->setText(strText);
	pButton->setPosition(CVector2ui32(38, 35 + 38));
	pButton->setSize(CVector2ui32(172, 40));
	pButton->setFillColour(0x1A3C4EFF);
	pButton->setControlId(1);
	pEntryListWindow->getControls().addEntry(pButton);

	CWindowControl_List *pList = new CWindowControl_List;
	pList->setWindow(pEntryListWindow);
	pList->setPosition(CVector2ui32(252, 35 + 87));
	pList->setSize(CVector2ui32(732, 480));
	pList->setFillColour(0xECF3FDFF);
	pList->setRowBackgroundColour1(0xFF0000FF);
	pList->setRowBackgroundColour2(0x00FF00FF);
	pList->setRowHeight(25);
	pList->setControlId(2);
	pEntryListWindow->setEntryListControl(pList);
	pEntryListWindow->getControls().addEntry(pList);

	CEventManager::getInstance()->bindEvent(EVENT_onPressButton, [](void *pData)
	{
		CWindowControl_Button *pButton = (CWindowControl_Button*) pData;
		if (pButton->getControlId() == 1)
		{
			getKGM()->getTaskManager()->getDispatch()->onRequestOpen();
			getKGM()->getWindowManager()->getMainWindow()->setMarkedToRedraw(true);
			getKGM()->getWindowManager()->render(); // todo - needed?
		}
	});

	// tmep here: should be in lambda bound to like onWindowOpen
	CGUIUtility::setParentWindowHwnd(pMainWindow->getWindowHandle());

	return pMainWindow;
}

LRESULT CALLBACK			WndProc_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CWindowManager *pWindowManager = getKGM()->getWindowManager();
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		pWindowManager->onMouseDown(CVector2ui32(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_LBUTTONUP:
		pWindowManager->onMouseUp(CVector2ui32(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_MOUSEMOVE:
		pWindowManager->onMouseMove(CVector2ui32(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_LBUTTONDBLCLK:
		pWindowManager->onDoubleLeftClick(CVector2ui32(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_KEYDOWN:
		pWindowManager->onKeyDown(wParam);
		break;
	case WM_KEYUP:
		pWindowManager->onKeyUp();
		break;
	case WM_CHAR:
		pWindowManager->onCharDown(wParam);
		break;
	case WM_PAINT:
		if (pWindowManager->getMainWindow() != nullptr)
		{
			pWindowManager->onRender();
		}
		break;
	case WM_SIZE:
		if (pWindowManager->getMainWindow() != nullptr)
		{
			pWindowManager->getMainWindow()->setSize(CVector2ui32(LOWORD(lParam), HIWORD(lParam)));
			pWindowManager->getMainWindow()->setMarkedToRedraw(true);
			pWindowManager->render();
		}
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
		if (pWindowManager->getEntryCount() > 0) // todo
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

void					CWindowManager::onMouseDown(CVector2ui32& vecCursorPosition)
{
	for (CWindow *pWindow : getEntries())
	{
		pWindow->onMouseDown(vecCursorPosition);
	}

	m_pMainWindow->onMouseDown(vecCursorPosition);
	
	CEventManager::getInstance()->setEventHogged(false);

	render();
}

void					CWindowManager::onMouseUp(CVector2ui32& vecCursorPosition)
{
	for (CWindow *pWindow : getKGM()->getWindowManager()->getEntries())
	{
		pWindow->onMouseUp(vecCursorPosition);
	}

	m_pMainWindow->onMouseUp(vecCursorPosition);
	
	CEventManager::getInstance()->setEventHogged(false);

	render();
}

void					CWindowManager::onMouseMove(CVector2ui32& vecCursorPosition)
{
	for (CWindow *pWindow : getKGM()->getWindowManager()->getEntries())
	{
		pWindow->onMouseMove(vecCursorPosition);
	}

	m_pMainWindow->onMouseMove(vecCursorPosition);

	render();

	CEventManager::getInstance()->setEventHogged(false);
	CEventManager::getInstance()->setLastCursorPosition(vecCursorPosition);
}

void					CWindowManager::onDoubleLeftClick(CVector2ui32& vecCursorPosition)
{
	m_pMainWindow->onDoubleLeftClick(vecCursorPosition);

	CEventManager::getInstance()->setEventHogged(false);
}

void					CWindowManager::onCharDown(uint8 uiCharCode)
{
	for (CWindow *pWindow : getKGM()->getWindowManager()->getEntries())
	{
		pWindow->onCharDown(uiCharCode);
	}

	render();

	CEventManager::getInstance()->setEventHogged(false);
}

void					CWindowManager::onKeyDown(uint8 uiCharCode)
{
	for (CWindow *pWindow : getKGM()->getWindowManager()->getEntries())
	{
		pWindow->onKeyDown(uiCharCode);
	}

	render();

	CEventManager::getInstance()->setEventHogged(false);
}

void					CWindowManager::onKeyUp(void)
{
	CEventManager::getInstance()->setEventHogged(false);
}

void					CWindowManager::render(void)
{
	bool bMarkedToRedraw = m_pMainWindow->isMarkedToRedraw();

	if (bMarkedToRedraw)
	{
		//UpdateWindow(m_pMainWindow->getWindowHandle());
		//RedrawWindow(m_pMainWindow->getWindowHandle(), NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);

		RedrawWindow(m_pMainWindow->getWindowHandle(), NULL, NULL, RDW_INVALIDATE);

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

void					CWindowManager::onRender(void)
{
	// prepare
	RECT rect;
	GetClientRect(m_pMainWindow->getWindowHandle(), &rect);

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_pMainWindow->getWindowHandle(), &ps);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);

	HANDLE hOld = SelectObject(hdcMem, hbmMem);

	// store hdc
	//m_pMainWindow->setHDC(hdcMem);
	for (CWindow *pWindow : getKGM()->getWindowManager()->getEntries())
	{
		pWindow->setHDC(hdcMem);
	}
	CGDIPlusUtility::setHDC(hdcMem);

	// clear background
	clearBackground(); // todo - move to like CGDIPlusUtility::clearRect(CVector4ui32(x,y,w,h), CVector3ui8(r,g,b));

	// render to memory
	m_pMainWindow->render();
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

	EndPaint(m_pMainWindow->getWindowHandle(), &ps);

	getKGM()->getWindowManager()->getEntryByIndex(0)->setMarkedToRedraw(false);
}

void					CWindowManager::clearBackground(void)
{
	RECT clientRect;
	GetClientRect(m_pMainWindow->getWindowHandle(), &clientRect);
	HBRUSH bkgBrush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(getKGM()->getWindowManager()->getEntryByIndex(0)->getHDC(), &clientRect, bkgBrush);
	DeleteObject(bkgBrush);
}

LRESULT					WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
	// process
	switch (msg)
	{
	case WM_PAINT:
		if (!g_bWindowRenderHasOccurred)
		{
			g_bWindowRenderHasOccurred = true; // todo - change postinit() stuff to happen in a different thread
			//getKGM()->postinit();
		}
		break;
	case WM_CONTEXTMENU:
	{
		int xPos = GET_X_LPARAM(lp);
		int yPos = GET_Y_LPARAM(lp);

		getKGM()->getEntryListWindow()->loadRightClickMenu(xPos, yPos);
		break;
	}
	case WM_ACTIVATE:
		if (wp != 0)
		{
			//CDebugger::log("WM_ACTIVATE");
			// when focus is given to main window, show the entry viewer window too
			/*
			if (getKGM()->getEntryViewerManager()->isWindowOpen())
			{
			::ShowWindow(getKGM()->getEntryViewerManager()->getEntryViewerWindow(), SW_RESTORE);
			::SetFocus(getKGM()->getEntryViewerManager()->getEntryViewerWindow());
			//::SetActiveWindow(getKGM()->getEntryViewerManager()->getEntryViewerWindow());
			}
			*/
		}
		break;
		/*
		case WM_WINDOWPOSCHANGED:
		// when the main window moves, move the entry viewer window - clipping it to the top right of the main window.
		// the reason for checking if getKGM() returns nullptr, is because a WM_MOVE event is fired before the objects are created.
		if (getKGM() != nullptr && getKGM()->getEntryViewerManager()->isWindowOpen())
		{
		// reposition the main window - didn't want to have to do this but then again we are actually processing the WM_WINDOWPOSCHANGED window message.
		WINDOWPOS *pWindowPos = (WINDOWPOS*)lp;
		//::SetWindowPos(getKGM()->getDialog()->GetSafeHwnd(), HWND_TOP, pWindowPos->cx, pWindowPos->cy, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		::UpdateWindow(getKGM()->getDialog()->GetSafeHwnd());
		RedrawWindow();

		// reposition thr entry viewer window
		getKGM()->getEntryViewerManager()->repositionWindow();
		}
		break;
		*/
	case WM_COMMAND:
	{
		CTaskDispatchManager *pTaskDispatchManager = getKGM()->getTaskManager()->getDispatch();
		switch (LOWORD(wp))
		{
		case 1100:
			pTaskDispatchManager->onRequestOpen();
			break;
		case 1101:
			pTaskDispatchManager->onRequestSaveLog(true, true);
			break;
		case 1102:
			pTaskDispatchManager->onRequestSaveLog(true, false);
			break;
		case 1115:
			pTaskDispatchManager->onRequestSaveLog(false, true);
			break;
		case 1116:
			pTaskDispatchManager->onRequestSaveLog(false, false);
			break;
		case 1105:
			pTaskDispatchManager->onRequestClose();
			break;
		case 1106:
			pTaskDispatchManager->onRequestCloseAll();
			break;
		case 1107:
			pTaskDispatchManager->onRequestExitTool();
			break;
		case 1108:
			pTaskDispatchManager->onRequestNew(IMG_1);
			break;
		case 1109:
			pTaskDispatchManager->onRequestNew(IMG_2);
			break;
		case 1112:
			pTaskDispatchManager->onRequestNew(IMG_3); // todo - make it unencrypted
			break;
		case 1113:
			pTaskDispatchManager->onRequestNew(IMG_3); // todo - make it encrypted
			break;
		case 1111:
			pTaskDispatchManager->onRequestSaveSession();
			break;
			/*
			todo
			case 1109:
			pTaskDispatchManager->onRequestAssociateIMGExtension();
			break;
			*/
		case 1114:
			pTaskDispatchManager->onRequestReopen();
			break;
		case 1140:
			pTaskDispatchManager->onRequestRebuild();
			break;
		case 1141:
			pTaskDispatchManager->onRequestRebuildAs();
			break;
		case 1142:
			pTaskDispatchManager->onRequestRebuildAll();
			break;
		case 1143:
			pTaskDispatchManager->onRequestConvertIMGVersion(IMG_1);
			break;
		case 1144:
			pTaskDispatchManager->onRequestConvertIMGVersion(IMG_2);
			break;
		case 1145:
			pTaskDispatchManager->onRequestConvertIMGVersion(IMG_3); // todo - make it unencrypted
			break;
		case 1146:
			pTaskDispatchManager->onRequestConvertIMGVersion(IMG_3); // todo - make it encrypted
			break;
		case 1147:
			pTaskDispatchManager->onRequestMerge();
			break;
		case 1156:
			pTaskDispatchManager->onRequestSplitSelectedEntries();
			break;
		case 1157:
			pTaskDispatchManager->onRequestSplitViaIDEFile();
			break;
		case 1158:
			pTaskDispatchManager->onRequestSplitViaTextLines();
			break;
		case 1149:
			pTaskDispatchManager->onRequestStats();
			break;
		case 1150:
			pTaskDispatchManager->onRequestTextureList();
			break;
		case 1160:
			pTaskDispatchManager->onRequestImportViaFiles();
			break;
		case 1161:
			pTaskDispatchManager->onRequestImportViaIDEFile();
			break;
		case 1169:
			pTaskDispatchManager->onRequestImportViaTextLines();
			break;
		case 1162:
			pTaskDispatchManager->onRequestExportSelected();
			break;
		case 1163:
			pTaskDispatchManager->onRequestExportViaIDEFile();
			break;
		case 1164:
			pTaskDispatchManager->onRequestExportViaTextLines();
			break;
		case 1192:
			pTaskDispatchManager->onRequestExportViaDATFile();
			break;
		case 1165:
			pTaskDispatchManager->onRequestRemoveSelected();
			break;
		case 1166:
			pTaskDispatchManager->onRequestRenameEntry();
			break;
		case 1167:
			pTaskDispatchManager->onRequestSelectAll();
			break;
		case 1168:
			pTaskDispatchManager->onRequestSelectInverse();
			break;
		case 1550:
			pTaskDispatchManager->onRequestQuickExport();
			break;
		case 1176:
			pTaskDispatchManager->onRequestReplace();
			break;
		case 1177:
			pTaskDispatchManager->onRequestRemoveViaIDEFile();
			break;
		case 1178:
			pTaskDispatchManager->onRequestRemoveViaTextLines();
			break;
		case 1180:
			pTaskDispatchManager->onRequestFind();
			break;
		case 1181:
			pTaskDispatchManager->onRequestFind(true);
			break;
		case 1500:
			pTaskDispatchManager->onRequestNameCase(0, 0);
			break;
		case 1501:
			pTaskDispatchManager->onRequestNameCase(0, 1);
			break;
		case 1502:
			pTaskDispatchManager->onRequestNameCase(0, 2);
			break;
		case 1510:
			pTaskDispatchManager->onRequestNameCase(1, 0);
			break;
		case 1511:
			pTaskDispatchManager->onRequestNameCase(1, 1);
			break;
		case 1512:
			pTaskDispatchManager->onRequestNameCase(1, 2);
			break;
		case 1520:
			pTaskDispatchManager->onRequestNameCase(2, 0);
			break;
		case 1521:
			pTaskDispatchManager->onRequestNameCase(2, 1);
			break;
		case 1522:
			pTaskDispatchManager->onRequestNameCase(2, 2);
			break;
		case 1530:
			pTaskDispatchManager->onRequestCopyEntryData(IMG_ENTRY_ID);
			break;
		case 1531:
			pTaskDispatchManager->onRequestCopyEntryData(IMG_ENTRY_NAME);
			break;
		case 1532:
			pTaskDispatchManager->onRequestCopyEntryData(IMG_ENTRY_OFFSET);
			break;
		case 1533:
			pTaskDispatchManager->onRequestCopyEntryData(IMG_ENTRY_RESOURCETYPE);
			break;
		case 1534:
			pTaskDispatchManager->onRequestCopyEntryData(IMG_ENTRY_RWVERSION);
			break;
		case 1535:
			pTaskDispatchManager->onRequestCopyEntryData(IMG_ENTRY_SIZE);
			break;
		case 1536:
			pTaskDispatchManager->onRequestCopyEntryData(IMG_ENTRY_TYPE);
			break;
		case 1540:
			pTaskDispatchManager->onRequestShift(0);
			break;
		case 1541:
			pTaskDispatchManager->onRequestShift(1);
			break;
		case 1560:
			pTaskDispatchManager->onRequestSelectViaFileExtension();
			break;
		case 1568:
			pTaskDispatchManager->onRequestSelectViaRWVersion(0x0);
			break;
		case 1570:
			pTaskDispatchManager->onRequestVersion();
			break;
		case 1580:
			pTaskDispatchManager->onRequestSettings();
			break;
		case 1620:
			pTaskDispatchManager->onRequestMissingTextures();
			break;
		case 1621:
			pTaskDispatchManager->onRequestReplaceAllFromFolder();
			break;
		case 1551:
			pTaskDispatchManager->onRequestExportAllEntriesFromAllTabs();
			break;
		case 1552:
			pTaskDispatchManager->onRequestExportEntriesViaIDEFileFromAllTabs();
			break;
		case 1553:
			pTaskDispatchManager->onRequestExportEntriesViaTextLinesFromAllTabs();
			break;
		case 1170:
			pTaskDispatchManager->onRequestImportViaFolder();
			break;
		case 1153:
			pTaskDispatchManager->onRequestDuplicateEntries();
			break;
		case 1554:
			pTaskDispatchManager->onRequestExportAllEntriesFromAllTabsIntoMultipleFolders();
			break;
		case 1117:
			pTaskDispatchManager->onRequestOpenLast();
			break;
		case 1182:
			pTaskDispatchManager->onRequestImportViaFiles();
			break;
		case 1183:
			pTaskDispatchManager->onRequestReplace();
			break;
		case 1184:
			pTaskDispatchManager->onRequestRemoveSelected();
			break;
		case 1650:
			pTaskDispatchManager->onRequestConvertTXDToGame(PLATFORMED_GAME_PC_GTA_III);
			break;
		case 1651:
			pTaskDispatchManager->onRequestConvertTXDToGame(PLATFORMED_GAME_PC_GTA_VC);
			break;
		case 1652:
			pTaskDispatchManager->onRequestConvertTXDToGame(PLATFORMED_GAME_PC_GTA_SA);
			break;
		case 1653:
			pTaskDispatchManager->onRequestConvertTXDToGame(PLATFORMED_GAME_ANDROID_GTA);
			break;
		case 1654:
			pTaskDispatchManager->onRequestConvertTXDToGame(PLATFORMED_GAME_XBOX_GTA);
			break;
		case 1154:
			pTaskDispatchManager->onRequestDump();
			break;
		case 1118:
			pTaskDispatchManager->onRequestSessionManager();
			break;
		case 1571:
			pTaskDispatchManager->onRequestWebsite();
			break;
		case 1103:
			pTaskDispatchManager->onRequestOpenLogBasic();
			break;
		case 1104:
			pTaskDispatchManager->onRequestOpenLogExtended();
			break;
		case 1120:
			pTaskDispatchManager->onRequestOpenLogFolder();
			break;
		case 1155:
			pTaskDispatchManager->onRequestProcessLSTFile();
			break;
		case 1900:
			pTaskDispatchManager->onRequestSelectViaIDE();
			break;
		case 1350:
			pTaskDispatchManager->onRequestExportViaIPLFile();
			break;
		case 1179:
			pTaskDispatchManager->onRequestRenameIMG();
			break;
		case 1572:
			pTaskDispatchManager->onRequestUpdate();
			break;
		case 2002:
			pTaskDispatchManager->onRequestSaveIMGSignature();
			break;
		case 2003:
			pTaskDispatchManager->onRequestVerifyIMGSignature();
			break;
		case 2004:
			pTaskDispatchManager->onRequestCompareIMG();
			break;
		case 1121:
			pTaskDispatchManager->onRequestClearLogs(false);
			break;
		case 1122:
			pTaskDispatchManager->onRequestClearLogs(true);
			break;
		case 2005:
			pTaskDispatchManager->onRequestValidateAllDFFInActiveTab();
			break;
		case 2006:
			pTaskDispatchManager->onRequestValidateAllTXDInActiveTab();
			break;
		case 2201:
			pTaskDispatchManager->onRequestConvertIMGVersion(IMG_FASTMAN92);
			break;
		case 1573:
			pTaskDispatchManager->onRequestCredits();
			break;
		case 2050:
			pTaskDispatchManager->onRequestRenamer();
			break;
		case 1880:
			pTaskDispatchManager->onRequestClearRecentlyOpenedList();
			break;
		case 2051:
			pTaskDispatchManager->onRequestBuildTXD();
			break;
		case 2053:
			pTaskDispatchManager->onRequestIMGVersionSettings();
			break;
		case 1926:
			pTaskDispatchManager->onRequestLastFeatureUsed();
			break;
		case 1575:
			pTaskDispatchManager->onRequestReportIssueOrIdea();
			break;
		case 1943:
			pTaskDispatchManager->onRequestNew(IMG_FASTMAN92);
			break;
		case 1680:
			pTaskDispatchManager->onRequestCenterCOLCollisionMeshes();
			break;
		case 1681:
			pTaskDispatchManager->onRequestAlignCOLCollisionMeshesToDFFMesh();
			break;
		case 1840:
			pTaskDispatchManager->onRequestConvertDFFFileToWDRFile();
			break;
		case 2055:
			pTaskDispatchManager->onRequestTXDOrganizer();
			break;
		case 2500:
			pTaskDispatchManager->onRequestConvertWTDFileToTXDFile();
			break;
		case 2056:
			pTaskDispatchManager->onRequestDATPathsMover();
			break;
		case 2057:
			pTaskDispatchManager->onRequestDATModelList();
			break;
		case 2058:
			pTaskDispatchManager->onRequestMapMoverAndIDShifter();
			break;
		case 2059:
			pTaskDispatchManager->onRequestFindTXDMissingFromIMGFoundInIDE();
			break;
		case 2060:
			pTaskDispatchManager->onRequestFindCOLMissingFromCOLFoundInIDE();
			break;
		case 2061:
			pTaskDispatchManager->onRequestFindDFFMissingFromIMGFoundInIDE();
			break;
		case 2090:
			pTaskDispatchManager->onRequestCloneIMG();
			break;
		case 1752:
			pTaskDispatchManager->onRequestOpenIMGFolder();
			break;
		case 2081:
			pTaskDispatchManager->onRequestRemoveOrphanTexturesFromModel();
			break;
		case 2084:
			pTaskDispatchManager->onRequestNewWindow();
			break;
		case 2097:
			pTaskDispatchManager->onRequestFindDFFMissingFromIDEFoundInIPL();
			break;
		case 2062:
			pTaskDispatchManager->onRequestSortIDEAndIPLFilesByObjectId();
			break;
		case 2105:
			pTaskDispatchManager->onRequestExtractDVCAndNVCColoursIntoDFFs();
			break;
		case 2106:
			pTaskDispatchManager->onRequestExtract2DFXIntoDFFs();
			break;
		case 2091:
			pTaskDispatchManager->onRequestOrphanCOLEntriesNotInIDE();
			break;
		case 2092:
			pTaskDispatchManager->onRequestOrphanDFFEntriesNotInIDE();
			break;
		case 2093:
			pTaskDispatchManager->onRequestOrphanIMGEntriesNotInIDE();
			break;
		case 2094:
			pTaskDispatchManager->onRequestOrphanIPLEntriesNotInIDE();
			break;
		case 2095:
			pTaskDispatchManager->onRequestOrphanTXDEntriesNotInIDE();
			break;
		default:
			/*
			todo
			
			// sort
			if (getKGM()->getSortManager()->isMenuHandleUsed(LOWORD(wp)))
			{
				getKGM()->getSortManager()->onClickMenuItem(LOWORD(wp));
			}
			// recently opened files
			else if (getKGM()->getRecentlyOpenManager()->getRecentlyOpenedFilesContainer().find(LOWORD(wp)) != getKGM()->getRecentlyOpenManager()->getRecentlyOpenedFilesContainer().end())
			{
				pTaskDispatchManager->onRequestOpen2(getKGM()->getRecentlyOpenManager()->getRecentlyOpenedFilesContainer()[LOWORD(wp)]);
			}
			// sessions
			else if (getKGM()->getSessionManager()->getSessionsContainer().find(LOWORD(wp)) != getKGM()->getSessionManager()->getSessionsContainer().end())
			{
				vector<string> vecIMGPaths = CStringUtility::split(getKGM()->getSessionManager()->getSessionsContainer()[LOWORD(wp)], ";");
				for (auto strIMGPath : vecIMGPaths)
				{
					pTaskDispatchManager->onRequestOpen2(strIMGPath);
				}
			}
			// Select RW version
			else if (getKGM()->m_umapMenuItemMapping_SelectRWVersion.find(LOWORD(wp)) != getKGM()->m_umapMenuItemMapping_SelectRWVersion.end())
			{
				pTaskDispatchManager->onRequestSelectViaRWVersion(getKGM()->m_umapMenuItemMapping_SelectRWVersion[LOWORD(wp)]);
			}
			// Convert DFF to RW version
			else if (getKGM()->m_umapMenuItemMapping_ConvertDFFtoRWVersion.find(LOWORD(wp)) != getKGM()->m_umapMenuItemMapping_ConvertDFFtoRWVersion.end())
			{
				pTaskDispatchManager->onRequestConvertDFFToRWVersion(getKGM()->m_umapMenuItemMapping_ConvertDFFtoRWVersion[LOWORD(wp)]);
			}
			// convert TXD to RW version
			else if (getKGM()->m_umapMenuItemMapping_ConvertTXDtoRWVersion.find(LOWORD(wp)) != getKGM()->m_umapMenuItemMapping_ConvertTXDtoRWVersion.end())
			{
				pTaskDispatchManager->onRequestConvertTXDToRWVersion(getKGM()->m_umapMenuItemMapping_ConvertTXDtoRWVersion[LOWORD(wp)]);
			}
			// convert TXD to texture format
			else if (getKGM()->m_umapMenuItemMapping_ConvertTXDtoTextureFormat.find(LOWORD(wp)) != getKGM()->m_umapMenuItemMapping_ConvertTXDtoTextureFormat.end())
			{
				pTaskDispatchManager->onRequestConvertTXDToTextureFormat(getKGM()->m_umapMenuItemMapping_ConvertTXDtoTextureFormat[LOWORD(wp)]);
			}
			// convert COL to COL version
			else if (getKGM()->m_umapMenuItemMapping_ConvertCOLtoCOLVersion.find(LOWORD(wp)) != getKGM()->m_umapMenuItemMapping_ConvertCOLtoCOLVersion.end())
			{
				pTaskDispatchManager->onRequestConvertCOLtoCOLVersion(getKGM()->m_umapMenuItemMapping_ConvertCOLtoCOLVersion[LOWORD(wp)]);
			}
			*/
			break;
		}
		break;
	}
	case WM_DRAWITEM:
		if (LOWORD(wp) == 1) // tabs
		{
			// todo
			//getKGM()->getDialog()->OnDrawTab((LPDRAWITEMSTRUCT)lp);
		}
		break;
	case WM_CLOSE:
		if (getKGM()->getSettingsManager()->getSettingBool("RebuildConfirmationOnClose"))
		{
			if (getKGM()->getTaskManager()->getDispatch()->onRequestClose2(true))
			{
				return true;
			}
		}
		break;
	case WM_NOTIFY:
		if (LOWORD(wp) == 22)
		{
			if (((LPNMHDR)lp)->code == LVN_ITEMCHANGED)
			{
				getKGM()->getTaskManager()->getDispatch()->onRequestSearchSelection();
			}
		}
		else if (LOWORD(wp) == 37) // main list view
		{
			if (((LPNMHDR)lp)->code == LVN_BEGINDRAG)
			{
				// todo
				//getKGM()->getDialog()->OnBeginDrag();
			}
			else if (((LPNMHDR)lp)->code == LVN_ITEMCHANGED)
			{
				NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)lp;
				if (pNMListView->uChanged & LVIF_STATE)
				{
					if (pNMListView->uNewState & LVNI_SELECTED)
					{
						if ((pNMListView->uOldState & LVNI_SELECTED) == 0)
						{
							getKGM()->getEntryListWindow()->onSelectIMGEntry(true);
						}
					}
					else
					{
						if (pNMListView->uOldState & LVNI_SELECTED)
						{
							getKGM()->getEntryListWindow()->onSelectIMGEntry(false);
						}
					}
				}
			}
			else if (((LPNMHDR)lp)->code == NM_CUSTOMDRAW) // main list view item background and text colours
			{
				LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)lp;
				// todo CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
				// todo if (lplvcd->nmcd.hdr.hwndFrom != pListControl->GetSafeHwnd())
				// todo 	return CDRF_DODEFAULT;
				CIMGEntry *pIMGEntry;
				DWORD uiTextColour;
				switch (lplvcd->nmcd.dwDrawStage) {
				case CDDS_PREPAINT:
					return CDRF_NOTIFYITEMDRAW;
				case CDDS_ITEMPREPAINT:
					return CDRF_NOTIFYSUBITEMDRAW;
				case CDDS_SUBITEM:
					/*
					todo
					pIMGEntry = (CIMGEntry*)pListControl->GetItemData(lplvcd->nmcd.dwItemSpec);

					uiTextColour = RGB(0, 0, 0);
					if (pIMGEntry->isNewEntry())
					{
						uiTextColour = RGB(16, 140, 16); // green
					}
					else if (pIMGEntry->isReplacedEntry()) // blue
					{
						uiTextColour = RGB(28, 75, 236);
					}
					else if (pIMGEntry->isProtectedEntry()) // red
					{
						uiTextColour = RGB(227, 21, 21);
					}
					lplvcd->clrText = uiTextColour;

					if (((int)lplvcd->nmcd.dwItemSpec % 2) == 0)
					{
						lplvcd->clrTextBk = RGB(255, 255, 255); // white
					}
					else
					{
						lplvcd->clrTextBk = RGB(235, 235, 235); // grey
					}
					*/
					return CDRF_NEWFONT;
				default:
					return CDRF_DODEFAULT;
				}
				return TRUE;
			}
		}
		else if (LOWORD(wp) == 28 || LOWORD(wp) == 43 || // button colours
			LOWORD(wp) == 26 || LOWORD(wp) == 27 ||
			LOWORD(wp) == 29 || LOWORD(wp) == 45 ||
			LOWORD(wp) == 31 ||
			LOWORD(wp) == 49 ||
			LOWORD(wp) == 53)
		{
			if (!getKGM()->getSettingsManager()->getSettingBool("ColouredButtons"))
			{
				return FALSE;
			}

			COLORREF colour;
			if (LOWORD(wp) == 28 || LOWORD(wp) == 43) // import
			{
				colour = RGB(176, 226, 255);
			}
			else if (LOWORD(wp) == 26 || LOWORD(wp) == 27) // export
			{
				colour = RGB(164, 243, 164);
			}
			else if (LOWORD(wp) == 29 || LOWORD(wp) == 45) // remove
			{
				colour = RGB(251, 181, 181);
			}
			else if (LOWORD(wp) == 31) // rename
			{
				colour = RGB(246, 226, 141);
			}
			else if (LOWORD(wp) == 49) // dump
			{
				colour = RGB(245, 202, 84);
			}
			else if (LOWORD(wp) == 53) // update.lst
			{
				colour = RGB(206, 245, 255);
			}

			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)(LPNMHDR)lp;

			HBRUSH hPattern = createSolidBrush2(colour, item);
			HGDIOBJ old_brush = SelectObject(item->hdc, hPattern);

			RECT rect;
			rect.left = 2;
			rect.top = 2;
			rect.right = 79;
			rect.bottom = 24;
			FillRect(item->hdc, &rect, hPattern);

			SelectObject(item->hdc, old_brush);
			DeleteObject(hPattern);

			return CDRF_DODEFAULT;
		}
		/*
		else if (LOWORD(wp) == 37)
		{
		if (((LPNMHDR)lp)->code == -16)
		{
		//MessageBox(CStringUtility::convertStdStringToStdWString(CStringUtility::toString(((LPNMHDR)lp)->code)).c_str());
		COleDataSource ods;
		HGLOBAL hgData = GlobalAlloc(GPTR, 10); //The 10 is the data length
		LPCSTR  lpData = (LPCSTR)GlobalLock(hgData);
		_ltoa_s(20, (char*)lpData, 10, 10);
		ods.CacheGlobalData(CF_TEXT, hgData);
		ods.DoDragDrop(DROPEFFECT_COPY);
		}
		}
		*/
		break;
	}
	return 0;
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