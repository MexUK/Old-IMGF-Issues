#include "CGUIStyles.h"
#include "GUI/CGUIManager.h" // for RGB()
#include "String/CStringUtility.h"

using namespace std;

CMultipleTypeValuesUMapContainer<std::string>		CGUIStyles::m_umapStyleDefaultValues;

CGUIStyles::CGUIStyles(void)
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
	getStyleDefaultValues().setEntry("border-colour", RGB(0, 0, 0));
	getStyleDefaultValues().setEntry<uint32>("border-thickness", 0);
	getStyleDefaultValues().setEntry<string>("border-style", "consistent");
	getStyleDefaultValues().setEntry<int32>("border-intersection-radius", 0);
	getStyleDefaultValues().setEntry<bool>("border-state-left", false); // todo - document x4
	getStyleDefaultValues().setEntry<bool>("border-state-right", false);
	getStyleDefaultValues().setEntry<bool>("border-state-top", false);
	getStyleDefaultValues().setEntry<bool>("border-state-bottom", false);

	getStyleDefaultValues().setEntry("fill-colour", RGB(0xFF,0xFF,0xFF));
	getStyleDefaultValues().setEntry("fill-colour-start", RGB(0xFF, 0xFF, 0xFF));
	getStyleDefaultValues().setEntry("fill-colour-stop", RGB(0x80, 0x80, 0x80));

	getStyleDefaultValues().setEntry("text-colour", RGB(0, 0, 0));
	getStyleDefaultValues().setEntry("text-size", 11);
	getStyleDefaultValues().setEntry<string>("text-font", "Verdana");
	getStyleDefaultValues().setEntry<string>("text-style", "default");

	getStyleDefaultValues().setEntry<string>("text-align-x", "left");
	getStyleDefaultValues().setEntry<string>("text-align-y", "top");
	getStyleDefaultValues().setEntry<int32>("inner-spacing-left", 0);
	getStyleDefaultValues().setEntry<int32>("inner-spacing-top", 0);
	getStyleDefaultValues().setEntry<int32>("inner-spacing-right", 0);
	getStyleDefaultValues().setEntry<int32>("inner-spacing-bottom", 0);

	getStyleDefaultValues().setEntry("caret-colour", RGB(0, 0, 0));
	getStyleDefaultValues().setEntry("row-fill-colour-1", RGB(255, 255, 255));
	getStyleDefaultValues().setEntry("row-fill-colour-2", RGB(200, 200, 200));
	getStyleDefaultValues().setEntry("progress-bar-fill-colour", RGB(0, 50, 150));
	getStyleDefaultValues().setEntry("fill-colour-marked", RGB(0, 255, 0));
	getStyleDefaultValues().setEntry("fill-colour-unmarked", RGB(0xFF, 0xFF, 0xFF));
}

void					CGUIStyles::uninit(void)
{
	removeAllEntries();
	m_umapStyleDefaultValues.removeAllEntries();
	m_umapStyleNameOverwrites.clear();
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
	return doesEntryExist("fill-colour")
		|| (doesEntryExist("fill-colour-start") && doesEntryExist("fill-colour-end"))
	// todo	|| doesEntryExist("progress-bar-fill-colour")
		|| (doesEntryExist("fill-colour-marked") || doesEntryExist("fill-colour-unmarked"));
}

// spacing
CVector2i32				CGUIStyles::getMinInnerSpacing(void)
{
	CVector2i32 vecMinSpacing(
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

CVector2i32				CGUIStyles::getMaxInnerSpacing(void)
{
	CVector2i32 vecMaxSpacing(
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

// style name overwrites
void					CGUIStyles::setStyleNameOverwrite(string strStyleName, string strNewStyleName)
{
	m_umapStyleNameOverwrites[strStyleName] = strNewStyleName;
}

void					CGUIStyles::restoreStyleNameOverwrites(void)
{
	m_umapStyleNameOverwrites.clear();
}

string					CGUIStyles::getStyleNameOverwrite(string strStyleName)
{
	if (m_umapStyleNameOverwrites.count(strStyleName) == 0)
	{
		return strStyleName;
	}
	else
	{
		return m_umapStyleNameOverwrites[strStyleName];
	}
}