#ifndef CControlGroup_H
#define CControlGroup_H

#include "Types.h"
#include "Pool/CVectorPool.h"
#include "GUI/Control/CWindowControl.h"

class CControlGroup : public CVectorPool<CWindowControl*>
{
public:
	CControlGroup(void);

	void					unload(void) {}

	void					bindAllEvents(void);
	void					unbindAllEvents(void);

	void					setWindow(CWindow *pWindow) { m_pWindow = pWindow; }
	CWindow*				getWindow(void) { return m_pWindow; }

private:
	CWindow*				m_pWindow;
};

#endif