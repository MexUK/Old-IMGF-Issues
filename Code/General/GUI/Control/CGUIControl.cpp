#include "CGUIControl.h"
#include "GUI/Window/CWindow.h"
#include "GUI/Styles/CGUIStyles.h"
#include "CGUIScrollPool.h"
#include "Math/CMathUtility.h"

CGUIControl::CGUIControl(eGUIControl eControlType) :
	m_pControlGroup(nullptr),
	m_eControlType(eControlType),
	m_uiControlId(0),
	m_bPointMarkedAsInControl(false),
	m_pScrolls(nullptr)
{
	m_vecPosition.m_x = 0;
	m_vecPosition.m_y = 0;
	m_vecSize.m_x = 0;
	m_vecSize.m_y = 0;
	m_pScrolls = new CGUIScrollPool;
}
CGUIControl::~CGUIControl(void)
{
	delete m_pScrolls;
}

// window
CWindow*								CGUIControl::getWindow(void)
{
	return getControlGroup()->getWindow();
}

// point testing
bool									CGUIControl::isPointInControl(CVector2i32& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getPosition(), getSize());
}

// focus
bool									CGUIControl::doesControlHaveFocus(void)
{
	return getWindow()->getFocusedControl() == this;
}