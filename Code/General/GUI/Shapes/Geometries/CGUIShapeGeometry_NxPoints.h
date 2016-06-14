#ifndef CGUIShapeGeometry_NxPoints_H
#define CGUIShapeGeometry_NxPoints_H

#include "Types.h"
#include "CVector2i32.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shape/CGUIShape.h"
#include <vector>

class CGUIShapeGeometry_NxPoints : public CGUIShape
{
public:
	CGUIShapeGeometry_NxPoints(eGUIShape eShapeType);

	void							moveItem(CVector2i32& vecPositionChange);
	void							resizeItemViaOffsets(CVector2i32& vecItemSizeChange) {}

	void							setPoints(std::vector<CVector2i32>& vecPoints) { m_vecPoints = vecPoints; }
	std::vector<CVector2i32>&		getPoints(void) { return m_vecPoints; }
	
private:
	std::vector<CVector2i32>		m_vecPoints;
};

#endif