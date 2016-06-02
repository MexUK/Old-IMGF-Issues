#include "CControlGroup.h"
#include "GUI/Controls/CWindowControl_Button.h"
#include "GUI/Controls/CWindowControl_Check.h"
#include "GUI/Controls/CWindowControl_Drop.h"
#include "GUI/Controls/CWindowControl_Edit.h"
#include "GUI/Controls/CWindowControl_List.h"
#include "GUI/Controls/CWindowControl_Menu.h"
#include "GUI/Controls/CWindowControl_Progress.h"
#include "GUI/Controls/CWindowControl_Radio.h"
#include "GUI/Controls/CWindowControl_Scroll.h"
// todo #include "GUI/Controls/CWindowControl_TabBar.h"
#include "GUI/Controls/CWindowControl_Text.h"

using namespace std;

CControlGroup::CControlGroup(void) :
	m_pWindow(nullptr)
{
}

// event binding
void									CControlGroup::bindAllEvents(void)
{
	for (CWindowControl *pWindowControl : getEntries())
	{
		pWindowControl->bindEvents();
	}
}

void									CControlGroup::unbindAllEvents(void)
{
	for (CWindowControl *pWindowControl : getEntries())
	{
		pWindowControl->unbindEvents();
	}
}

// add control - derived
CWindowControl_Button*		CControlGroup::addButton(CVector2i32& vecPosition, CVector2ui32& vecSize, string strButtonText)
{
	CWindowControl_Button *pControl = new CWindowControl_Button;
	pControl->setText(strButtonText);
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CWindowControl_Check*		CControlGroup::addCheck(CVector2i32& vecPosition, CVector2ui32& vecSize, string strCheckText)
{
	CWindowControl_Check *pControl = new CWindowControl_Check;
	pControl->setText(strCheckText);
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CWindowControl_Drop*		CControlGroup::addDrop(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CWindowControl_Drop *pControl = new CWindowControl_Drop;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CWindowControl_Edit*		CControlGroup::addEdit(CVector2i32& vecPosition, CVector2ui32& vecSize, string strEditText)
{
	CWindowControl_Edit *pControl = new CWindowControl_Edit;
	pControl->setText(strEditText);
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CWindowControl_List*		CControlGroup::addList(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CWindowControl_List *pControl = new CWindowControl_List;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CWindowControl_Menu*		CControlGroup::addMenu(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CWindowControl_Menu *pControl = new CWindowControl_Menu;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CWindowControl_Progress*	CControlGroup::addProgress(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CWindowControl_Progress *pControl = new CWindowControl_Progress;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CWindowControl_Radio*		CControlGroup::addRadio(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CWindowControl_Radio *pControl = new CWindowControl_Radio;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CWindowControl_Scroll*		CControlGroup::addScroll(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CWindowControl_Scroll *pControl = new CWindowControl_Scroll;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CWindowControl_TabBar*		CControlGroup::addTabBar(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	/*
	todo
	CWindowControl_TabBar *pControl = new CWindowControl_TabBar;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
	*/
	return nullptr;
}

CWindowControl_Text*		CControlGroup::addText(CVector2i32& vecPosition, CVector2ui32& vecSize, string strText)
{
	CWindowControl_Text *pControl = new CWindowControl_Text;
	pControl->setText(strText);
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

// add control - base
void						CControlGroup::_addControl(CWindowControl *pControl, CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	pControl->setControlGroup(this);
	pControl->setPosition(vecPosition);
	pControl->setSize(vecSize);
	addEntry(pControl);
}