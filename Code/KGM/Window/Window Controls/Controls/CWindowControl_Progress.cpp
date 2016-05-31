#include "CWindowControl_Progress.h"
#include "String/CStringUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"

using namespace std;

void			CWindowControl_Progress::render(void)
{
	CGDIPlusUtility::drawRectangleFill(getPosition(), getSize(), getFillColour());
	CGDIPlusUtility::drawRectangleFill(getPosition(), getProgressPosition(), getProgressFillColour());
	CGDIPlusUtility::drawRectangleBorder(getPosition(), getSize(), getLineColour());
	CGDIPlusUtility::drawText(getPosition(), getSize(), getProgressPercentText(), getTextColour(), getFontSize(), isBold());
}

string		CWindowControl_Progress::getProgressPercentText(void)
{
	return CStringUtility::toString(getProgress() * 100.0f) + "%";
}

float32			CWindowControl_Progress::getProgress(void)
{
	return ((float32) getCurrentTicks()) / ((float32) getMaxTicks());
}

CVector2ui32	CWindowControl_Progress::getProgressPosition(void)
{
	return CVector2ui32(getProgressPositionX(), getSize().m_y);
}

uint32			CWindowControl_Progress::getProgressPositionX(void)
{
	return ((float32) getSize().m_x) * getProgress();
}