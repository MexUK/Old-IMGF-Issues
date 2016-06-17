#ifndef CGUIShapeGeometry_NxPoints_H
#define CGUIShapeGeometry_NxPoints_H

#include "Types.h"
#include "CVector2i32.h"
#include "CPoint2D.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shape/CGUIShape.h"
#include <vector>

class CGUIShapeGeometry_NxPoints : public CGUIShape
{
public:
	CGUIShapeGeometry_NxPoints(eGUIShape eShapeType);

	void							unserialize(bool bSkipShapeId = false);
	void							serialize(void);

	void							moveItem(CVector2i32& vecPositionChange);
	void							resizeItemViaOffsets(CVector2i32& vecItemSizeChange) {}

	void							setPoints(std::vector<CPoint2D>& vecPoints) { m_vecPoints = vecPoints; }
	std::vector<CPoint2D>&			getPoints(void) { return m_vecPoints; }
	
private:
	std::vector<CPoint2D>			m_vecPoints;
};

#endif