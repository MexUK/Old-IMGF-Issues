#include "CGUIShape.h"
#include "GUI/Window/CWindow.h"
#include "GUI/Styles/CGUIStyles.h"

CGUIShape::CGUIShape(eGUIShapeGeometry eShapeGeometry, eGUIShape eShapeType) :
	m_eShapeGeometry(eShapeGeometry),
	m_eShapeType(eShapeType),
	m_pLayer(nullptr)
{
}

// window
CWindow*								CGUIShape::getWindow(void)
{
	return getLayer()->getWindow();
}