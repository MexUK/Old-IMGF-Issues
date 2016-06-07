#include "CGUIShape.h"
#include "GUI/Window/CWindow.h"
#include "GUI/Styles/CGUIStyles.h"

CGUIShape::CGUIShape(eGUIShapeGeometry eShapeGeometry, eGUIShape eShapeType) :
	m_eShapeGeometry(eShapeGeometry),
	m_eShapeType(eShapeType),
	m_pControlGroup(nullptr),
	m_pStyles(nullptr)
{
	m_pStyles = new CGUIStyles;
}

CGUIShape::~CGUIShape(void)
{
	delete m_pStyles;
}

// window
CWindow*								CGUIShape::getWindow(void)
{
	return getControlGroup()->getWindow();
}