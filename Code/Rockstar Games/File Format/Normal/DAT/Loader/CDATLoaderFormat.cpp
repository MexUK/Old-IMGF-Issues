#include "CDATLoaderFormat.h"
#include "CDATLoaderEntry.h"
#include "Path/CPathUtility.h"
#include "IMG/CIMGManager.h"
#include "IMG/CIMGFormat.h"
#include "IDE/CIDEManager.h"
#include "IDE/CIDEFormat.h"
#include "IPL/CIPLManager.h"
#include "IPL/CIPLFormat.h"
#include "String/CStringUtility.h"
#include "Data Stream/CDataReader.h"
#include "Vector/CVectorUtility.h"
#include "CDATLoaderManager.h"

using namespace std;

void					CDATLoaderFormat::unload(void)
{
	removeAllEntries();
}

void					CDATLoaderFormat::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	pDataReader->readAndStoreLines();
	while (pDataReader->iterateLines())
	{
		unserializeLine();
	}
}

void					CDATLoaderFormat::unserializeLine(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	string strLine = *pDataReader->getActiveLine();

	// remove comment from end of line
	string strComment = "";
	size_t uiCommentPosition = strLine.find('#');
	if (uiCommentPosition != string::npos)
	{
		strComment = strLine.substr(uiCommentPosition);
		strLine = strLine.substr(0, uiCommentPosition);
	}

	if (strLine == "")
	{
		// blank line
	}

	else if (CStringUtility::ltrim(strLine).c_str()[0] == '#')
	{
		// line is a comment
	}

	else
	{
		// parse line
		deque<string> deqTokens = CVectorUtility::convertVectorToDeque(CStringUtility::split(strLine, " "));

		CDATLoaderEntry *pDATLoaderEntry = new CDATLoaderEntry;
		pDATLoaderEntry->setEntryType(CDATLoaderManager::getDATEntryTypeFromString(deqTokens[0]));
		deqTokens.pop_front();
		pDATLoaderEntry->setEntryValues(deqTokens);
		addEntry(pDATLoaderEntry);
	}
}

vector<string>			CDATLoaderFormat::getRelativeIDEPaths(void)
{
	vector<string> vecRelativeIDEPaths;
	for (auto pDATEntry : getEntries())
	{
		if (pDATEntry->getEntryType() == DAT_LOADER_IDE)
		{
			vecRelativeIDEPaths.push_back(pDATEntry->getEntryValues()[0]);
		}
	}
	return vecRelativeIDEPaths;
}

vector<string>			CDATLoaderFormat::getRelativeIPLPaths(void)
{
	vector<string> vecRelativeIDEPaths;
	for (auto pDATEntry : getEntries())
	{
		if (pDATEntry->getEntryType() == DAT_LOADER_IPL)
		{
			vecRelativeIDEPaths.push_back(pDATEntry->getEntryValues()[0]);
		}
	}
	return vecRelativeIDEPaths;
}

vector<CIMGFormat*>		CDATLoaderFormat::parseIMGFiles(string strGameDirectoryPath)
{
	return parseFiles<CIMGManager, CIMGFormat>(strGameDirectoryPath, DAT_LOADER_IMG, DAT_LOADER_CDIMAGE);
}

vector<CIDEFormat*>		CDATLoaderFormat::parseIDEFiles(string strGameDirectoryPath)
{
	return parseFiles<CIDEManager, CIDEFormat>(strGameDirectoryPath, DAT_LOADER_IDE);
}

vector<CIPLFormat*>		CDATLoaderFormat::parseIPLFiles(string strGameDirectoryPath)
{
	return parseFiles<CIPLManager, CIPLFormat>(strGameDirectoryPath, DAT_LOADER_IPL);
}

template<class ManagerClass, class FormatClass>
vector<FormatClass*>	CDATLoaderFormat::parseFiles(string strGameDirectoryPath, eDATLoaderEntryType eType1, eDATLoaderEntryType eType2)
{
	strGameDirectoryPath = CPathUtility::addSlashToEnd(strGameDirectoryPath);

	vector<FormatClass*> vecFormats;

	for (auto pDATEntry : getEntries())
	{
		if (pDATEntry->getEntryType() == eType1 || (eType2 != DAT_LOADER_UNKNOWN && pDATEntry->getEntryType() == eType2))
		{
			string strFormatPath = strGameDirectoryPath + CPathUtility::removeSlashFromFront(pDATEntry->getEntryValues()[0]);
			FormatClass *pFormat = ManagerClass::getInstance()->parseViaFile(strFormatPath);
			if (pFormat->doesHaveError())
			{
				pFormat->unload();
				delete pFormat;
			}
			else
			{
				vecFormats.push_back(pFormat);
			}
		}
	}

	return vecFormats;
}