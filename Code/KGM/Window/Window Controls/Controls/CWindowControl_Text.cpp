#include "CWindowControl_Text.h"
#include "GDIPlus/CGDIPlusUtility.h"

void		CWindowControl_Text::render(void)
{
	CGDIPlusUtility::drawText(getPosition(), getSize(), getText(), getTextColour(), getFontSize(), isBold());
}