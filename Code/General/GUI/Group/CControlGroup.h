#ifndef CControlGroup_H
#define CControlGroup_H

#include "Types.h"
#include "Pool/CVectorPool.h"
#include "GUI/Control/CGUIControl.h"
#include "CVector2i32.h"
#include "CVector2ui32.h"
#include <string>

class CButtonControl;
class CCheckControl;
class CDropControl;
class CEditControl;
class CListControl;
class CMenuControl;
class CProgressControl;
class CRadioControl;
class CScrollControl;
class CTabBarControl;
class CTextControl;

class CControlGroup : public CVectorPool<CGUIControl*>
{
public:
	CControlGroup(void);

	void					unload(void) {}

	void					bindAllEvents(void);
	void					unbindAllEvents(void);

	void					setWindow(CWindow *pWindow) { m_pWindow = pWindow; }
	CWindow*				getWindow(void) { return m_pWindow; }

	CButtonControl*		addButton(CVector2i32& vecPosition, CVector2ui32& vecSize, std::string strButtonText);
	CCheckControl*		addCheck(CVector2i32& vecPosition, CVector2ui32& vecSize, std::string strCheckText);
	CDropControl*		addDrop(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CEditControl*		addEdit(CVector2i32& vecPosition, CVector2ui32& vecSize, std::string strEditText = "");
	CListControl*		addList(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CMenuControl*		addMenu(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CProgressControl*	addProgress(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CRadioControl*		addRadio(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CScrollControl*		addScroll(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CTabBarControl*		addTabBar(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CTextControl*		addText(CVector2i32& vecPosition, CVector2ui32& vecSize, std::string strText);

private:
	void						_addControl(CGUIControl *pWindowControl, CVector2i32& vecPosition, CVector2ui32& vecSize);

private:
	CWindow*				m_pWindow;
};

#endif