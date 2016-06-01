#ifndef CKGMWindow_H
#define CKGMWindow_H

#include "GUI/Window/CTabbedWindow.h"

class CKGMWindow : public CTabbedWindow
{
public:
	CKGMWindow(void);

	void					initTabs(void);

	void					bindEvents(void);
	void					unbindEvents(void);

	void					renderTitleBar(void);
};

#endif