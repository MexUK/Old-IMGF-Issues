#ifndef CWindow_H
#define CWindow_H

#include <Windows.h>
#include <gdiplus.h>
#include "Types.h"
#include "CVector2ui32.h"
#include "Pool/CVectorPool.h"
#include "GUI/Control/CWindowControl.h"
#include "Event/CEventType.h"
#include "Event/CEventBoundFunction.h"
#include "Event/CEventBinder.h"

class CWindowControl_Radio;
class CDropTarget;

class CWindow : public CEventType, public CEventBinder
{
public:
	CWindow(void);
	~CWindow(void);
	
	void									unload(void);

	void									bindEvents(void);
	void									bindAllEvents(void);
	void									unbindAllEvents(void);

	void									onMouseDown(CVector2ui32& vecCursorPosition);
	void									onMouseUp(CVector2ui32& vecCursorPosition);
	void									onMouseMove(CVector2ui32& vecCursorPosition);
	void									onDoubleLeftClick(CVector2ui32& vecCursorPosition);

	virtual void							render(void);
	void									onRenderFromWMPaint(void);

	void									uncheckRadios(CWindowControl_Radio *pRadio);

	void									setOpenLastFilename(std::string strFileName);
	void									clearOpenLastFilename(void);

	void									setWindowHandle(HWND hwndWindow) { m_hwndWindow = hwndWindow; }
	HWND									getWindowHandle(void) { return m_hwndWindow; }

	void									setHDC(HDC hdc) { m_hdc = hdc; }
	HDC										getHDC(void) { return m_hdc; }

	void									setMarkedToRedraw(bool bMarkedToRedraw) { m_bMarkedToRedraw = bMarkedToRedraw; }
	bool									isMarkedToRedraw(void) { return m_bMarkedToRedraw; }
	
	void									setFocusedControl(CWindowControl *pWindowControl) { m_pFocusedControl = pWindowControl; }
	CWindowControl*							getFocusedControl(void) { return m_pFocusedControl; }

	void									setBackgroundColour(uint32 uiBackgroundColour) { m_uiBackgroundColour = uiBackgroundColour; }
	uint32									getBackgroundColour(void) { return m_uiBackgroundColour; }

	void									setPosition(CVector2ui32& vecPosition) { m_vecPosition = vecPosition; m_vecPreviousPosition = vecPosition; }
	CVector2ui32&							getPosition(void) { return m_vecPosition; }

	void									setSize(CVector2ui32& vecSize) { m_vecSize = vecSize; }
	CVector2ui32&							getSize(void) { return m_vecSize; }

	void									setMovingWindow(bool bMovingMainWindow) { m_bMovingWindow = bMovingMainWindow; }
	bool									isMovingWindow(void) { return m_bMovingWindow; }

	void									setResizingWindow(bool bResizingWindow) { m_bResizingWindow = bResizingWindow; }
	bool									isResizingWindow(void) { return m_bResizingWindow; }

	void									setWindowResizeEdges(uint32 uiMainWindowResizeEdges) { m_uiWindowResizeEdges = uiMainWindowResizeEdges; }
	uint32									getWindowResizeEdges(void) { return m_uiWindowResizeEdges; }

	void									setParentWindow(CWindow *pParentWindow) { m_pParentWindow = pParentWindow; }
	CWindow*								getParentWindow(void) { return m_pParentWindow; }

	void									setTitleBarHeight(uint32 uiTitleBarHeight) { m_uiTitleBarHeight = uiTitleBarHeight; }
	uint32									getTitleBarHeight(void) { return m_uiTitleBarHeight; }

	void									setMaximized(bool bMaximized);
	bool									isMaximized(void) { return m_bMaximized; }

	void									setPreviousPosition(CVector2ui32& vecPreviousPosition) { m_vecPreviousPosition = vecPreviousPosition; }
	CVector2ui32&							getPreviousPosition(void) { return m_vecPreviousPosition; }

	CVectorPool<CWindowControl*>&			getControls(void) { return m_vecControls; }
	
private:
	HWND									m_hwndWindow;
	HDC										m_hdc;
	CWindow*								m_pParentWindow;
	CWindowControl*							m_pFocusedControl;
	CVector2ui32							m_vecPosition;
	CVector2ui32							m_vecSize;
	uint32									m_uiBackgroundColour;
	uint32									m_uiWindowResizeEdges;
	CDropTarget*							m_pDropTarget;
	uint32									m_uiTitleBarHeight;
	uint8									m_bMovingWindow			: 1;
	uint8									m_bResizingWindow		: 1;
	uint8									m_bMarkedToRedraw		: 1;
	uint8									m_bMaximized			: 1;
	CVector2ui32							m_vecPreviousPosition;
	CVectorPool<CWindowControl*>			m_vecControls;

	/*
	todo
	HMENU						m_hMenu_File;
	HMENU						m_hSubMenu_File_OpenRecent;
	HMENU						m_hMenu_Entry_Sort;
	HMENU						m_hSubMenu_File_Sessions;

	std::unordered_map<uint32, CRWVersion*>			m_umapMenuItemMapping_SelectRWVersion; // todo - move this shit to like CMenuManager
	std::unordered_map<uint32, CRWVersion*>			m_umapMenuItemMapping_ConvertDFFtoRWVersion;
	std::unordered_map<uint32, CRWVersion*>			m_umapMenuItemMapping_FilterRWVersion;
	std::unordered_map<uint32, eCOLVersion>			m_umapMenuItemMapping_FilterCOLVersion;
	std::unordered_map<uint32, CRWVersion*>			m_umapMenuItemMapping_ConvertTXDtoRWVersion;
	std::unordered_map<uint32, CRasterDataFormat*>	m_umapMenuItemMapping_ConvertTXDtoTextureFormat;
	std::unordered_map<uint32, CCOLVersion*>		m_umapMenuItemMapping_ConvertCOLtoCOLVersion;
	std::unordered_map<uint32, CRWVersion*>			m_umapMenuItemMapping_ConvertDFFtoRWVersion_DragDrop;
	std::unordered_map<uint32, CRWVersion*>			m_umapMenuItemMapping_ConvertTXDtoRWVersion_DragDrop;
	std::unordered_map<uint32, CRasterDataFormat*>	m_umapMenuItemMapping_ConvertTXDtoTextureFormat_DragDrop;
	std::unordered_map<uint32, CCOLVersion*>		m_umapMenuItemMapping_ConvertCOLtoCOLVersion_DragDrop;

	std::unordered_map<uint32, eCOLVersion>		m_umapFilterMapping_COLVersion;
	std::unordered_map<uint32, eRWVersion>		m_umapFilterMapping_RWVersion;
	int											m_iFilterMapping_UnknownVersion;
	*/
};

#endif