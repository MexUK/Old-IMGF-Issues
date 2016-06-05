#include "CGUIStyles.h"
#include "GUI/CGUIManager.h" // for RGB()

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
	getStyleDefaultValues().setEntry("border-thickness", 0);
	getStyleDefaultValues().setEntry<string>("border-style", "consistent");
	getStyleDefaultValues().setEntry("border-intersection-radius", 0);
	getStyleDefaultValues().setEntry("background-colour", RGB(0xFF,0xFF,0xFF));
	getStyleDefaultValues().setEntry("background-colour-start", RGB(0xFF, 0xFF, 0xFF));
	getStyleDefaultValues().setEntry("background-colour-stop", RGB(0x80, 0x80, 0x80));
	getStyleDefaultValues().setEntry("text-colour", RGB(0, 0, 0));
	getStyleDefaultValues().setEntry("text-size", 11);
	getStyleDefaultValues().setEntry<string>("text-font", "Verdana");
	getStyleDefaultValues().setEntry<string>("text-style", "default");
	getStyleDefaultValues().setEntry("caret-colour", RGB(0, 0, 0));
	getStyleDefaultValues().setEntry("row-background-colour-1", RGB(255, 255, 255));
	getStyleDefaultValues().setEntry("row-background-colour-2", RGB(200, 200, 200));
	getStyleDefaultValues().setEntry("progress-background-colour", RGB(0, 50, 150));
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

bool					CGUIStyles::doesHaveFill(void)
{
	return doesEntryExist("background-colour")
		|| (doesEntryExist("background-colour-start") && doesEntryExist("background-colour-start"));
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