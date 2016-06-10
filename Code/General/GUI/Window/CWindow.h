#ifndef CWindow_H
#define CWindow_H

#include <Windows.h>
#include <gdiplus.h>
#include "Types.h"
#include "CVector2ui32.h"
#include "Pool/CVectorPool.h"
#include "Event/CEventType.h"
#include "Event/CEventBoundFunction.h"
#include "Event/CEventBinder.h"
#include "GUI/Group/CControlGroup.h"
#include "GUI/Control/CGUIControl.h"
#include "GUI/Styles/CGUIStyleableEntity.h"

class CRadioControl;
class CDropTarget;
class CGUIStyles;
class CWindow;

class CWindow : public CEventType, public CEventBinder, public CGUIStyleableEntity, public CVectorPool<CControlGroup*>
{
public:
	CWindow(void);
	~CWindow(void);
	
	void									unload(void);

	void									bindEvents(void);
	void									bindAllEvents(void);
	void									unbindAllEvents(void);
	
	CEventBoundFunction*					bindEvent(uint32 uiEventId, void(*pFunction)(void*), void *pTriggerArgument = nullptr, int32 iZOrder = 0);
	CEventBoundFunction*					bindEvent(uint32 uiEventId, void(*pFunction)(void*, void*), void *pTriggerArgument = nullptr, int32 iZOrder = 0);
	bool									triggerEvent(uint32 uiEventId, void *pTriggerArgument = nullptr);

	void									onMouseDown(CVector2i32& vecCursorPosition);
	void									onMouseUp(CVector2i32& vecCursorPosition);
	void									onMouseMove(CVector2i32& vecCursorPosition);
	void									onDoubleLeftClick(CVector2i32& vecCursorPosition);

	virtual void							render(void);
	void									onRenderFromWMPaint(void);

	CControlGroup*							addControlGroup(bool bEnabled = true);
	CControlGroup*							addControlGroup(CWindow *pWindow, bool bEnabled = true);

	void									unmarkRadios(CRadioControl *pRadio);

	void									setOpenLastFilename(std::string strFileName);
	void									clearOpenLastFilename(void);

	void									setWindowHandle(HWND hwndWindow) { m_hwndWindow = hwndWindow; }
	HWND									getWindowHandle(void) { return m_hwndWindow; }

	void									setMarkedToRedraw(bool bMarkedToRedraw) { m_bMarkedToRedraw = bMarkedToRedraw; }
	bool									isMarkedToRedraw(void) { return m_bMarkedToRedraw; }
	
	void									setFocusedControl(CGUIControl *pWindowControl) { m_pFocusedControl = pWindowControl; }
	CGUIControl*							getFocusedControl(void) { return m_pFocusedControl; }

	void									setPosition(CVector2i32& vecPosition) { m_vecPosition = vecPosition; m_vecPreviousPosition = vecPosition; }
	CVector2i32&							getPosition(void) { return m_vecPosition; }

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

	void									setPreviousPosition(CVector2i32& vecPreviousPosition) { m_vecPreviousPosition = vecPreviousPosition; }
	CVector2i32&							getPreviousPosition(void) { return m_vecPreviousPosition; }

private:
	HWND									m_hwndWindow;
	CWindow*								m_pParentWindow;
	CGUIControl*							m_pFocusedControl;
	CVector2i32								m_vecPosition;
	CVector2ui32							m_vecSize;
	uint32									m_uiWindowResizeEdges;
	CDropTarget*							m_pDropTarget;
	uint32									m_uiTitleBarHeight;
	uint8									m_bMovingWindow			: 1;
	uint8									m_bResizingWindow		: 1;
	uint8									m_bMarkedToRedraw		: 1;
	uint8									m_bMaximized			: 1;
	CVector2i32								m_vecPreviousPosition;

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