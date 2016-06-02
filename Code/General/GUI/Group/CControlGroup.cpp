#include "CControlGroup.h"
#include "GUI/Controls/CButtonControl.h"
#include "GUI/Controls/CCheckControl.h"
#include "GUI/Controls/CDropControl.h"
#include "GUI/Controls/CEditControl.h"
#include "GUI/Controls/CListControl.h"
#include "GUI/Controls/CMenuControl.h"
#include "GUI/Controls/CProgressControl.h"
#include "GUI/Controls/CRadioControl.h"
#include "GUI/Controls/CScrollControl.h"
// todo #include "GUI/Controls/CTabBarControl.h"
#include "GUI/Controls/CTextControl.h"

using namespace std;

CControlGroup::CControlGroup(void) :
	m_pWindow(nullptr)
{
}

// event binding
void									CControlGroup::bindAllEvents(void)
{
	for (CGUIControl *pWindowControl : getEntries())
	{
		pWindowControl->bindEvents();
	}
}

void									CControlGroup::unbindAllEvents(void)
{
	for (CGUIControl *pWindowControl : getEntries())
	{
		pWindowControl->unbindEvents();
	}
}

// add control - derived
CButtonControl*		CControlGroup::addButton(CVector2i32& vecPosition, CVector2ui32& vecSize, string strButtonText)
{
	CButtonControl *pControl = new CButtonControl;
	pControl->setText(strButtonText);
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CCheckControl*		CControlGroup::addCheck(CVector2i32& vecPosition, CVector2ui32& vecSize, string strCheckText)
{
	CCheckControl *pControl = new CCheckControl;
	pControl->setText(strCheckText);
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CDropControl*		CControlGroup::addDrop(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CDropControl *pControl = new CDropControl;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CEditControl*		CControlGroup::addEdit(CVector2i32& vecPosition, CVector2ui32& vecSize, string strEditText)
{
	CEditControl *pControl = new CEditControl;
	pControl->setText(strEditText);
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CListControl*		CControlGroup::addList(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CListControl *pControl = new CListControl;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CMenuControl*		CControlGroup::addMenu(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CMenuControl *pControl = new CMenuControl;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CProgressControl*	CControlGroup::addProgress(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CProgressControl *pControl = new CProgressControl;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CRadioControl*		CControlGroup::addRadio(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CRadioControl *pControl = new CRadioControl;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CScrollControl*		CControlGroup::addScroll(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	CScrollControl *pControl = new CScrollControl;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

CTabBarControl*		CControlGroup::addTabBar(CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	/*
	todo
	CTabBarControl *pControl = new CTabBarControl;
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
	*/
	return nullptr;
}

CTextControl*		CControlGroup::addText(CVector2i32& vecPosition, CVector2ui32& vecSize, string strText)
{
	CTextControl *pControl = new CTextControl;
	pControl->setText(strText);
	_addControl(pControl, vecPosition, vecSize);
	return pControl;
}

// add control - base
void						CControlGroup::_addControl(CGUIControl *pControl, CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	pControl->setControlGroup(this);
	pControl->setPosition(vecPosition);
	pControl->setSize(vecSize);
	addEntry(pControl);
}