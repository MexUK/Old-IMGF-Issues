#ifndef CGUIItem_H
#define CGUIItem_H

#include "CVector2i32.h"
#include "CVector2ui32.h"
#include "GUI/Styles/CGUIStyleableEntity.h"

class CGUILayer;
class CWindow;

class CGUIItem : public CGUIStyleableEntity
{
public:
	CGUIItem(void);

	void									unload(void) {}

	virtual void							render(void)											= 0;
	virtual bool							isPointInItem(CVector2i32& vecPoint)					= 0;
	virtual CVector2i32						getBoundingRectanglePosition(void)						= 0;
	virtual CVector2ui32					getBoundingRectangleSize(void)							= 0;
	virtual void							moveItem(CVector2i32& vecItemPositionChange)			= 0;
	virtual void							resizeItemViaOffsets(CVector2i32& vecItemSizeChange)	= 0;

	void									onMoveItem(CVector2i32& vecItemPositionChange);
	void									onResizeItem(CVector2i32& vecItemPositionChange, CVector2i32& vecItemSizeChange);

	void									setActiveItem(void);

	CWindow*								getWindow(void);

	bool									isPointInBoundingRectangle(CVector2i32& vecPoint, uint32 uiOuterSpacing);

	void									setLayer(CGUILayer* pLayer) { m_pLayer = pLayer; }
	CGUILayer*								getLayer(void) { return m_pLayer; }

private:
	CGUILayer*								m_pLayer;
};

#endif