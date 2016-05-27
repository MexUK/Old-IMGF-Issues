#ifndef CRasterDataFormatManager_H
#define CRasterDataFormatManager_H

#include "CManager.h"
#include "CRasterDataFormat.h"
#include "Pool/CVectorPool.h"

class CRasterDataFormatManager : public CManager, public CVectorPool<CRasterDataFormat*>
{
public:
	void							init(void);
	void							uninit(void);

private:
	void							initRasterDataFormats(void);
};

#endif