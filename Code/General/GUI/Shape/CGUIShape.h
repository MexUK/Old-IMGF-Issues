#ifndef CGUIShape_H
#define CGUIShape_H

#include "CVector2i32.h"
#include "CVector2ui32.h"
#include "Event/CEventBinder.h"
#include "GUI/Shape/Geometry/eGUIShapeGeometry.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Item/CGUIItem.h"
#include <vector>

class CGUIStyles;
class CGUILayer;
class CWindow;

class CGUIShape : public CGUIItem, public CEventBinder
{
public:
	CGUIShape(eGUIShapeGeometry eShapeGeometry, eGUIShape eShapeType);

	void									unload(void) {}

	virtual void							bindEvents(void) = 0;
	virtual void							render(void) = 0;

	CWindow*								getWindow(void);

	void									setLayer(CGUILayer* pLayer) { m_pLayer = pLayer; }
	CGUILayer*							getLayer(void) { return m_pLayer; }

	void									setShapeGeometry(eGUIShapeGeometry eShapeGeometry) { m_eShapeGeometry = eShapeGeometry; }
	eGUIShapeGeometry						getShapeGeometry(void) { return m_eShapeGeometry; }

	void									setShapeType(eGUIShape eShapeType) { m_eShapeType = eShapeType; }
	eGUIShape								getShapeType(void) { return m_eShapeType; }

private:
	CGUILayer*							m_pLayer;
	eGUIShapeGeometry						m_eShapeGeometry;
	eGUIShape								m_eShapeType;
};

#endif