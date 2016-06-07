#ifndef CGUIShapeGeometry_NxPoint_H
#define CGUIShapeGeometry_NxPoint_H

#include "Types.h"
#include "CVector2i32.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shape/CGUIShape.h"
#include <vector>

class CGUIShapeGeometry_NxPoint : public CGUIShape
{
public:
	CGUIShapeGeometry_NxPoint(eGUIShape eShapeType);

	void							setPoints(std::vector<CVector2i32>& vecPoints) { m_vecPoints = vecPoints; }
	std::vector<CVector2i32>&		getPoints(void) { return m_vecPoints; }
	
private:
	std::vector<CVector2i32>		m_vecPoints;
};

#endif