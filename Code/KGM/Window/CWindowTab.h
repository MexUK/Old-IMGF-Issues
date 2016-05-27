#ifndef CWindowTab_H
#define CWindowTab_H

#include "Types.h"

class CWindowTab
{
public:
	CWindowTab(void) :
		m_uiIndex(0)
	{};

	void									unload(void);

	void									setIndex(uint32 uiIndex) { m_uiIndex = uiIndex; }
	uint32									getIndex(void) { return m_uiIndex; }

private:
	uint32									m_uiIndex;
};

#endif