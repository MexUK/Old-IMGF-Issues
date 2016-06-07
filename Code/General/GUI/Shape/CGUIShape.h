#ifndef CGUIShape_H
#define CGUIShape_H

#include "CVector2i32.h"
#include "CVector2ui32.h"
#include "Event/CEventBinder.h"
#include "GUI/Shape/eGUIShape.h"
#include <vector>

class CGUIStyles;
class CControlGroup;
class CWindow;

class CGUIShape : public CEventBinder
{
public:
	CGUIShape(eGUIShape eShapeType);
	
	void									unload(void) {}

	virtual void							bindEvents(void) {}

	CWindow*								getWindow(void);

	void									setControlGroup(CControlGroup* pControlGroup) { m_pControlGroup = pControlGroup; }
	CControlGroup*							getControlGroup(void) { return m_pControlGroup; }

	void									setShapeType(eGUIShape eShapeType) { m_eShapeType = eShapeType; }
	eGUIShape								getShapeType(void) { return m_eShapeType; }

	void									setPosition(CVector2i32& vecPosition) { m_vecPosition = vecPosition; }
	CVector2i32&							getPosition(void) { return m_vecPosition; }

	void									setSize(CVector2ui32& vecSize) { m_vecSize = vecSize; }
	CVector2ui32&							getSize(void) { return m_vecSize; }

	CGUIStyles*								getStyles(void) { return m_pStyles; } // todo - repeated code x3

	void									setPolygonPointPositions(std::vector<CVector2i32>& vecPolygonPointPositions) { m_vecPolygonPointPositions = vecPolygonPointPositions; }
	std::vector<CVector2i32>&				getPolygonPointPositions(void) { return m_vecPolygonPointPositions; }

private:
	CControlGroup*							m_pControlGroup;
	eGUIShape								m_eShapeType;
	CVector2i32								m_vecPosition;
	CVector2ui32							m_vecSize;
	CGUIStyles*								m_pStyles;
	std::vector<CVector2i32>				m_vecPolygonPointPositions;
};

#endif