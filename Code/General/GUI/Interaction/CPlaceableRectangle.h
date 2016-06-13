#ifndef CPlaceableRectangle_H
#define CPlaceableRectangle_H

#include "Types.h"
#include "CVector2i32.h"
#include "CVector2ui32.h"
#include "Pool/CVectorPool.h"
#include "Event/eEvent.h"
#include "Event/CEventManager.h"
#include "Event/CEventBinder.h"
#include "Event/CInputEventCallbacks.h"
#include "Math/CMathUtility.h"
#include <vector>

class CWindow;

template <class Item>
class CPlaceableRectangle : public CEventBinder, public CInputEventCallbacks
{
public:
	CPlaceableRectangle(void);
	
	void						bindEvents(void);
	void						bindEvents_WhenNotPlacing(void);
	void						bindEvents_WhenPlacing(void);
	
	void						startMovingItem(Item *pItem);
	void						stopMovingItem(void);
	bool						isMovingItem(void) { return m_pItemBeingMoved != nullptr; }
	
	void						startResizingItem(Item *pItem, uint32 uiEdges);
	void						stopResizingItem(void);
	bool						isResizingItem(void) { return m_pItemBeingResized != nullptr; }
	
	void						onLeftMouseDown(CVector2i32& vecCursorPoint);
	void						onLeftMouseUp(CVector2i32& vecCursorPoint);
	void						onMouseMove(CVector2i32& vecCursorPoint);

	void						setItemBeingMoved(Item *pItem) { m_pItemBeingMoved = pItem; }
	Item*						getItemBeingMoved(void) { return m_pItemBeingMoved; }

	void						setItemBeingResized(Item *pItem) { m_pItemBeingResized = pItem; }
	Item*						getItemBeingResized(void) { return m_pItemBeingResized; }
	
	void						setResizingItemEdges(uint32 uiEdges) { m_uiItemResizeEdges = uiEdges; }
	uint32						getResizingItemEdges(void) { return m_uiItemResizeEdges; }

	void						setWindow(CWindow *pWindow) { m_pWindow = pWindow; }
	CWindow*					getWindow(void) { return m_pWindow; }

	void						setItems(CVectorPool<Item*> *pvecItems) { m_pvecItems = pvecItems; }
	CVectorPool<Item*>*			getItems(void) { return m_pvecItems; }

private:
	void						checkToStartMovingOrResizingItem(CVector2i32& vecCursorPoint, uint32 uiOuterSpacing);
	
private:
	Item*						m_pItemBeingMoved;		// the item that is being moved or resized
	Item*						m_pItemBeingResized;	// the item that is being moved or resized
	uint8						m_uiItemResizeEdges;
	CWindow*					m_pWindow;
	CVectorPool<Item*>*			m_pvecItems;
};

template <class Item>
CPlaceableRectangle<Item>::CPlaceableRectangle(void) :
	m_pItemBeingMoved(nullptr),
	m_pItemBeingResized(nullptr),
	m_uiItemResizeEdges(0),
	m_pWindow(0),
	m_pvecItems(nullptr)
{
}

template <class Item>
void					CPlaceableRectangle<Item>::bindEvents(void)
{
	bindEvents_WhenNotPlacing();
}

template <class Item>
void					CPlaceableRectangle<Item>::bindEvents_WhenNotPlacing(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseDown, this, nullptr, -10));
}

template <class Item>
void					CPlaceableRectangle<Item>::bindEvents_WhenPlacing(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseUp, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onMouseMove, this));
}

template <class Item>
void					CPlaceableRectangle<Item>::startMovingItem(Item *pItem)
{
	setItemBeingMoved(pItem);
	bindEvents_WhenPlacing();
}

template <class Item>
void					CPlaceableRectangle<Item>::stopMovingItem(void)
{
	unbindEvents();
	setItemBeingMoved(nullptr);
	bindEvents_WhenNotPlacing();
}

template <class Item>
void					CPlaceableRectangle<Item>::startResizingItem(Item *pItem, uint32 uiEdges)
{
	setItemBeingResized(pItem);
	setResizingItemEdges(uiEdges);
	bindEvents_WhenPlacing();
}

template <class Item>
void					CPlaceableRectangle<Item>::stopResizingItem(void)
{
	unbindEvents();
	setItemBeingResized(nullptr);
	setResizingItemEdges(0);
	bindEvents_WhenNotPlacing();
}

template <class Item>
void					CPlaceableRectangle<Item>::checkToStartMovingOrResizingItem(CVector2i32& vecCursorPoint, uint32 uiOuterSpacing)
{
	uint32 uiRectangleEdges;
	for(Item *pItem : getItems()->getEntries())
	{
		if(pItem->isPointInBoundingRectangle(vecCursorPoint, uiOuterSpacing))
		{
			uiRectangleEdges = CMathUtility::getRectangleResizeEdges(vecCursorPoint, pItem->getBoundingRectanglePosition(), pItem->getBoundingRectangleSize(), uiOuterSpacing);
			if(uiRectangleEdges == 0)
			{
				// move item
				startMovingItem(pItem);
				CEventManager::getInstance()->setEventHogged(true);
				break;
			}
			else
			{
				// resize item
				startResizingItem(pItem, uiRectangleEdges);
				CEventManager::getInstance()->setEventHogged(true);
				break;
			}
		}
	}
}

template <class Item>
void					CPlaceableRectangle<Item>::onLeftMouseDown(CVector2i32& vecCursorPoint)
{
	uint32 uiOuterSpacing = 1;
	checkToStartMovingOrResizingItem(vecCursorPoint, uiOuterSpacing);
}

template <class Item>
void					CPlaceableRectangle<Item>::onLeftMouseUp(CVector2i32& vecCursorPoint)
{
	if (isMovingItem())
	{
		stopMovingItem();
	}
	if (isResizingItem())
	{
		stopResizingItem();
	}
}

template <class Item>
void					CPlaceableRectangle<Item>::onMouseMove(CVector2i32& vecCursorPoint)
{
	if (isMovingItem())
	{
		CVector2i32
			vecItemPositionChange = CEventManager::getInstance()->getScreenCursorMoveDifference();
		getItemBeingMoved()->onMoveItem(vecItemPositionChange);
		getWindow()->setMarkedToRedraw(true);
	}
	if (isResizingItem())
	{
		CVector2i32
			vecCursorChange = CEventManager::getInstance()->getScreenCursorMoveDifference(),
			vecItemPositionChange,
			vecItemSizeChange;
		CMathUtility::getResizePositionAndSizeChange(vecCursorChange, getResizingItemEdges(), vecItemPositionChange, vecItemSizeChange);
		getItemBeingResized()->onResizeItem(vecItemPositionChange, vecItemSizeChange);
		getWindow()->setMarkedToRedraw(true);
	}
}

#endif