#ifndef CUpdateConnectionManager_H
#define CUpdateConnectionManager_H

#include "CManager.h"
#include "CUpdateConnection.h"
#include "Pool/CVectorPool.h"

class CUpdateConnectionManager : public CManager, public CVectorPool<CUpdateConnection*>
{
public:
	void							init(void);
	void							uninit(void);

private:
	void							initUpdateConnections(void);
	void							uninitUpdateConnections(void);
};

#endif