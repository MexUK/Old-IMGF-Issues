#include "CWindowControl.h"
#include "GUI/Window/CWindow.h"
#include "CWindowScrollPool.h"
#include "Math/CMathUtility.h"

CWindowControl::CWindowControl(eWindowControlType eControlType) :
	m_eControlType(eControlType),
	m_pWindow(nullptr),
	m_uiControlId(0),
	m_bPointMarkedAsInControl(false)
{
	m_vecPosition.m_x = 0;
	m_vecPosition.m_y = 0;
	m_vecSize.m_x = 0;
	m_vecSize.m_y = 0;
	m_pScrolls = new CWindowScrollPool;
}
CWindowControl::~CWindowControl(void)
{
	delete m_pScrolls;
}

// point testing
bool									CWindowControl::isPointInControl(CVector2ui32& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getPosition(), getSize());
}

// focus
bool									CWindowControl::doesControlHaveFocus(void)
{
	return getWindow()->getFocusedControl() == this;
}