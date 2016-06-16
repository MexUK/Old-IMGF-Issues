#include "CKGMWindow.h"
#include "GUI/Editors/CIMGEditor.h"
#include "GUI/Controls/CButtonControl.h"
#include "GUI/Controls/CListControl.h"
#include "GUI/Styles/CGUIStyles.h"
#include "GUI/CWindowManager.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "Globals.h"
#include "CKGM.h"
#include "Task/CTaskManager.h"
#include "Task/CTaskDispatchManager.h"
#include "File/CFileUtility.h"
#include "Path/CPathUtility.h"
#include "String/CStringUtility.h"
#include "CColour.h"

using namespace std;

CKGMWindow::CKGMWindow(void)
{
}

// event binding
void					CKGMWindow::bindEvents(void)
{
	storeEventBoundFunction(bindEvent(EVENT_onRender, [](void *pWindow)
	{
		((CKGMWindow*) pWindow)->onRender();
	}, this));

	CWindow::bindEvents();
}

// window initialization
void					CKGMWindow::initTabs(void)
{
	getStyles()->setStyle("fill-colour", CColour(0xFF, 0xFF, 0xFF));
	return; // todo

	// set window properties
	getStyles()->setStyle("fill-colour", CColour(0x21, 0x4E, 0x67));

	// fetch layer
	CEditor *pEditor = (CEditor*) getEntryByIndex(0);

	// add window controls
	CButtonControl *pButton = pEditor->addButton(CPoint2D(38, 35 + 38), CSize2D(172, 40), "Open");
	pButton->setLayer(pEditor);
	pButton->setControlId(1);
	pButton->getStyles()->setStyle("fill-colour", CColour(0, 0, 0));
	//pButton->getStyles()->setStyle("fill-colour-start", CColour(255, 0, 0));
	//pButton->getStyles()->setStyle("fill-colour-stop", CColour(255, 255, 0));
	pButton->getStyles()->setStyle("border-intersection-radius", (float32)15.0f);
	pButton->getStyles()->setStyle("text-colour", CColour(255, 128, 0));
	pEditor->getControls().addEntry(pButton);

	CListControl *pList = pEditor->addList(CPoint2D(252, 35 + 87), CSize2D(732, 480));
	pList->setLayer(pEditor);
	pList->getStyles()->setStyle("fill-colour", CColour(0xEC, 0xF3, 0xFD, 0xFF));
	pList->getStyles()->setStyle("row-fill-colour-1", CColour(0xFF, 0x00, 0x00, 0xFF));
	pList->getStyles()->setStyle("row-fill-colour-2", CColour(0x00, 0xFF, 0x00, 0xFF));
	pList->setRowHeight(25);
	pList->setControlId(2);
	// todo setEntryListControl(pList);
	pEditor->getControls().addEntry(pList);

	bindEvent(EVENT_onPressButton, [](void *pWindow, void *pData)
	{
		CButtonControl *pButton = (CButtonControl*) pData;
		if (pButton->getControlId() == 1)
		{
			getKGM()->getTaskManager()->getDispatch()->onRequestOpen();
			// todo getKGM()->getWindowManager()->getMainWindow()->setMarkedToRedraw(true);
			// todo getKGM()->getWindowManager()->render(); // todo - needed?
		}
	}, this);
}

// render
void					CKGMWindow::onRender(void)
{
	renderTitleBar();
	renderEtc();
}

void					CKGMWindow::renderTitleBar(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	string strTitleBarText = "Komodo Game Manager";
	uint32 uiTitleBarTextFontSize = 25;
	CGUIStyles styles9;
	styles9.setEntry("text-size", uiTitleBarTextFontSize);
	uint32 uiTitleBarTextWidth = pGFX->getTextSize(strTitleBarText, &styles9).m_x;
	uint32 uiTitleBarTextX = (getSize().m_x / 2) - (uiTitleBarTextWidth / 2);

	CGUIStyles styles1;
	styles1.setEntry("fill-colour", CColour(0x38, 0x7E, 0xA3));
	styles1.setEntry("text-colour", CColour(0xE1, 0xE6, 0xEF));
	styles1.setEntry("text-size", uiTitleBarTextFontSize);
	pGFX->drawRectangle(CPoint2D(0, 0), CSize2D(getSize().m_x, getTitleBarHeight()), &styles1);
	pGFX->drawText(CPoint2D(uiTitleBarTextX, 1), CSize2D(uiTitleBarTextWidth, getTitleBarHeight()), strTitleBarText, &styles1);

	if (CGUIManager::getInstance()->isThemeDesignerModeEnabled())
	{
		CGUIStyles stylesThemeDesigner;
		stylesThemeDesigner.setStyle("text-colour", CColour(241, 155, 30));
		//stylesThemeDesigner.setStyle<string>("text-style", "bold");
		stylesThemeDesigner.setStyle<uint32>("text-size", 15);
		string strText = "[Theme Designer Mode]";
		pGFX->drawText(CPoint2D(10, 8), CSize2D(200, 20), strText, &stylesThemeDesigner);
	}
}

void					CKGMWindow::renderEtc(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();
	CPoint2D vecDrawStartPosition = CPoint2D(0, getTitleBarHeight());

	return; // todo







	// render background
	pGFX->drawRectangleFill(vecDrawStartPosition, getSize(), getStyles());

	// render window backgrounds
	string strTabText = "Opened.IMG";
	string strTotalEntriesText = "Total Entries: 0";

	uint32 uiTabTextFontSize = 14;
	uint32 uiTabTextY = vecDrawStartPosition.m_y + 66;
	CGUIStyles styles10;
	styles10.setEntry("text-size", uiTabTextFontSize);
	uint32 uiTabTextWidth = pGFX->getTextSize(strTabText, &styles10).m_x;
	uint32 uiTabPaddingX = 17;
	uint32 uiTabWidth = uiTabTextWidth + (2 * uiTabPaddingX);
	uint32 uiTabTextX = vecDrawStartPosition.m_x + 252 + uiTabPaddingX;

	CGUIStyles styles2;
	styles2.setEntry("fill-colour", CColour(0x73, 0x9B, 0xB2, 0xFF));
	pGFX->drawRectangleFill(CPoint2D(vecDrawStartPosition.m_x + 213, vecDrawStartPosition.m_y), CSize2D(805, 38), &styles2);

	styles2.setEntry("fill-colour", CColour(0x2B, 0x63, 0x81, 0xFF));
	pGFX->drawRectangleFill(CPoint2D(vecDrawStartPosition.m_x + 213, vecDrawStartPosition.m_y + 38), CSize2D(805, 586), &styles2);

	CGUIStyles styles3;
	styles3.setEntry("fill-colour", CColour(0x2B, 0x63, 0x81, 0xFF));
	styles3.setEntry("border-intersection-radius", 20);
	styles3.setEntry("border-colour", CColour(0xFD, 0xFE, 0xFE, 0xFF));
	pGFX->drawRectangle(CPoint2D(vecDrawStartPosition.m_x + 226, vecDrawStartPosition.m_y + 57), CSize2D(777, 528), &styles3);
	
	/*
	todo
	CGUIStyles styles8;
	styles8.setEntry("fill-colour", CColour(0xEC, 0xF3, 0xFD, 0xFF));
	pGFX->drawRectangleFill(CSize2D(vecDrawStartPosition.m_x + 252, vecDrawStartPosition.m_y + 87), CSize2D(732, 480), &styles8);
	*/
	
	CGUIStyles styles4;
	styles4.setEntry("fill-colour-start", CColour(0x54, 0x89, 0xA7, 0xFF));
	styles4.setEntry("fill-colour-stop", CColour(0x31, 0x69, 0x88, 0xFF));
	pGFX->drawRectangleFill(CPoint2D(vecDrawStartPosition.m_x + 252, vecDrawStartPosition.m_y + 61), CSize2D(uiTabWidth, 26), &styles4);
	
	CGUIStyles styles5;
	styles4.setEntry("text-colour", CColour(0xE1, 0xE6, 0xEF, 0xFFF));
	styles4.setEntry("text-size", uiTabTextFontSize);
	pGFX->drawText(CPoint2D(vecDrawStartPosition.m_x + uiTabTextX, uiTabTextY), CSize2D(uiTabWidth, 26), strTabText, &styles5);
	
	CGUIStyles styles6;
	styles4.setEntry("text-colour", CColour(0xE1, 0xE6, 0xEF, 0xFF));
	styles4.setEntry("text-size", 13);
	pGFX->drawText(CPoint2D(vecDrawStartPosition.m_x + 252, vecDrawStartPosition.m_y + 597), CSize2D(200, 20), strTotalEntriesText, &styles6);

	uint32 uiButtonY = vecDrawStartPosition.m_y + 38 + 40;
	CGUIStyles styles7;
	for (uint32 i = 1; i < 14; i++)
	{
		CColour fillColour = (i % 2) == 0 ? CColour(0x1A, 0x3C, 0x4E, 0xFF) : CColour(0x21, 0x4E, 0x67, 0xFF);
		styles7.setEntry("fill-colour", fillColour);
		pGFX->drawRectangleFill(CPoint2D(38, uiButtonY), CSize2D(172, 40), &styles7);
		uiButtonY += 40;
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