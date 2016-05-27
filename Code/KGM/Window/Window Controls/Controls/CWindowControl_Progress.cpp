#include "CWindowControl_Progress.h"
#include "String/CStringUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"

using namespace std;

void		CWindowControl_Progress::render(void)
{
	float32 fProgress = ((float32)getCurrentTicks()) / ((float32)getMaxTicks());
	uint32 uiPixelProgressX = ((float32)getSize().m_x) * fProgress;
	string strProgress = CStringUtility::toString(fProgress * 100.0f) +"%";

	CGDIPlusUtility::drawRectangleFill(getPosition(), getSize(), getFillColour());
	CGDIPlusUtility::drawRectangleFill(getPosition(), CVector2ui32(uiPixelProgressX, getSize().m_y), getProgressFillColour());
	CGDIPlusUtility::drawRectangleBorder(getPosition(), getSize(), getLineColour());
	CGDIPlusUtility::drawText(getPosition(), getSize(), strProgress, getTextColour(), getFontSize(), isBold());
}