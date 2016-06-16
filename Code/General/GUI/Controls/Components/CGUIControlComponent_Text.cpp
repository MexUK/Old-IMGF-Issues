#include "CGUIControlComponent_Text.h"

CGUIControlComponent_Text::CGUIControlComponent_Text(void) :
	m_bStringSizeNeedsRecalculating(true)
{
}

bool									CGUIControlComponent_Text::checkToRecalculateStringSize(CGUIStyles *pStyles)
{
	if (doesStringSizeNeedRecalculating())
	{
		recalculateStringSize(pStyles);
		setStringSizeNeedsRecalculating(false);
		return true;
	}
	else
	{
		return false;
	}
}

void									CGUIControlComponent_Text::recalculateStringSize(CGUIStyles *pStyles)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	m_gstrText.setSize(pGFX->getTextSize(m_gstrText.getString(), pStyles));
}