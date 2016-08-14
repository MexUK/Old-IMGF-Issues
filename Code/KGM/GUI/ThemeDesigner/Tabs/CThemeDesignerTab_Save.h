#ifndef CThemeDesignerTab_Save_H
#define CThemeDesignerTab_Save_H

#include "CPoint2D.h"
#include "CSize2D.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Control/eGUIControl.h"
#include "GUI/ThemeDesigner/Tab/CThemeDesignerTab.h"

class CButtonControl;

class CThemeDesignerTab_Save : public CThemeDesignerTab
{
public:
	CThemeDesignerTab_Save(CThemeDesigner *pThemeDesigner);

	void													bindEvents(void);
	void													initDesign(void);

	void													onPressButton(CButtonControl *pButton);
};

#endif