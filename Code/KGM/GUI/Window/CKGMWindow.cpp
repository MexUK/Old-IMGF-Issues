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
	/*
	todo
	bindEvent(EVENT_onRender, [](void *pData)
	{
		((CKGMWindow*) pData)->renderTitleBar();
	});
	*/
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
void					CKGMWindow::renderTitleBar(void)
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