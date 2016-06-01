#ifndef CKGMWindow_H
#define CKGMWindow_H

#include "GUI/Window/CTabbedWindow.h"
#include <string>
#include <vector>

class CKGMWindow : public CTabbedWindow
{
public:
	CKGMWindow(void);

	void					initTabs(void);

	void					bindEvents(void);

	void					onRender(void);

	void					onDropFiles(std::vector<std::string>& vecPaths);
};

#endif