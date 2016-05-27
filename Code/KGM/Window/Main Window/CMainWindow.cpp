#include "CMainWindow.h"
#include "CKGM.h"
#include "Task/CTaskManager.h"
#include "Task/CTaskDispatchManager.h"
#include "Math/CMathUtility.h"
#include "Event/CEventManager.h"
#include "CVector2i32.h"
#include "String/CStringUtility.h"
#include "Path/CPathUtility.h"
#include "File/CFileUtility.h"
#include "Window/Window Types/Tabs/CEntryListWindowTab.h"
#include "GDIPlus/CGDIPlusUtility.h"

using namespace std;

// input
void					CMainWindow::onMouseDown(CVector2ui32& vecCursorPosition)
{
	CWindow::onMouseDown(vecCursorPosition);

	if (CEventManager::getInstance()->isEventHogged())
	{
		return;
	}

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

	// store cursor position
	POINT point;
	GetCursorPos(&point);
	CEventManager::getInstance()->setLastCursorPosition(CVector2ui32(point.x, point.y));
}

void					CMainWindow::onMouseUp(CVector2ui32& vecCursorPosition)
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

void					CMainWindow::onMouseMove(CVector2ui32& vecCursorPosition)
{
	if (isMovingWindow())
	{
		POINT point;
		GetCursorPos(&point);
		vecCursorPosition = CVector2ui32(point.x, point.y);

		RECT rect;
		GetWindowRect(getWindowHandle(), &rect);

		CVector2ui32
			vecPreviousCursorPosition = CEventManager::getInstance()->getLastCursorPosition();
		CVector2i32
			vecCursorDiff;

		vecCursorDiff.m_x = ((int32)vecCursorPosition.m_x) - ((int32)vecPreviousCursorPosition.m_x);
		vecCursorDiff.m_y = ((int32)vecCursorPosition.m_y) - ((int32)vecPreviousCursorPosition.m_y);

		rect.left += vecCursorDiff.m_x;
		rect.top += vecCursorDiff.m_y;

		SetWindowPos(getWindowHandle(), NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		setPosition(CVector2ui32(rect.left, rect.top));
		//MoveWindow(hwndWindow, rect.left, rect.top, rect.right, rect.bottom, false);
		//InvalidateRect(hwndWindow, &rect, false);

		CEventManager::getInstance()->setLastCursorPosition(vecCursorPosition);
		return;
	}
	else if (isResizingWindow())
	{
		POINT point;
		GetCursorPos(&point);
		vecCursorPosition = CVector2ui32(point.x, point.y);

		RECT rect;
		GetWindowRect(getWindowHandle(), &rect);

		CVector2ui32
			vecPreviousCursorPosition = CEventManager::getInstance()->getLastCursorPosition();
		CVector2i32
			vecCursorDiff;

		vecCursorDiff.m_x = ((int32)vecCursorPosition.m_x) - ((int32)vecPreviousCursorPosition.m_x);
		vecCursorDiff.m_y = ((int32)vecCursorPosition.m_y) - ((int32)vecPreviousCursorPosition.m_y);

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

		CEventManager::getInstance()->setLastCursorPosition(vecCursorPosition);
		return;
	}
}

void					CMainWindow::onDoubleLeftClick(CVector2ui32& vecCursorPosition)
{
	RECT rect;
	GetWindowRect(getWindowHandle(), &rect);
	CVector2ui32 vecMainWindowTitleBarSize(rect.right - rect.left, 35);

	if (CMathUtility::isPointInRectangle(vecCursorPosition, CVector2ui32(0, 0), vecMainWindowTitleBarSize))
	{
		setMaximized(!isMaximized());
	}
}

// render
void					CMainWindow::render(void)
{
	// render background
	CGDIPlusUtility::drawRectangleFill(CVector2ui32(0,0), getSize(), getBackgroundColour());

	// render title bar
	string strTitleBarText = "Komodo Game Manager";
	uint32 uiTitleBarTextFontSize = 30;
	uint32 uiTitleBarTextWidth = CGDIPlusUtility::getTextWidth(strTitleBarText, uiTitleBarTextFontSize);
	uint32 uiTitleBarTextX = (getSize().m_x / 2) - (uiTitleBarTextWidth / 2);

	CGDIPlusUtility::drawRectangleFill(CVector2ui32(0, 0), CVector2ui32(getSize().m_x, getTitleBarHeight()), 0x387EA3FF);
	CGDIPlusUtility::drawText(CVector2ui32(uiTitleBarTextX, 1), CVector2ui32(uiTitleBarTextWidth, getTitleBarHeight()), strTitleBarText, 0xE1E6EFFF, uiTitleBarTextFontSize, false);
}

// input processing
void					CMainWindow::onDropFiles(vector<string>& vecPaths)
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

// math
/*
todo

CVector2ui32							CMainWindow::getPosition(void)
{
	RECT rect;
	GetWindowRect(getWindowHandle(), &rect);
	return CVector2ui32(rect.left, rect.top);
}
CVector2ui32							CMainWindow::getSize(void)
{
	RECT rect;
	GetWindowRect(getWindowHandle(), &rect);
	return CVector2ui32(rect.right - rect.left, rect.bottom - rect.top);
}
*/