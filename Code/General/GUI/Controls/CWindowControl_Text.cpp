#include "CWindowControl_Text.h"
#include "GDIPlus/CGDIPlusUtility.h"

// render
void		CWindowControl_Text::render(void)
{
	CGDIPlusUtility::drawText(getPosition(), getSize(), getText(), getTextColour(), getFontSize(), isBold());
}