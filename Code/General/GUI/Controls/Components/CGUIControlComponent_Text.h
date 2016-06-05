#ifndef CGUIControlComponent_Text_H
#define CGUIControlComponent_Text_H

#include "Types.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "GUI/Styles/CGUIStyles.h"
#include <string>
#include <Windows.h>
#include <Gdiplus.h>

class CGUIControlComponent_Text
{
public:
	uint32									getTextWidth(void) { return 500; /* todo CGUIManager::getInstance()->getGraphicsLibrary()->getTextSize(getText(), getStyles()).m_x */; }

	void									setText(std::string& strText) { m_strText = strText; }
	std::string&							getText(void) { return m_strText; }

private:
	std::string								m_strText;
};

#endif