#ifndef CRageResourceTypeManager_H
#define CRageResourceTypeManager_H

#include "Types.h"
#include "CManager.h"
#include "CRageResourceType.h"
#include "Pool/CVectorPool.h"
#include <string>
#include <vector>
#include <unordered_map>

class CRageResourceTypeManager : public CManager, public CVectorPool<CRageResourceType*>
{
public:
	void											init(void);
	void											uninit(void);

	CRageResourceType*								getResourceTypeByIdentifierOrFileExtension(uint32 uiResourceIdentifer, std::string strFileExtension);
	CRageResourceType*								getResourceTypeByIdentifier(uint32 uiResourceIdentifer);
	CRageResourceType*								getResourceTypeByFileExtension(std::string strFileExtension);

private:
	void											initRageResourceTypes(void);
};

#endif