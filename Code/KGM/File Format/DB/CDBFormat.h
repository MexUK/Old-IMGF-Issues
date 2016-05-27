#ifndef CDBFormat_H
#define CDBFormat_H

#include "Types.h"
#include "CFormat.h"
#include "Pool/CVectorPool.h"
#include "CDBEntry.h"
#include <string>
#include <vector>

class CIMGFormat;
class CIMGEntry;

class CDBFormat : public CFormat, public CVectorPool<CDBEntry*>
{
public:
	CDBFormat(void) : CFormat(true, LITTLE_ENDIAN) {};
	
	void								unload(void);

	bool								isIMGEntryFound(CIMGFormat *pIMGFile, CIMGEntry *pIMGEntry);
	CDBEntry*							getEntryByNameExactCase(std::string strEntryName);

	void								setDBVersion(uint32 uiDBVersion) { m_uiDBVersion = uiDBVersion; }
	uint32								getDBVersion(void) { return m_uiDBVersion; }

private:
	void								unserialize(void);
	void								serialize(void);

private:
	uint32								m_uiDBVersion;
};

#endif