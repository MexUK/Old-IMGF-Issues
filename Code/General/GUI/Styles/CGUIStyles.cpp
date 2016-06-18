#include "CGUIStyles.h"
#include "GUI/CGUIManager.h" // for RGB()
#include "String/CStringUtility.h"
#include "CColour.h"

using namespace std;

CMultipleTypeValuesUMapContainer<std::string>		CGUIStyles::m_umapStyleDefaultValues;

CGUIStyles::CGUIStyles(void) :
	m_bHasFillOverwrite(false)
{
	init();
}

// init/uninit
void					CGUIStyles::init(void)
{
	if (!getStyleDefaultValues().isEmpty())
	{
		return;
	}
	getStyleDefaultValues().setEntry("border-colour", CColour(0, 0, 0));
	getStyleDefaultValues().setEntry<uint32>("border-thickness", 0);
	getStyleDefaultValues().setEntry<string>("border-style", "consistent");
	getStyleDefaultValues().setEntry<int32>("border-intersection-radius", 0);
	getStyleDefaultValues().setEntry<bool>("border-state-left", false); // todo - document x4
	getStyleDefaultValues().setEntry<bool>("border-state-right", false);
	getStyleDefaultValues().setEntry<bool>("border-state-top", false);
	getStyleDefaultValues().setEntry<bool>("border-state-bottom", false);

	getStyleDefaultValues().setEntry("fill-colour", CColour(0xFF,0xFF,0xFF));
	getStyleDefaultValues().setEntry("fill-colour-start", CColour(0xFF, 0xFF, 0xFF));
	getStyleDefaultValues().setEntry("fill-colour-stop", CColour(0x80, 0x80, 0x80));

	getStyleDefaultValues().setEntry("text-colour", CColour(0, 0, 0));
	getStyleDefaultValues().setEntry<uint32>("text-size", 11);
	getStyleDefaultValues().setEntry<string>("text-font", "Verdana");
	getStyleDefaultValues().setEntry<string>("text-style", "default");
	getStyleDefaultValues().setEntry<uint32>("text-thickness", 1);

	getStyleDefaultValues().setEntry<string>("text-align-x", "left");
	getStyleDefaultValues().setEntry<string>("text-align-y", "top");
	getStyleDefaultValues().setEntry<int32>("inner-spacing-left", 0);
	getStyleDefaultValues().setEntry<int32>("inner-spacing-top", 0);
	getStyleDefaultValues().setEntry<int32>("inner-spacing-right", 0);
	getStyleDefaultValues().setEntry<int32>("inner-spacing-bottom", 0);
	getStyleDefaultValues().setEntry<int32>("inner-spacing-x", 0);
	getStyleDefaultValues().setEntry<int32>("inner-spacing-y", 0);

	getStyleDefaultValues().setEntry("caret-colour", CColour(0, 0, 0));
	getStyleDefaultValues().setEntry("list-row-1.fill-colour", CColour(255, 255, 255));
	getStyleDefaultValues().setEntry("list-row-2.fill-colour", CColour(200, 200, 200));
	getStyleDefaultValues().setEntry("progress-seek-bar.fill-colour", CColour(0, 50, 150));
	getStyleDefaultValues().setEntry("fill-colour:marked", CColour(0, 255, 0));
	getStyleDefaultValues().setEntry("fill-colour:unmarked", CColour(0xFF, 0xFF, 0xFF));
	getStyleDefaultValues().setEntry("list.line-colour", CColour(0, 0, 0));
	getStyleDefaultValues().setEntry<int32>("markable-text-spacing", 0);
	getStyleDefaultValues().setEntry("drop-list.fill-colour", CColour(0xFF, 0xFF, 0xFF));
	getStyleDefaultValues().setEntry("edit-selected-text.fill-colour", CColour(111, 187, 248));
	getStyleDefaultValues().setEntry("edit-selected-text.border-colour", CColour(0xFF, 0xFF, 0xFF));
}

void					CGUIStyles::uninit(void)
{
	removeAllEntries();
	m_umapStyleDefaultValues.removeAllEntries();
}

// does style exist
bool					CGUIStyles::doesStyleExist(string strStyleName)
{
	return doesEntryExist(strStyleName);
}

// set style
void					CGUIStyles::setStyle(string strStyleName, bool bStyleValue)
{
	setEntry(strStyleName, bStyleValue);
}

void					CGUIStyles::setStyle(string strStyleName, uint32 uiStyleValue)
{
	setEntry(strStyleName, uiStyleValue);
}

void					CGUIStyles::setStyle(string strStyleName, int32 iStyleValue)
{
	setEntry(strStyleName, iStyleValue);
}

void					CGUIStyles::setStyle(string strStyleName, float32 fStyleValue)
{
	setEntry(strStyleName, fStyleValue);
}

void					CGUIStyles::setStyle(string strStyleName, string strStyleValue)
{
	setEntry(strStyleName, strStyleValue);
}

// border
bool					CGUIStyles::doesHaveBorder(void)
{
	return
		doesEntryExist("border-thickness")
	 || doesEntryExist("border-colour")
	 || doesEntryExist("border-style")
	 || doesEntryExist("border-intersection-radius");
}

bool					CGUIStyles::doesHaveLeftBorder(void)
{
	return getStyle<bool>("border-state-left") || doesHaveBorder();
}

bool					CGUIStyles::doesHaveRightBorder(void)
{
	return getStyle<bool>("border-state-right") || doesHaveBorder();
}

bool					CGUIStyles::doesHaveTopBorder(void)
{
	return getStyle<bool>("border-state-top") || doesHaveBorder();
}

bool					CGUIStyles::doesHaveBottomBorder(void)
{
	return getStyle<bool>("border-state-bottom") || doesHaveBorder();
}

// fill
bool					CGUIStyles::doesHaveFill(void)
{
	/*
	todo
	// check for alpha in fill colour
	uint32 uiColour = getStyle<uint32>("fill-colour");
	return (uiColour & 0xFF) != 0;
	*/

	return doesEntryExist("fill-colour")
		|| (doesEntryExist("fill-colour-start") && doesEntryExist("fill-colour-end"))
		// todo	|| doesEntryExist("progress-bar-fill-colour")
		|| doesEntryExist("fill-colour:marked") || doesEntryExist("fill-colour:unmarked")
		|| doesHaveFillOverwrite()
		|| doesDefaultStyleValueExist(getResolvedStyleName("fill-colour"));
}

// spacing
CPoint2D				CGUIStyles::getMinInnerSpacing(void)
{
	CPoint2D vecMinSpacing(
		getStyle<int32>("inner-spacing-x"),
		getStyle<int32>("inner-spacing-y")
	);
	if (doesStyleExist("inner-spacing-left"))
	{
		vecMinSpacing.m_x = getStyle<int32>("inner-spacing-left");
	}
	if (doesStyleExist("inner-spacing-top"))
	{
		vecMinSpacing.m_y = getStyle<int32>("inner-spacing-top");
	}
	return vecMinSpacing;
}

CPoint2D				CGUIStyles::getMaxInnerSpacing(void)
{
	CPoint2D vecMaxSpacing(
		getStyle<int32>("inner-spacing-x"),
		getStyle<int32>("inner-spacing-y")
	);
	if (doesStyleExist("inner-spacing-right"))
	{
		vecMaxSpacing.m_x = getStyle<int32>("inner-spacing-right");
	}
	if (doesStyleExist("inner-spacing-bottom"))
	{
		vecMaxSpacing.m_y = getStyle<int32>("inner-spacing-bottom");
	}
	return vecMaxSpacing;
}

uint32					CGUIStyles::getInnerSpacingTotalX(void)
{
	return getMinInnerSpacing().m_x + getMaxInnerSpacing().m_x;
}

uint32					CGUIStyles::getInnerSpacingTotalY(void)
{
	return getMinInnerSpacing().m_y + getMaxInnerSpacing().m_y;
}

// alignment
string					CGUIStyles::getTextAlignX(void)
{
	if (doesStyleExist("text-align"))
	{
		return CStringUtility::split(getStyle<string>("text-align"), " ")[0];
	}
	else
	{
		return getStyle<string>("text-align-x");
	}
}

string					CGUIStyles::getTextAlignY(void)
{
	if (doesStyleExist("text-align"))
	{
		return CStringUtility::split(getStyle<string>("text-align"), " ")[1];
	}
	else
	{
		return getStyle<string>("text-align-y");
	}
}

// style overwrites
void					CGUIStyles::restoreTemporaryStyleData(void)
{
	setItemComponent("");
	setItemStatus("");
	restoreStyleOverwrites();
}

void					CGUIStyles::restoreStyleOverwrites(void)
{
	setHasFillOverwrite(false);
}

// default styles
bool					CGUIStyles::doesDefaultStyleValueExist(string strStyleName)
{
	return getStyleDefaultValues().doesEntryExist(strStyleName);
}

// resolved style name
string					CGUIStyles::getResolvedStyleName(string strStyleName)
{
	bool
		bHasComponent = getItemComponent() != "",
		bHasStatus = getItemStatus() != "";
	string
		strStyleNameFullyResolved = (getItemComponent() == "" ? "default." : (getItemComponent() + ".")) + strStyleName + (getItemStatus() == "" ? "" : (":" + getItemStatus())); // with component and status
	return strStyleNameFullyResolved;
}