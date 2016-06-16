#include "CGUIItem.h"
#include "GUI/Layer/CGUILayer.h"
#include "GUI/Window/CWindow.h"
#include "GUI/Styles/CGUIStyles.h"
#include "Math/CMathUtility.h"
#include "Event/CEventManager.h"

CGUIItem::CGUIItem(void) :
	m_pLayer(nullptr)
{
}

// item placement
void									CGUIItem::onMoveItem(CVector2i32& vecItemPositionChange)
{
	moveItem(vecItemPositionChange); // todo - rename moveItem to moveItem
}

void									CGUIItem::onResizeItem(CVector2i32& vecItemPositionChange, CVector2i32& vecItemSizeChange)
{
	moveItem(vecItemPositionChange);
	resizeItemViaOffsets(vecItemSizeChange);
}

// active item
void									CGUIItem::setActiveItem(void)
{
	getLayer()->getWindow()->setActiveItem(this);
}

// window
CWindow*								CGUIItem::getWindow(void)
{
	return getLayer()->getWindow();
}

// point testing
bool									CGUIItem::isPointInBoundingRectangle(CPoint2D& vecPoint, uint32 uiOuterSpacing)
{
	// todo - uiOuterSpacing
	return CMathUtility::isPointInRectangle(vecPoint, getBoundingRectanglePosition(), getBoundingRectangleSize());
}