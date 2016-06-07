#include "CGUIShape.h"
#include "GUI/Window/CWindow.h"
#include "GUI/Styles/CGUIStyles.h"

CGUIShape::CGUIShape(eGUIShape eShapeType) :
	m_pControlGroup(nullptr),
	m_eShapeType(eShapeType),
	m_pStyles(nullptr)
{
	m_vecPosition.m_x = 0;
	m_vecPosition.m_y = 0;
	m_vecSize.m_x = 0;
	m_vecSize.m_y = 0;
	m_pStyles = new CGUIStyles;
	m_vecPolygonPointPositions.resize(0);
}

// window
CWindow*								CGUIShape::getWindow(void)
{
	return getControlGroup()->getWindow();
}