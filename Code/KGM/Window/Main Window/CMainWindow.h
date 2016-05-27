#ifndef CMainWindow_H
#define CMainWindow_H

#include "Window/CWindow.h"
#include "CVector2ui32.h"
#include <string>
#include <vector>

class CMainWindow : public CWindow
{
public:
	void									onMouseDown(CVector2ui32& vecCursorPosition);
	void									onMouseUp(CVector2ui32& vecCursorPosition);
	void									onMouseMove(CVector2ui32& vecCursorPosition);
	void									onDoubleLeftClick(CVector2ui32& vecCursorPosition);

	void									render(void);

	void									onDropFiles(std::vector<std::string>& vecPaths);

	//CVector2ui32							getPosition(void); // todo - override base class
	//CVector2ui32							getSize(void); // todo - override base class
};

#endif