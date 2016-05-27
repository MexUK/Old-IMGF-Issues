#ifndef CEntryViewerManager_H
#define CEntryViewerManager_H

#include <Windows.h>
#include "CManager.h"
#include "TXD/eTXDRasterDataFormat.h"
#include "Image/eDXTCompressionType.h"
#include "d3d9.h"
#include <string>
#include <vector>

LRESULT CALLBACK	WndProc_EntryViewer(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void				onPaint(HWND hwnd);

class CIMGEntry;
class CTextureViewer;
class CCollisionViewer;

class CEntryViewerManager : public CManager
{
public:
	CEntryViewerManager(void);
	~CEntryViewerManager(void);

	void									init(void);
	void									uninit(void);

	CTextureViewer*							getTextureViewer(void) { return m_pTextureViewer; }
	CCollisionViewer*						getCollisionViewer(void) { return m_pCollisionViewer; }

private:
	CTextureViewer*							m_pTextureViewer;
	CCollisionViewer*						m_pCollisionViewer;
};

#endif