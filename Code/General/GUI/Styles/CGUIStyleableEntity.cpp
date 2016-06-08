#include "CGUIStyleableEntity.h"
#include "GUI/Styles/CGUIStyles.h"

CGUIStyleableEntity::CGUIStyleableEntity(void) :
	m_pStyles(new CGUIStyles)
{
}

CGUIStyleableEntity::~CGUIStyleableEntity(void)
{
	delete m_pStyles;
}

// styles
void									CGUIStyleableEntity::setStyles(CGUIStyles *pStyles)
{
	if (m_pStyles)
	{
		delete m_pStyles;
	}
	m_pStyles = pStyles;
}

CGUIStyles*								CGUIStyleableEntity::getStyles(void)
{
	return m_pStyles;
}