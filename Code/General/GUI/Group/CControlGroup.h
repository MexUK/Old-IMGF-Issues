#ifndef CControlGroup_H
#define CControlGroup_H

#include "Types.h"
#include "Pool/CVectorPool.h"
#include "GUI/Control/CWindowControl.h"
#include "CVector2i32.h"
#include "CVector2ui32.h"
#include <string>

class CWindowControl_Button;
class CWindowControl_Check;
class CWindowControl_Drop;
class CWindowControl_Edit;
class CWindowControl_List;
class CWindowControl_Menu;
class CWindowControl_Progress;
class CWindowControl_Radio;
class CWindowControl_Scroll;
class CWindowControl_TabBar;
class CWindowControl_Text;

class CControlGroup : public CVectorPool<CWindowControl*>
{
public:
	CControlGroup(void);

	void					unload(void) {}

	void					bindAllEvents(void);
	void					unbindAllEvents(void);

	void					setWindow(CWindow *pWindow) { m_pWindow = pWindow; }
	CWindow*				getWindow(void) { return m_pWindow; }

	CWindowControl_Button*		addButton(CVector2i32& vecPosition, CVector2ui32& vecSize, std::string strButtonText);
	CWindowControl_Check*		addCheck(CVector2i32& vecPosition, CVector2ui32& vecSize, std::string strCheckText);
	CWindowControl_Drop*		addDrop(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CWindowControl_Edit*		addEdit(CVector2i32& vecPosition, CVector2ui32& vecSize, std::string strEditText = "");
	CWindowControl_List*		addList(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CWindowControl_Menu*		addMenu(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CWindowControl_Progress*	addProgress(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CWindowControl_Radio*		addRadio(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CWindowControl_Scroll*		addScroll(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CWindowControl_TabBar*		addTabBar(CVector2i32& vecPosition, CVector2ui32& vecSize);
	CWindowControl_Text*		addText(CVector2i32& vecPosition, CVector2ui32& vecSize, std::string strText);

private:
	void						_addControl(CWindowControl *pWindowControl, CVector2i32& vecPosition, CVector2ui32& vecSize);

private:
	CWindow*				m_pWindow;
};

#endif