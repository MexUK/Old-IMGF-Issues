#ifndef CDBManager_H
#define CDBManager_H

#include "CFormatManager.h"
#include "CSingleton.h"
#include <string>

class CDBFormat;
class CIMGFormat;

class CDBManager : public CFormatManager<CDBFormat>, public CSingleton<CDBManager>
{
public:
	CDBManager(void);
	
	void						init(void);
	void						uninit(void);

	CDBFormat*					createDBFileFromIMGFile(CIMGFormat *pIMGFile);
	CDBFormat*					createBlankDBFile(void);
	
	bool						compareDBFiles(CDBFormat *pDBFile1, CDBFormat *pDBFile2);
};

#endif