#include "CKGMWindow.h"
#include "GUI/Screens/CIMGScreen.h"
#include "GUI/Controls/CWindowControl_Button.h"
#include "GUI/Controls/CWindowControl_List.h"
#include "GUI/CWindowManager.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "Globals.h"
#include "CKGM.h"
#include "Task/CTaskManager.h"
#include "Task/CTaskDispatchManager.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "File/CFileUtility.h"
#include "Path/CPathUtility.h"
#include "String/CStringUtility.h"

using namespace std;

CKGMWindow::CKGMWindow(void)
{
	initTabs();
}

// event binding
void					CKGMWindow::bindEvents(void)
{
	bindEvent(EVENT_onRender, [](void *pWindow)
	{
		((CKGMWindow*) pWindow)->onRender();
	}, this);

	CTabbedWindow::bindEvents();
}

// window initialization
void					CKGMWindow::initTabs(void)
{
	// add inner window
	setBackgroundColour(0x214E67FF);

	// add inner window controls
	CWindowControl_Button *pButton = new CWindowControl_Button;
	pButton->setWindow(this);
	string strText = "Open";
	pButton->setText(strText);
	pButton->setPosition(CVector2ui32(38, 35 + 38));
	pButton->setSize(CVector2ui32(172, 40));
	pButton->setFillColour(0x1A3C4EFF);
	pButton->setControlId(1);
	getControls().addEntry(pButton);

	CWindowControl_List *pList = new CWindowControl_List;
	pList->setWindow(this);
	pList->setPosition(CVector2ui32(252, 35 + 87));
	pList->setSize(CVector2ui32(732, 480));
	pList->setFillColour(0xECF3FDFF);
	pList->setRowBackgroundColour1(0xFF0000FF);
	pList->setRowBackgroundColour2(0x00FF00FF);
	pList->setRowHeight(25);
	pList->setControlId(2);
	// todo setEntryListControl(pList);
	getControls().addEntry(pList);

	CEventManager::getInstance()->bindEvent(EVENT_onPressButton, [](void *pData)
	{
		CWindowControl_Button *pButton = (CWindowControl_Button*) pData;
		if (pButton->getControlId() == 1)
		{
			getKGM()->getTaskManager()->getDispatch()->onRequestOpen();
			// todo getKGM()->getWindowManager()->getMainWindow()->setMarkedToRedraw(true);
			// todo getKGM()->getWindowManager()->render(); // todo - needed?
		}
	});
}

// render
void					CKGMWindow::onRender(void)
{
	// render background
	CGDIPlusUtility::drawRectangleFill(CVector2ui32(0, 0), getSize(), getBackgroundColour());

	// render title bar
	string strTitleBarText = "Komodo Game Manager";
	uint32 uiTitleBarTextFontSize = 30;
	uint32 uiTitleBarTextWidth = CGDIPlusUtility::getTextWidth(strTitleBarText, uiTitleBarTextFontSize);
	uint32 uiTitleBarTextX = (getSize().m_x / 2) - (uiTitleBarTextWidth / 2);

	CGDIPlusUtility::drawRectangleFill(CVector2ui32(0, 0), CVector2ui32(getSize().m_x, getTitleBarHeight()), 0x387EA3FF);
	CGDIPlusUtility::drawText(CVector2ui32(uiTitleBarTextX, 1), CVector2ui32(uiTitleBarTextWidth, getTitleBarHeight()), strTitleBarText, 0xE1E6EFFF, uiTitleBarTextFontSize, false);




	CVector2ui32 vecDrawStartPosition = CVector2ui32(0, getTitleBarHeight());

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
	// todo CGDIPlusUtility::drawRectangleFill(CVector2ui32(vecDrawStartPosition.m_x + 252, vecDrawStartPosition.m_y + 87), CVector2ui32(732, 480), 0xECF3FDFF);
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
	for (CWindowControl *pWindowControl : getControls().getEntries())
	{
		pWindowControl->render();
	}
}

// input processing
void					CKGMWindow::onDropFiles(vector<string>& vecPaths)
{
	vector<string>
		vecOpenPaths,
		vecEntryPaths;
	for (uint32 i = 0; i < vecPaths.size(); i++)
	{
		string strPath = vecPaths[i];

		// folder
		if (CFileUtility::doesFolderExist(strPath))
		{
			strPath = CPathUtility::addSlashToEnd(strPath);
			vector<string> vecFileNames = CFileUtility::getFileNames(strPath);
			for (auto strFileName : vecFileNames)
			{
				vecPaths.push_back(strPath + strFileName);
			}
			continue;
		}

		string strExtension = CStringUtility::toUpperCase(CPathUtility::getFileExtension(strPath));
		if (strExtension == "IMG" || strExtension == "DIR")
		{
			// open IMG
			vecOpenPaths.push_back(strPath);
		}
		else
		{
			if (getKGM()->getActiveTab() == nullptr)
			{
				return;
			}

			vecEntryPaths.push_back(strPath);
		}
	}

	if (vecEntryPaths.size() > 0)
	{
		for (auto strPath : vecEntryPaths)
		{
			getKGM()->getEntryListTab()->addOrReplaceEntryViaFileAndSettings(strPath);
		}
		//getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_23", vecImportPaths.size()));

		getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	}

	for (auto strPath : vecOpenPaths)
	{
		getKGM()->getTaskManager()->getDispatch()->onRequestOpen2(strPath);
	}
}