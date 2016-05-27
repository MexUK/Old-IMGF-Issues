#ifndef CRageManager_H
#define CRageManager_H

#include "Types.h"
#include "CSingleton.h"
#include "CManager.h"

class CRageResourceTypeManager;

class CRageManager : public CManager, public CSingleton<CRageManager>
{
public:
	CRageManager(void);
	~CRageManager(void);

	void											init(void);
	void											uninit(void);

	CRageResourceTypeManager*						getResourceTypeManager(void) { return m_pResourceTypeManager; }

	static uint32									getPackedOffset(uint32 uiOffset);
	static uint32									getPackedDataOffset(uint32 uiOffset);

private:
	CRageResourceTypeManager*						m_pResourceTypeManager;
};

#endif