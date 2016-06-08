#ifndef CGUIShape_H
#define CGUIShape_H

#include "CVector2i32.h"
#include "CVector2ui32.h"
#include "Event/CEventBinder.h"
#include "GUI/Shape/Geometry/eGUIShapeGeometry.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Styles/CGUIStyleableEntity.h"
#include <vector>

class CGUIStyles;
class CControlGroup;
class CWindow;

class CGUIShape : public CEventBinder, public CGUIStyleableEntity
{
public:
	CGUIShape(eGUIShapeGeometry eShapeGeometry, eGUIShape eShapeType);

	void									unload(void) {}

	virtual void							bindEvents(void) = 0;
	virtual void							render(void) = 0;

	CWindow*								getWindow(void);

	void									setControlGroup(CControlGroup* pControlGroup) { m_pControlGroup = pControlGroup; }
	CControlGroup*							getControlGroup(void) { return m_pControlGroup; }

	void									setShapeGeometry(eGUIShapeGeometry eShapeGeometry) { m_eShapeGeometry = eShapeGeometry; }
	eGUIShapeGeometry						getShapeGeometry(void) { return m_eShapeGeometry; }

	void									setShapeType(eGUIShape eShapeType) { m_eShapeType = eShapeType; }
	eGUIShape								getShapeType(void) { return m_eShapeType; }

private:
	CControlGroup*							m_pControlGroup;
	eGUIShapeGeometry						m_eShapeGeometry;
	eGUIShape								m_eShapeType;
};

#endif