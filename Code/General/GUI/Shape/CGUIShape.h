#ifndef CGUIShape_H
#define CGUIShape_H

#include "CPoint2D.h"
#include "CSize2D.h"
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

	void									setShapeGeometry(eGUIShapeGeometry eShapeGeometry) { m_eShapeGeometry = eShapeGeometry; }
	eGUIShapeGeometry						getShapeGeometry(void) { return m_eShapeGeometry; }

	void									setShapeType(eGUIShape eShapeType) { m_eShapeType = eShapeType; }
	eGUIShape								getShapeType(void) { return m_eShapeType; }

private:
	eGUIShapeGeometry						m_eShapeGeometry;
	eGUIShape								m_eShapeType;
};

#endif