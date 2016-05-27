#include "CWindow.h"
#include "Globals.h"
#include "CKGM.h"
#include "Window Controls/Controls/CWindowControl_Radio.h"
#include "Math/CMathUtility.h"
#include "Path/CPathUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "GUI/CGUIUtility.h"
#include "Event/CEventManager.h"
#include "Recently Open/CRecentlyOpenManager.h"
#include "Localization/CLocalizationManager.h"
#include "Window/CWindowManager.h"

using namespace std;

CWindow::CWindow(void) :
	m_hwndWindow(nullptr),
	m_hdc(nullptr),
	m_pFocusedControl(nullptr),
	m_uiWindowResizeEdges(0),
	m_bMovingWindow(false),
	m_bResizingWindow(false),
	m_bMarkedToRedraw(false),
	m_bMaximized(false),
	m_uiBackgroundColour(0xFFFFFFFF),
	m_pDropTarget(nullptr),
	m_pParentWindow(nullptr),
	m_uiTitleBarHeight(0)
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

// input
void									CWindow::onMouseDown(CVector2ui32& vecCursorPosition)
{
	bool bGainedFocusOverall = false;
	for (CWindowControl *pWindowControl : getControls().getEntries())
	{
		if (CMathUtility::isPointInRectangle(vecCursorPosition, pWindowControl->getPosition(), pWindowControl->getSize()))
		{
			bool bGainedFocus = m_pFocusedControl != pWindowControl;
			m_pFocusedControl = pWindowControl;
			if (bGainedFocus)
			{
				pWindowControl->onGainFocus();
				bGainedFocusOverall = true;
			}
		}
		pWindowControl->onMouseDown(vecCursorPosition);
	}
	if (!bGainedFocusOverall)
	{
		m_pFocusedControl = nullptr;
	}
}

void									CWindow::onMouseUp(CVector2ui32& vecCursorPosition)
{
	for (CWindowControl *pWindowControl : getControls().getEntries())
	{
		pWindowControl->onMouseUp(vecCursorPosition);
	}
}

void									CWindow::onMouseMove(CVector2ui32& vecCursorPosition)
{
	for (CWindowControl *pWindowControl : getControls().getEntries())
	{
		pWindowControl->onMouseMove(vecCursorPosition);

		if (pWindowControl->isPointInControl(vecCursorPosition))
		{
			if (!pWindowControl->isPointMarkedAsInControl())
			{
				pWindowControl->setPointMarkedAsInControl(true);
				CEventManager::getInstance()->triggerEvent("onCursorEnterControl", pWindowControl);
			}
		}
		else
		{
			if (pWindowControl->isPointMarkedAsInControl())
			{
				pWindowControl->setPointMarkedAsInControl(false);
				CEventManager::getInstance()->triggerEvent("onCursorExitControl", pWindowControl);
			}
		}
	}
}

void									CWindow::onCharDown(uint8 uiCharCode)
{
	for (CWindowControl *pWindowControl : getControls().getEntries())
	{
		pWindowControl->onCharDown(uiCharCode);
	}
}

void									CWindow::onKeyDown(uint8 uiCharCode)
{
	for (CWindowControl *pWindowControl : getControls().getEntries())
	{
		pWindowControl->onKeyDown(uiCharCode);
	}
}

void									CWindow::onKeyUp(void)
{
}

// render
void									CWindow::render(void)
{
	CWindow *pParentWindow = getParentWindow();
	CVector2ui32 vecDrawStartPosition = CVector2ui32(0, pParentWindow->getTitleBarHeight());



	// render background
	CGDIPlusUtility::drawRectangleFill(vecDrawStartPosition, getSize(), getBackgroundColour());

	// render window inner backgrounds
	string strTabText = "Opened.IMG";
	string strTotalEntriesText = "Total Entries: 0";

	uint32 uiTabTextFontSize = 14;
	uint32 uiTabTextY = vecDrawStartPosition.m_y + 66;
	uint32 uiTabTextWidth = CGDIPlusUtility::getTextWidth(strTabText, uiTabTextFontSize);
	uint32 uiTabPaddingX = 17;
	uint32 uiTabWidth = uiTabTextWidth + (2 * uiTabPaddingX);
	uint32 uiTabTextX = vecDrawStartPosition.m_x + 252 + uiTabPaddingX;

	CGDIPlusUtility::drawRectangleFill(CVector2ui32(vecDrawStartPosition.m_x + 213, vecDrawStartPosition.m_y), CVector2ui32(805, 38), 0x739BB2FF);
	CGDIPlusUtility::drawRectangleFill(CVector2ui32(vecDrawStartPosition.m_x + 213, vecDrawStartPosition.m_y + 38), CVector2ui32(805, 586), 0x2B6381FF);
	CGDIPlusUtility::drawRectangleWithBorderRadius(CVector2ui32(vecDrawStartPosition.m_x + 226, vecDrawStartPosition.m_y + 57), CVector2ui32(777, 528), 20, 0x2B6381FF, 0xFDFEFEFF);
	//CGDIPlusUtility::drawRectangleFill(CVector2ui32(vecDrawStartPosition.m_x + 252, vecDrawStartPosition.m_y + 87), CVector2ui32(732, 480), 0xECF3FDFF);
	CGDIPlusUtility::drawRectangleFillWithGradient(CVector2ui32(vecDrawStartPosition.m_x + 252, vecDrawStartPosition.m_y + 61), CVector2ui32(uiTabWidth, 26), 0x5489A7FF, 0x316988FF);
	CGDIPlusUtility::drawText(CVector2ui32(vecDrawStartPosition.m_x + uiTabTextX, uiTabTextY), CVector2ui32(uiTabWidth, 26), strTabText, 0xE1E6EFFF, uiTabTextFontSize, false);
	CGDIPlusUtility::drawText(CVector2ui32(vecDrawStartPosition.m_x + 252, vecDrawStartPosition.m_y + 597), CVector2ui32(200, 20), strTotalEntriesText, 0xE1E6EFFF, 13, false);

	uint32 uiButtonY = vecDrawStartPosition.m_y + 38 + 40;
	for (uint32 i = 1; i < 14; i++)
	{
		uint32 uiFillColour = (i % 2) == 0 ? 0x1A3C4EFF : 0x214E67FF;
		CGDIPlusUtility::drawRectangleFill(CVector2ui32(38, uiButtonY), CVector2ui32(172, 40), uiFillColour);
		uiButtonY += 40;
	}

	// render controls
	for (CWindowControl *pWindowControl : m_vecControls.getEntries())
	{
		pWindowControl->render();
	}
}

// process
void									CWindow::processWindow(void)
{
	MSG msg;

	PeekMessage(&msg, 0, WM_USER, WM_USER, PM_NOREMOVE);

	while (GetMessage(&msg, getWindowHandle(), 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

// maximized
void									CWindow::setMaximized(bool bMaximized)
{
	m_bMaximized = bMaximized;

	if (m_bMaximized)
	{
		// Don't use ShowWindow() because it does full screen rather than maximized. (It excludes and overlaps the bottom app bar)
		//ShowWindow(getWindowHandle(), SW_MAXIMIZE);

		RECT rectWorkArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rectWorkArea, 0);
		SetWindowPos(getWindowHandle(), NULL, rectWorkArea.left, rectWorkArea.top, rectWorkArea.right, rectWorkArea.bottom, NULL);
	}
	else
	{
		//ShowWindow(getWindowHandle(), SW_RESTORE);

		RECT rectWorkArea;
		rectWorkArea.left = getPreviousPosition().m_x;
		rectWorkArea.top = getPreviousPosition().m_y;
		rectWorkArea.right = 1025; // todo - previous size
		rectWorkArea.bottom = 698;
		SetWindowPos(getWindowHandle(), NULL, rectWorkArea.left, rectWorkArea.top, rectWorkArea.right, rectWorkArea.bottom, NULL);
	}
}

// other
void									CWindow::uncheckRadios(CWindowControl_Radio *pRadio)
{
	for (CWindowControl *pWindowControl : getControls().getEntries())
	{
		if (pWindowControl->getControlType() == WINDOW_CONTROL_RADIO)
		{
			CWindowControl_Radio *pRadio2 = (CWindowControl_Radio*)pWindowControl;
			if (pRadio2->getGroupId() == pRadio->getGroupId())
			{
				pRadio2->setChecked(false);
			}
		}
	}
}

void									CWindow::setOpenLastFilename(string strFileName)
{
	string strMenuItemText = CLocalizationManager::getInstance()->getTranslatedText("Menu_OpenLast") + (getKGM()->getRecentlyOpenManager()->getLastOpenEntry() == "" ? "" : " (" + CPathUtility::getFileName(getKGM()->getRecentlyOpenManager()->getLastOpenEntry()) + ")");
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
	//getGUIManager()->setParentWindowHwnd(getActiveWindow()->getWindowHandle());
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
	getEntryListWindow()->addColumnsToMainListView(IMG_UNKNOWN);

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