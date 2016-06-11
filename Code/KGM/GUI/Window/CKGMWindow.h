#ifndef CKGMWindow_H
#define CKGMWindow_H

#include "GUI/Window/CWindow.h"
#include <string>
#include <vector>

class CKGMWindow : public CWindow
{
public:
	CKGMWindow(void);

	void					initTabs(void);

	void					bindEvents(void);

	void					onRender(void);

	void					onDropFiles(std::vector<std::string>& vecPaths);

private:

	void					renderTitleBar(void);
	void					renderEtc(void);
};

#endif