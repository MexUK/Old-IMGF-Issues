#ifndef CDATLoaderFormat_H
#define CDATLoaderFormat_H

#include "CFormat.h"
#include "Pool/CVectorPool.h"
#include "eDATLoaderEntryType.h"
#include <string>
#include <vector>

class CDATLoaderEntry;
class CIMGFormat;
class CIDEFormat;
class CIPLFormat;

class CDATLoaderFormat : public CFormat, public CVectorPool<CDATLoaderEntry*>
{
public:
	CDATLoaderFormat(void) : CFormat(false) {};
	
	void									unload(void);

	std::vector<std::string>				getRelativeIDEPaths(void);
	std::vector<std::string>				getRelativeIPLPaths(void);

	std::vector<CIMGFormat*>				parseIMGFiles(std::string strGameDirectoryPath);
	std::vector<CIDEFormat*>				parseIDEFiles(std::string strGameDirectoryPath);
	std::vector<CIPLFormat*>				parseIPLFiles(std::string strGameDirectoryPath);

	template<class ManagerClass, class FormatClass>
	std::vector<FormatClass*>				parseFiles(std::string strGameDirectoryPath, eDATLoaderEntryType eType1, eDATLoaderEntryType eType2 = DAT_LOADER_UNKNOWN);

private:
	void									unserialize(void);
	void									serialize(void) {}; // todo

	void									unserializeLine(void);
};

#endif