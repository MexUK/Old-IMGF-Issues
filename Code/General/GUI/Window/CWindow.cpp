#include "CWindow.h"
#include "GUI/CGUIManager.h"
#include "GUI/Controls/CRadioControl.h"
#include "Math/CMathUtility.h"
#include "Path/CPathUtility.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibraries/CGraphicsLibrary_GDIPlus.h"
#include "GUI/Styles/CGUIStyles.h"
#include "GUI/CGUIUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "Event/eEventType.h"
#include "Localization/CLocalizationManager.h"

using namespace std;

auto pOnMouseDown_Window		= [](void *pWindow, void *pTriggerArg) { ((CWindow*) pWindow)->onMouseDown(*(CVector2i32*) pTriggerArg); };
auto pOnMouseUp_Window			= [](void *pWindow, void *pTriggerArg) { ((CWindow*) pWindow)->onMouseUp(*(CVector2i32*) pTriggerArg); };
auto pOnMouseMove_Window		= [](void *pWindow, void *pTriggerArg) { ((CWindow*) pWindow)->onMouseMove(*(CVector2i32*) pTriggerArg); };
auto pOnDoubleLeftClick_Window	= [](void *pWindow, void *pTriggerArg) { ((CWindow*) pWindow)->onDoubleLeftClick(*(CVector2i32*) pTriggerArg); };

CWindow::CWindow(void) :
	CEventType(EVENT_TYPE_WINDOW),
	m_hwndWindow(nullptr),
	m_pFocusedControl(nullptr),
	m_uiWindowResizeEdges(0),
	m_bMovingWindow(false),
	m_bResizingWindow(false),
	m_bMarkedToRedraw(false),
	m_bMaximized(false),
	m_pDropTarget(nullptr),
	m_pParentWindow(nullptr),
	m_uiTitleBarHeight(0),
	m_pActiveItem(nullptr)
{
	m_vecPosition.m_x = 0;
	m_vecPosition.m_y = 0;
	m_vecSize.m_x = 0;
	m_vecSize.m_y = 0;
	m_vecPreviousPosition.m_x = 0;
	m_vecPreviousPosition.m_y = 0;
}
CWindow::~CWindow(void)
{
	// todo delete m_pDropTarget;
}

void									CWindow::unload(void)
{
}

// event binding
void									CWindow::bindEvents(void)
{
	m_placeableItem.setWindow(this);
	m_placeableItem.setItems(((CVectorPool<CGUIItem*>*)&(getEntryByIndex(0)->getShapes())));

	storeEventBoundFunction(bindEvent(EVENT_onLeftMouseDown, pOnMouseDown_Window, this));
	storeEventBoundFunction(bindEvent(EVENT_onLeftMouseUp, pOnMouseUp_Window, this));
	storeEventBoundFunction(bindEvent(EVENT_onMouseMove, pOnMouseMove_Window, this));
	storeEventBoundFunction(bindEvent(EVENT_onLeftMouseDoubleClick, pOnDoubleLeftClick_Window, this));

	m_placeableItem.bindEvents();
}

void									CWindow::unbindEvents(void)
{
	m_placeableItem.unbindEvents();
	CEventBinder::unbindEvents();
}

void									CWindow::bindAllEvents(void)
{
	bindEvents();
	for (CGUILayer *pLayer : getEntries())
	{
		if (pLayer->isEnabled())
		{
			pLayer->bindAllEvents();
		}
	}
}

void									CWindow::unbindAllEvents(void)
{
	for (CGUILayer *pLayer : getEntries())
	{
		if (pLayer->isEnabled())
		{
			pLayer->unbindAllEvents();
		}
	}
	unbindEvents();
}

// event triggering
bool									CWindow::triggerEvent(uint32 uiEventId, void *pTriggerArgument)
{
	return CEventType::triggerEvent(uiEventId, pTriggerArgument, (uint32)getWindowHandle());
}

CEventBoundFunction*					CWindow::bindEvent(uint32 uiEventId, void(*pFunction)(void*), void *pTriggerArgument, int32 iZOrder)
{
	return CEventType::bindEvent(uiEventId, pFunction, pTriggerArgument, iZOrder, (uint32) getWindowHandle());
}

CEventBoundFunction*					CWindow::bindEvent(uint32 uiEventId, void(*pFunction)(void*, void*), void *pTriggerArgument, int32 iZOrder)
{
	return CEventType::bindEvent(uiEventId, pFunction, pTriggerArgument, iZOrder, (uint32) getWindowHandle());
}

CEventBoundFunction*					CWindow::bindEvent(uint32 uiEventId, CInputEventCallbacks *pObject, void *pTriggerArgument, int32 iZOrder)
{
	return CEventType::bindEvent(uiEventId, pObject, pTriggerArgument, iZOrder, (uint32) getWindowHandle());
}

// input
void									CWindow::onMouseDown(CVector2i32& vecCursorPosition)
{
	if (CEventManager::getInstance()->isEventHogged())
	{
		return;
	}

	// cursor enter/leave control and focused control
	bool bGainedFocusOverall = false;
	for (CGUILayer *pLayer : getEntries())
	{
		for (CGUIControl *pWindowControl : pLayer->getControls().getEntries())
		{
			if (CMathUtility::isPointInRectangle(vecCursorPosition, pWindowControl->getPosition(), pWindowControl->getSize()))
			{
				bool bGainedFocus = m_pFocusedControl != pWindowControl;
				m_pFocusedControl = pWindowControl;
				if (bGainedFocus)
				{
					pWindowControl->getWindow()->triggerEvent(EVENT_onControlGainFocus, pWindowControl);
					bGainedFocusOverall = true;
				}
			}
		}
	}
	if (!bGainedFocusOverall)
	{
		m_pFocusedControl = nullptr;
	}

	// move/resize window
	if (isMaximized())
	{
		return;
	}

	RECT rect;
	GetWindowRect(getWindowHandle(), &rect);
	CVector2ui32 vecMainWindowSize(rect.right - rect.left, rect.bottom - rect.top);

	setWindowResizeEdges(CMathUtility::getRectangleResizeEdges(vecCursorPosition, vecMainWindowSize, 10));
	if (getWindowResizeEdges() == 0)
	{
		// move window
		setMovingWindow(true);
	}
	else
	{
		// resize window
		setResizingWindow(true);
	}

	// enable capture
	SetCapture(getWindowHandle());
}

void									CWindow::onMouseUp(CVector2i32& vecCursorPosition)
{
	if (isMovingWindow())
	{
		setMovingWindow(false);
		ReleaseCapture();
		return;
	}
	else if (isResizingWindow())
	{
		setResizingWindow(false);
		setWindowResizeEdges(0);
		ReleaseCapture();
		return;
	}
}

void									CWindow::onMouseMove(CVector2i32& vecCursorPosition)
{
	for (CGUILayer *pLayer : getEntries())
	{
		for (CGUIControl *pWindowControl : pLayer->getControls().getEntries())
		{
			if (pWindowControl->isPointInItem(vecCursorPosition))
			{
				if (!pWindowControl->isPointMarkedAsInControl())
				{
					pWindowControl->setPointMarkedAsInControl(true);
					triggerEvent(EVENT_onCursorEnterControl, pWindowControl);
				}
			}
			else
			{
				if (pWindowControl->isPointMarkedAsInControl())
				{
					pWindowControl->setPointMarkedAsInControl(false);
					triggerEvent(EVENT_onCursorLeaveControl, pWindowControl);
				}
			}
		}
	}

	if (isMovingWindow())
	{
		POINT point;
		GetCursorPos(&point);
		vecCursorPosition = CVector2i32(point.x, point.y);

		RECT rect;
		GetWindowRect(getWindowHandle(), &rect);

		CVector2i32 vecCursorDiff = CEventManager::getInstance()->getScreenCursorMoveDifference();

		rect.left += vecCursorDiff.m_x;
		rect.top += vecCursorDiff.m_y;

		SetWindowPos(getWindowHandle(), NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		setPosition(CVector2i32(rect.left, rect.top));
		//MoveWindow(hwndWindow, rect.left, rect.top, rect.right, rect.bottom, false);
		//InvalidateRect(hwndWindow, &rect, false);
		return;
	}
	else if (isResizingWindow())
	{
		POINT point;
		GetCursorPos(&point);
		vecCursorPosition = CVector2i32(point.x, point.y);

		RECT rect;
		GetWindowRect(getWindowHandle(), &rect);

		CVector2i32 vecCursorDiff = CEventManager::getInstance()->getScreenCursorMoveDifference();

		uint32 uiMainWindowResizeEdges = getWindowResizeEdges();
		if (uiMainWindowResizeEdges & 1) // left edge
		{
			rect.left += vecCursorDiff.m_x;
		}
		else if (uiMainWindowResizeEdges & 4) // right edge
		{
			rect.right += vecCursorDiff.m_x;
		}

		if (uiMainWindowResizeEdges & 2) // top edge
		{
			rect.top += vecCursorDiff.m_y;
		}
		else if (uiMainWindowResizeEdges & 8) // bottom edge
		{
			rect.bottom += vecCursorDiff.m_y;
		}

		SetWindowPos(getWindowHandle(), NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
		return;
	}
}

void									CWindow::onDoubleLeftClick(CVector2i32& vecCursorPosition)
{
	RECT rect;
	GetWindowRect(getWindowHandle(), &rect);
	CVector2ui32 vecMainWindowTitleBarSize(rect.right - rect.left, 35);

	if (CMathUtility::isPointInRectangle(vecCursorPosition, CVector2i32(0, 0), vecMainWindowTitleBarSize))
	{
		setMaximized(!isMaximized());
	}
}

// render
void									CWindow::render(void)
{
	RedrawWindow(getWindowHandle(), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void									CWindow::checkToRender(void)
{
	if (isMarkedToRedraw())
	{
		render();
	}
}

void									CWindow::onRenderFromWMPaint(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	// begin paint
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(getWindowHandle(), &ps);

	// fetch window position and size
	RECT rect;
	GetClientRect(getWindowHandle(), &rect);

	// create other DCs
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
	HANDLE hOld = SelectObject(hdcMem, hbmMem);

	// create and store GDI Plus Graphics object
	Gdiplus::Graphics *pGraphics = new Gdiplus::Graphics(hdcMem);
	((CGraphicsLibrary_GDIPlus*) pGFX)->setGraphics(pGraphics);

	// render window background and border
	if (getStyles()->doesHaveFill())
	{
		pGFX->drawRectangleFill(CVector2i32(0, 0), getSize(), getStyles());
	}
	if (getStyles()->doesHaveBorder())
	{
		pGFX->drawRectangleBorder(CVector2i32(0, 0), getSize(), getStyles());
	}

	// render to memory
	triggerEvent(EVENT_onRenderBefore);
	triggerEvent(EVENT_onRender);
	triggerEvent(EVENT_onRenderAfter);

	// render to screen
	BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);

	// delete GDI Plus Graphics object
	delete pGraphics;

	// clean other DCs
	SelectObject(hdcMem, hOld);
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);

	// end paint
	EndPaint(getWindowHandle(), &ps);
}

// layers
CGUILayer*							CWindow::addLayer(bool bEnabled)
{
	CGUILayer *pLayer = new CGUILayer;
	pLayer->setEnabled(bEnabled);
	return pLayer;
}

CGUILayer*							CWindow::addLayer(CWindow *pWindow, bool bEnabled)
{
	CGUILayer *pLayer = new CGUILayer;
	pLayer->setWindow(pWindow);
	pLayer->setEnabled(bEnabled);
	addEntry(pLayer);
	return pLayer;
}

// active item
void								CWindow::setActiveItem(CGUIItem *pItem)
{
	m_pActiveItem = pItem;
	pItem->getStyles()->setStyle("border-colour", RGB(255, 0, 0));
}

void								CWindow::clearActiveItem(void)
{
	m_pActiveItem->getStyles()->setStyle("border-colour", RGB(0, 0, 0));
	m_pActiveItem = nullptr;
}

// maximized
void									CWindow::setMaximized(bool bMaximized)
{
	m_bMaximized = bMaximized;

	if (m_bMaximized)
	{
		// Don't use ShowWindow() because it does full screen rather than maximized. (It excludes and overlaps the bottom app bar)
		// todo ShowWindow(getWindowHandle(), SW_MAXIMIZE);

		RECT rectWorkArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rectWorkArea, 0);
		SetWindowPos(getWindowHandle(), NULL, rectWorkArea.left, rectWorkArea.top, rectWorkArea.right, rectWorkArea.bottom, NULL);
	}
	else
	{
		// todo ShowWindow(getWindowHandle(), SW_RESTORE);

		RECT rectWorkArea;
		rectWorkArea.left = getPreviousPosition().m_x;
		rectWorkArea.top = getPreviousPosition().m_y;
		rectWorkArea.right = 1025; // todo - previous size
		rectWorkArea.bottom = 698;
		SetWindowPos(getWindowHandle(), NULL, rectWorkArea.left, rectWorkArea.top, rectWorkArea.right, rectWorkArea.bottom, NULL);
	}

	render();
}

// other
void									CWindow::unmarkRadios(CRadioControl *pRadio)
{
	for (CGUILayer *pLayer : getEntries())
	{
		for (CGUIControl *pWindowControl : pLayer->getControls().getEntries())
		{
			if (pWindowControl->getControlType() == GUI_CONTROL_RADIO)
			{
				CRadioControl *pRadio2 = (CRadioControl*) pWindowControl;
				if (pRadio2->getGroupId() == pRadio->getGroupId())
				{
					pRadio2->setMarked(false);
				}
			}
		}
	}
}

void									CWindow::setOpenLastFilename(string strFileName)
{
	// todo
	//string strMenuItemText = CLocalizationManager::getInstance()->getTranslatedText("Menu_OpenLast") + (getKGM()->getRecentlyOpenManager()->getLastOpenEntry() == "" ? "" : " (" + CPathUtility::getFileName(getKGM()->getRecentlyOpenManager()->getLastOpenEntry()) + ")");
	
	/*
	todo
	CGUIUtility::setMenuText(m_hMenu_File, 1117, strMenuItemText);
	*/
}

void									CWindow::clearOpenLastFilename(void)
{
	/*
	todo
	CGUIUtility::setMenuText(m_hMenu_File, 1117, CLocalizationManager::getInstance()->getTranslatedText("Menu_OpenLast"));
	*/
}










/*
todo

void		CKGM::init(void)
{
	if (isAlphaBuild())
	{
		//SetWindowText(getDialog()->GetSafeHwnd(), CStringUtility::convertStdStringToStdWString("KGM " + getCurrentVersionString() + (isAlphaBuild() ? (" - Build " + string(BUILDNUMBER_STR)) : "")).c_str());
		SetWindowText(getActiveWindow()->getWindowHandle(), CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Title_Alpha", getCurrentVersionString().c_str(), BUILDNUMBER_STR).c_str());
	}
	else
	{
		SetWindowText(getActiveWindow()->getWindowHandle(), CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Title_Regular", getCurrentVersionString().c_str()).c_str());
	}

	// todo - too early in execution progress?
	//CGUIUtility::setParentWindowHwnd(getActiveWindow()->getWindowHandle());
	//getPopupGUIManager()->setParentWindowHwnd(getActiveWindow()->getWindowHandle());
}
*/

	// todo
	// control text for language
	/*
	m_pDialog->GetDlgItem(23)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Group_Search").c_str());
	//m_pDialog->GetDlgItem(13)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Group_Filter").c_str());
	//m_pDialog->GetDlgItem(10)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Check_Filter_Extensions").c_str());
	//m_pDialog->GetDlgItem(52)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Check_Filter_PreserveAcrossTabs").c_str());
	//m_pDialog->GetDlgItem(44)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Offset").c_str());
	//m_pDialog->GetDlgItem(3)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Size").c_str());
	//m_pDialog->GetDlgItem(4)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Version").c_str());
	m_pDialog->GetDlgItem(19)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Text_IMGVersion").c_str());
	m_pDialog->GetDlgItem(46)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("AllTabs").c_str());
	m_pDialog->GetDlgItem(15)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Log").c_str());
	m_pDialog->GetDlgItem(50)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Find").c_str());
	m_pDialog->GetDlgItem(0)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Text_SearchResult_ActiveTab", 0).c_str());
	m_pDialog->GetDlgItem(20)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Text_EntryCount", 0).c_str());
	m_pDialog->GetDlgItem(51)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Text_SelectedEntryCount", 0).c_str());
	m_pDialog->GetDlgItem(21)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("IMG").c_str());
	m_pDialog->GetDlgItem(30)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Entries").c_str());

	// IMG buttons
	m_pDialog->GetDlgItem(39)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Open").c_str());
	m_pDialog->GetDlgItem(40)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Close").c_str());
	m_pDialog->GetDlgItem(41)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("CloseAll").c_str());
	m_pDialog->GetDlgItem(35)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Rebuild").c_str());
	m_pDialog->GetDlgItem(36)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Button_RebuildAs").c_str());
	m_pDialog->GetDlgItem(42)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("RebuildAll").c_str());
	m_pDialog->GetDlgItem(17)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Merge").c_str());
	m_pDialog->GetDlgItem(18)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Split").c_str());
	m_pDialog->GetDlgItem(16)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Convert").c_str());

	// Entry buttons
	m_pDialog->GetDlgItem(28)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Import").c_str());
	m_pDialog->GetDlgItem(43)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Button_ImportVia").c_str()); 
	m_pDialog->GetDlgItem(53)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("UpdateLST").c_str());

	m_pDialog->GetDlgItem(26)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Export").c_str());
	m_pDialog->GetDlgItem(27)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Button_ExportVia").c_str());
	m_pDialog->GetDlgItem(47)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("QuickExport").c_str());

	m_pDialog->GetDlgItem(29)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Remove").c_str());
	m_pDialog->GetDlgItem(45)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Button_RemoveVia").c_str());
	m_pDialog->GetDlgItem(49)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Dump").c_str());

	m_pDialog->GetDlgItem(31)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Rename").c_str());
	m_pDialog->GetDlgItem(25)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Replace").c_str());

	m_pDialog->GetDlgItem(32)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Select_All").c_str());
	m_pDialog->GetDlgItem(33)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Select_Inverse").c_str());
	m_pDialog->GetDlgItem(34)->SetWindowText(CLocalizationManager::getInstance()->getTranslatedTextW("Sort").c_str());

	// main list control
	getIMGEditor()->addColumnsToMainListView(IMG_UNKNOWN);

	// search list control
	((CListCtrl*)m_pDialog->GetDlgItem(22))->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
	((CListCtrl*)m_pDialog->GetDlgItem(22))->InsertColumn(0, CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_ListView_ColumnTitle_Name").c_str(), LVCFMT_LEFT, 115);
	((CListCtrl*)m_pDialog->GetDlgItem(22))->InsertColumn(1, CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_SearchListView_ColumnTitle_IMGFile").c_str(), LVCFMT_LEFT, 80);
	((CListCtrl*)m_pDialog->GetDlgItem(22))->InsertColumn(2, CLocalizationManager::getInstance()->getTranslatedTextW("RWVersion").c_str(), LVCFMT_LEFT, 125);

	// filter dropdown boxes
	//////////
	((CComboBox*)m_pDialog->GetDlgItem(6))->InsertString(0, CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Combo_Equals").c_str());
	((CComboBox*)m_pDialog->GetDlgItem(6))->InsertString(1, CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Combo_GTE").c_str());
	((CComboBox*)m_pDialog->GetDlgItem(6))->InsertString(2, CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Combo_LTE").c_str());
	((CComboBox*)m_pDialog->GetDlgItem(6))->InsertString(3, CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Combo_Not").c_str());

	((CComboBox*)m_pDialog->GetDlgItem(7))->InsertString(0, CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Combo_Equals").c_str());
	((CComboBox*)m_pDialog->GetDlgItem(7))->InsertString(1, CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Combo_GTE").c_str());
	((CComboBox*)m_pDialog->GetDlgItem(7))->InsertString(2, CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Combo_LTE").c_str());
	((CComboBox*)m_pDialog->GetDlgItem(7))->InsertString(3, CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Combo_Not").c_str());
	//////////

	((CComboBox*)m_pDialog->GetDlgItem(54))->InsertString(0, CLocalizationManager::getInstance()->getTranslatedTextW("AllTypes").c_str());
	((CComboBox*)m_pDialog->GetDlgItem(54))->SetCurSel(0);

	uint32 i = 0;
	((CComboBox*)m_pDialog->GetDlgItem(5))->InsertString(i, CLocalizationManager::getInstance()->getTranslatedTextW("AllVersions").c_str());
	*/
	/*
	/////////////
	i++;
	for (auto pRWVersion : CRWManager::getInstance()->getVersionManager()->getEntries())
	{
		m_umapMenuItemMapping_FilterRWVersion[i] = pRWVersion;
		((CComboBox*)m_pDialog->GetDlgItem(5))->InsertString(i, CStringUtility::convertStdStringToStdWString("RW " + pRWVersion->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pRWVersion->getLocalizationKey()) + ")").c_str());
		i++;
	}
	for (auto pCOLVersion : CCOLManager::getInstance()->getVersionManager()->getEntries())
	{
		m_umapMenuItemMapping_FilterCOLVersion[i] = pCOLVersion->getId();
		((CComboBox*)m_pDialog->GetDlgItem(5))->InsertString(i, CLocalizationManager::getInstance()->getTranslatedTextW(pCOLVersion->getLocalizationKey()).c_str());
		i++;
	}
	((CComboBox*)m_pDialog->GetDlgItem(5))->InsertString(i, CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Combo_RWVersion_Unknown").c_str());
	///////////////
	*/
	//((CComboBox*)m_pDialog->GetDlgItem(5))->SetCurSel(0);

	/*
	// drag drop
	AfxOleInit();
	m_pDropTarget = new CDropTarget;
	m_pDropTarget->Register(((CTabCtrl*)m_pDialog->GetDlgItem(1)));

	// recently open entries and sessions
	getRecentlyOpenManager()->init();
	getSessionManager()->init();
	*/