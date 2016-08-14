#pragma warning(disable : 4005)

#include "CGUIUtility.h"
#include "String/CStringUtility.h"
#include "Path/CPathUtility.h"
#include "Shlobj.h"

using namespace std;

HWND				CGUIUtility::m_hParentWindowHwnd;

vector<string>		CGUIUtility::openFileDialog(string strInitialDir, string strExtensionFilters, bool bAllowMultiSelect, string strDefaultFileName)
{
	const uint32 uiDefaultFileNameBufferSize = 1000 * (_MAX_PATH + 1); // todo - lol? - no wonder its been lagging

	wchar_t szInitialDirBuffer[_MAX_PATH];
	wchar_t szDefaultFileNameBuffer[uiDefaultFileNameBufferSize];
	wchar_t szDefaultExtensionBuffer[256];
	wchar_t szExtensionFilters[256];
	//wchar_t szFilePath[256]; // todo - fuck is this?

	wchar_t wuiZeroChar = 0;
	wmemchr(szInitialDirBuffer, wuiZeroChar, _MAX_PATH);
	wmemchr(szDefaultFileNameBuffer, wuiZeroChar, uiDefaultFileNameBufferSize);
	wmemchr(szDefaultExtensionBuffer, wuiZeroChar, 256);
	wmemchr(szExtensionFilters, wuiZeroChar, 256);

	vector<string> vecExtensionFilters = CStringUtility::split(strExtensionFilters, ",");
	string strExtensionFilters2 = "";
	string strZero = string(1, 0);
	string strPart1, strPart2;
	uint32 uiExtensionFiltersLength = 0;
	for (uint32 i = 0, j = vecExtensionFilters.size(); i < j; i++)
	{
		strPart1 = vecExtensionFilters[i] + " files (*." + vecExtensionFilters[i] + ")";
		strPart2 = "*." + vecExtensionFilters[i];
		strExtensionFilters2 += strPart1 + strZero + strPart2 + strZero;
		uiExtensionFiltersLength += strPart1.length() + strPart2.length() + 2;
	}
	strExtensionFilters2 += strZero;
	uiExtensionFiltersLength++;

	wcscpy_s(szInitialDirBuffer, CStringUtility::convertStdStringToStdWString(strInitialDir).c_str());
	wcscpy_s(szDefaultFileNameBuffer, CStringUtility::convertStdStringToStdWString(strDefaultFileName).c_str());
	wcscpy_s(szDefaultExtensionBuffer, CStringUtility::convertStdStringToStdWString(CStringUtility::toLowerCase(CPathUtility::getFileExtension(strDefaultFileName))).c_str());
	wcscpy_s(szExtensionFilters, uiExtensionFiltersLength, CStringUtility::convertStdStringToStdWString(strExtensionFilters2).c_str());
	//wcscpy_s(szFilePath, CStringUtility::convertStdStringToStdWString(CStringUtility::replace(CPathUtility::addSlashToEnd(strInitialDir), "/", "\\") + strDefaultFileName).c_str()); // todo - ?

	OPENFILENAME ofn;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = getParentWindowHwnd();
	ofn.hInstance = NULL; // GetWindowLong(getParentWindowHwnd(), GWL_HINSTANCE);
	ofn.lpstrFilter = NULL; // todo szExtensionFilters;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szDefaultFileNameBuffer;
	ofn.nMaxFile = uiDefaultFileNameBufferSize;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = szInitialDirBuffer;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | (bAllowMultiSelect ? OFN_ALLOWMULTISELECT : 0);
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = szDefaultExtensionBuffer;
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = L"";
	ofn.pvReserved = 0;
	ofn.dwReserved = 0;
	ofn.FlagsEx = 0;

	vector<string> vecFilePaths;

	BOOL bFilesAreSelected = GetOpenFileName(&ofn);
	if (bFilesAreSelected == FALSE)
	{
		return vecFilePaths;
	}

	uint32 uiStringStartByteIndex = 0;
	WCHAR wszString[_MAX_PATH];
	wmemchr(wszString, wuiZeroChar, _MAX_PATH);
	wstring wstrString;
	bool bPreviousByteWasZero = false; // todo - move to like CPathUtility::getFilePathsFromNullSeparatedEntries() and vice versa
	for (uint32 uiByteIndex = 0; ; uiByteIndex++)
	{
		if (ofn.lpstrFile[uiByteIndex] == 0)
		{
			if (bPreviousByteWasZero)
			{
				break;
			}
			wmemcpy(wszString, ofn.lpstrFile + uiStringStartByteIndex, (uiByteIndex - uiStringStartByteIndex) + 1);
			wstrString = wstring(wszString);
			vecFilePaths.push_back(CStringUtility::convertStdWStringToStdString(wstrString));
			uiStringStartByteIndex = uiByteIndex + 1;
			bPreviousByteWasZero = true;
		}
		else
		{
			bPreviousByteWasZero = false;
		}
	}

	return vecFilePaths;


	/*
	todo

	wchar_t szInitialDirBuffer[_MAX_PATH];
	wchar_t szDefaultFileNameBuffer[1000 * (_MAX_PATH + 1)];
	wchar_t szDefaultExtensionBuffer[256];
	wchar_t szExtensionFilters[256];
	//wchar_t szFilePath[256];

	vector<string> vecExtensionFilters = CStringUtility::split(strExtensionFilters, ",");
	string strExtensionFilters2 = "";
	for (uint32 i = 0, j = vecExtensionFilters.size(); i < j; i++)
	{
		strExtensionFilters2 += vecExtensionFilters[i] + " files (*." + vecExtensionFilters[i] + ")|*." + vecExtensionFilters[i] + "|";
	}
	strExtensionFilters2 += "|";

	wcscpy_s(szInitialDirBuffer, CStringUtility::convertStdStringToStdWString(strInitialDir).c_str());
	wcscpy_s(szDefaultFileNameBuffer, CStringUtility::convertStdStringToStdWString(strDefaultFileName).c_str());
	wcscpy_s(szDefaultExtensionBuffer, CStringUtility::convertStdStringToStdWString(CStringUtility::toLowerCase(CPathUtility::getFileExtension(strDefaultFileName))).c_str());
	wcscpy_s(szExtensionFilters, CStringUtility::convertStdStringToStdWString(strExtensionFilters2).c_str());
	//wcscpy_s(szFilePath, CStringUtility::convertStdStringToStdWString(CStringUtility::replace(CPathUtility::addSlashToEnd(strInitialDir), "/", "\\") + strDefaultFileName).c_str());

	CFileDialog dlg(
		TRUE,
		szDefaultExtensionBuffer,
		szDefaultFileNameBuffer,
		OFN_FILEMUSTEXIST | (bAllowMultiSelect ? OFN_ALLOWMULTISELECT : 0),
		szExtensionFilters,
		CWnd::FromHandle(getParentWindowHwnd()),
		0);
	dlg.m_ofn.lpstrInitialDir = szInitialDirBuffer;
	dlg.m_ofn.lpstrFile = szDefaultFileNameBuffer;
	dlg.m_ofn.nMaxFile = 1000 * (_MAX_PATH + 1);

	vector<string> vecFilePaths;
	CString filename;
	if (dlg.DoModal() == IDOK)
	{
		// get the list of files
		POSITION pos = dlg.GetStartPosition();

		while (pos)
		{
			// one filename retrieved per loop iteration
			filename = dlg.GetNextPathName(pos);
			vecFilePaths.push_back(CStringUtility::convertCStringToStdString(filename));
		}
	}

	return vecFilePaths;
	*/
}

string				CGUIUtility::saveFileDialog(string strInitialDir, string strExtensionFilters, string strDefaultFileName)
{
	const uint32 uiDefaultFileNameBufferSize = 1000 * (_MAX_PATH + 1); // todo - lol? - no wonder its been lagging

	wchar_t szInitialDirBuffer[_MAX_PATH];
	wchar_t szDefaultFileNameBuffer[uiDefaultFileNameBufferSize];
	wchar_t szDefaultExtensionBuffer[256];
	wchar_t szExtensionFilters[256];
	//wchar_t szFilePath[256]; // todo - fuck is this?

	wchar_t wuiZeroChar = 0;
	wmemchr(szInitialDirBuffer, wuiZeroChar, _MAX_PATH);
	wmemchr(szDefaultFileNameBuffer, wuiZeroChar, uiDefaultFileNameBufferSize);
	wmemchr(szDefaultExtensionBuffer, wuiZeroChar, 256);
	wmemchr(szExtensionFilters, wuiZeroChar, 256);

	vector<string> vecExtensionFilters = CStringUtility::split(strExtensionFilters, ",");
	string strExtensionFilters2 = "*."+CStringUtility::join(vecExtensionFilters, ";*.");
	uint32 uiExtensionFiltersLength = strExtensionFilters2.length();
	/*
	string strExtensionFilters2 = "";
	string strZero = string(1, 0);
	string strPart1, strPart2;
	uint32 uiExtensionFiltersLength = 0;
	for (uint32 i = 0, j = vecExtensionFilters.size(); i < j; i++)
	{
		strPart1 = vecExtensionFilters[i] + " files (*." + vecExtensionFilters[i] + ")";
		strPart2 = "*." + vecExtensionFilters[i];
		strExtensionFilters2 += strPart1 + strZero + strPart2 + strZero;
		uiExtensionFiltersLength += strPart1.length() + strPart2.length() + 2;
	}
	strExtensionFilters2 += strZero;
	uiExtensionFiltersLength++;
	*/

	wcscpy_s(szInitialDirBuffer, CStringUtility::convertStdStringToStdWString(strInitialDir).c_str());
	wcscpy_s(szDefaultFileNameBuffer, CStringUtility::convertStdStringToStdWString(strDefaultFileName).c_str());
	wcscpy_s(szDefaultExtensionBuffer, CStringUtility::convertStdStringToStdWString(CStringUtility::toLowerCase(CPathUtility::getFileExtension(strDefaultFileName))).c_str());
	wcscpy_s(szExtensionFilters, CStringUtility::convertStdStringToStdWString(strExtensionFilters2).c_str());
	//wcscpy_s(szFilePath, CStringUtility::convertStdStringToStdWString(CStringUtility::replace(CPathUtility::addSlashToEnd(strInitialDir), "/", "\\") + strDefaultFileName).c_str()); // todo - ?

	OPENFILENAME ofn;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = getParentWindowHwnd();
	ofn.hInstance = NULL; // GetWindowLong(getParentWindowHwnd(), GWL_HINSTANCE);
	ofn.lpstrFilter = szExtensionFilters;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szDefaultFileNameBuffer;
	ofn.nMaxFile = uiDefaultFileNameBufferSize;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = szInitialDirBuffer;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = szDefaultExtensionBuffer;
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = L"";
	ofn.pvReserved = 0;
	ofn.dwReserved = 0;
	ofn.FlagsEx = 0;

	BOOL bFilesAreSelected = GetSaveFileName(&ofn);
	if (bFilesAreSelected == FALSE)
	{
		return "";
	}

	uint32 uiStringStartByteIndex = 0;
	WCHAR wszString[_MAX_PATH];
	wmemchr(wszString, wuiZeroChar, _MAX_PATH);
	wstring wstrString;
	bool bPreviousByteWasZero = false; // todo - move to like CPathUtility::getFilePathsFromNullSeparatedEntries() and vice versa
	for (uint32 uiByteIndex = 0; ; uiByteIndex++)
	{
		if (ofn.lpstrFile[uiByteIndex] == 0)
		{
			if (bPreviousByteWasZero)
			{
				break;
			}
			wmemcpy(wszString, ofn.lpstrFile + uiStringStartByteIndex, (uiByteIndex - uiStringStartByteIndex) + 1);
			wstrString = wstring(wszString);
			return CStringUtility::convertStdWStringToStdString(wstrString);

			//vecFilePaths.push_back();
			uiStringStartByteIndex = uiByteIndex + 1;
			bPreviousByteWasZero = true;
		}
		else
		{
			bPreviousByteWasZero = false;
		}
	}

	return "";










	//return "";
	/*
	todo

	wchar_t szInitialDirBuffer[_MAX_PATH];
	wchar_t szDefaultFileNameBuffer[_MAX_FNAME + _MAX_EXT];
	wchar_t szDefaultExtensionBuffer[256];
	wchar_t szExtensionFilters[256];
	//wchar_t szFilePath[256];

	vector<string> vecExtensionFilters = CStringUtility::split(strExtensionFilters, ",");
	string strExtensionFilters2 = "";
	for (uint32 i = 0, j = vecExtensionFilters.size(); i < j; i++)
	{
		strExtensionFilters2 += vecExtensionFilters[i] + " files (*." + vecExtensionFilters[i] + ")|*." + vecExtensionFilters[i] + "|";
	}
	strExtensionFilters2 += "|";

	wcscpy_s(szInitialDirBuffer, CStringUtility::convertStdStringToStdWString(strInitialDir).c_str());
	wcscpy_s(szDefaultFileNameBuffer, CStringUtility::convertStdStringToStdWString(strDefaultFileName).c_str());
	wcscpy_s(szDefaultExtensionBuffer, CStringUtility::convertStdStringToStdWString(CStringUtility::toLowerCase(CPathUtility::getFileExtension(strDefaultFileName))).c_str());
	wcscpy_s(szExtensionFilters, CStringUtility::convertStdStringToStdWString(strExtensionFilters2).c_str());
	//wcscpy_s(szFilePath, CStringUtility::convertStdStringToStdWString(CStringUtility::replace(CPathUtility::addSlashToEnd(strInitialDir), "/", "\\") + strDefaultFileName).c_str());

	CFileDialog dlg(
		FALSE,
		szDefaultExtensionBuffer,
		szDefaultFileNameBuffer,
		OFN_OVERWRITEPROMPT,
		szExtensionFilters,
		NULL,
		0);
	dlg.m_ofn.lpstrInitialDir = szInitialDirBuffer;
	//dlg.m_ofn.lpstrFile = szFilePath;
	
	CString filename;
	if (dlg.DoModal() == IDOK)
	{
		// get the list of files
		POSITION pos = dlg.GetStartPosition();

		if (pos)
		{
			// one filename retreived for save file window
			filename = dlg.GetNextPathName(pos);
			return CStringUtility::convertCStringToStdString(filename);
		}
	}
	return "";
	*/
}

string				CGUIUtility::chooseFolderDialog(string strTitle, string strInitialDir)
{
	return CGUIUtility::chooseFolderDialog(CGUIUtility::getParentWindowHwnd(), strTitle, strInitialDir);
}
string				CGUIUtility::chooseFolderDialog(HWND hOwnerHwnd, string strTitle, string strInitialDir)
{
	wstring szTitle = CStringUtility::convertStdStringToStdWString(strTitle);
	wstring wstrInitialDir = CStringUtility::convertStdStringToStdWString(CStringUtility::replace(strInitialDir, "/", "\\"));

	BROWSEINFO bi = { 0 };
	bi.hwndOwner = hOwnerHwnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = szTitle.c_str();
	bi.ulFlags = BIF_USENEWUI;
	bi.lpfn = onChooseFolderDialogInit;
	bi.lParam = (LPARAM)wstrInitialDir.c_str();
	bi.iImage = 0;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != 0)
	{
		wchar_t szFolderPath[MAX_PATH];
		if (SHGetPathFromIDList(pidl, szFolderPath))
		{
			return CStringUtility::convertStdWStringToStdString(szFolderPath);
		}
	}

	return "";
}

int CALLBACK		CGUIUtility::onChooseFolderDialogInit(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	// If the BFFM_INITIALIZED message is received
	// set the path to the start path.
	switch (uMsg)
	{
		case BFFM_INITIALIZED:
		{
			if (NULL != lpData)
			{
				SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
			}
		}
	}

	return 0; // The function should always return 0.
}

uint32		CGUIUtility::showMessage(string strMessage, string strTitle, uint32 uiButtonType)
{
	return MessageBox(getParentWindowHwnd(), CStringUtility::convertStdStringToStdWString(strMessage).c_str(), CStringUtility::convertStdStringToStdWString(strTitle).c_str(), uiButtonType);
}

void				CGUIUtility::setMenuText(HMENU hMenu, uint32 uiControlIndex, string strText)
{
	MENUITEMINFOA menuitem = { sizeof(MENUITEMINFOA) };
	menuitem.fMask = MIIM_TYPE | MIIM_DATA;
	GetMenuItemInfoA(hMenu, uiControlIndex, false, &menuitem);
	menuitem.dwTypeData = (LPSTR)strText.c_str();
	SetMenuItemInfoA(hMenu, uiControlIndex, false, &menuitem);
}

string				CGUIUtility::getEditText(uint32 uiControlIndex)
{
	return ""; // todo
	/*
	CString cstr;
	((CEdit*)GetDlgItem(getParentWindowHwnd(), uiControlIndex))->GetWindowTextW(cstr);
	return CStringUtility::convertCStringToStdString(cstr);
	*/
}

bool				CGUIUtility::isControlKeyDown(void)
{
	return (GetKeyState(VK_CONTROL) & 0x8000) == 0x8000;
}

bool				CGUIUtility::isShiftKeyDown(void)
{
	return (GetKeyState(VK_SHIFT) & 0x8000) == 0x8000;
}

bool				CGUIUtility::isAltKeyDown(void)
{
	return (GetKeyState(VK_MENU) & 0x8000) == 0x8000;
}

bool				CGUIUtility::isCapsLockOn(void)
{
	return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

bool				CGUIUtility::isTextInputUppercase(void)
{
	bool
		bCapsLockIsOn = isCapsLockOn(),
		bShiftIsDown = isShiftKeyDown();
	return (bCapsLockIsOn && !bShiftIsDown) || (!bCapsLockIsOn && bShiftIsDown);
}

string				CGUIUtility::getTextInInputCase(string& strText)
{
	if (CGUIUtility::isTextInputUppercase())
	{
		return CStringUtility::toUpperCase(strText);
	}
	else
	{
		return CStringUtility::toLowerCase(strText);
	}
}