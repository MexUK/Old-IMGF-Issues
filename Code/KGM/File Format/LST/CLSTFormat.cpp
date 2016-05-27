#include "CLSTFormat.h"
#include "CLSTSection.h"
#include "CLSTEntry.h"
#include "String/CStringUtility.h"
#include "Vector/CVectorUtility.h"
#include "Data Stream/CDataReader.h"

using namespace std;

void						CLSTFormat::unload(void)
{
	removeAllEntries();
}

void						CLSTFormat::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	pDataReader->readAndStoreLines();
	while (pDataReader->iterateLines() && pDataReader->getUserData() == 0) // LST format user data for data reader: 0 = normal processing, 1 = stop processing (e.g. found END in file)
	{
		unserializeLine();
	}
}

void						CLSTFormat::unserializeLine(void)
{
	// initialize
	static CLSTSection *pLSTActiveSection = nullptr;
	CDataReader *pDataReader = CDataReader::getInstance();
	string strActiveLine = *pDataReader->getActiveLine();

	// remove comment from end of line ( comment characters: # ; )
	string strComment = "";
	size_t uiCommentPosition = strActiveLine.find('#');
	if (uiCommentPosition != string::npos)
	{
		strComment = strActiveLine.substr(uiCommentPosition + 1);
		strActiveLine = strActiveLine.substr(0, uiCommentPosition);
	}
	uiCommentPosition = strActiveLine.find(';');
	if (uiCommentPosition != string::npos)
	{
		strComment = strActiveLine.substr(uiCommentPosition + 1);
		strActiveLine = strActiveLine.substr(0, uiCommentPosition);
	}

	// trim line
	strActiveLine = CStringUtility::trim(strActiveLine);
	
	// process line
	if (strActiveLine == "")
	{
		// blank line
	}
	else
	{
		if (strActiveLine.c_str()[0] == '[')
		{
			// line is a section
			CLSTSection *pLSTFileSection = new CLSTSection;
			pLSTFileSection->setName(CStringUtility::trim(strActiveLine.substr(1, strActiveLine.length() - 2)));
			pLSTActiveSection = pLSTFileSection;
			addEntry(pLSTFileSection);

			if (CStringUtility::toUpperCase(pLSTFileSection->getName()) == "END")
			{
				pDataReader->setUserData(1); // stop processing LST format
			}
		}
		else
		{
			// line is an entry
			deque<string> deqTokens = CVectorUtility::convertVectorToDeque(CStringUtility::split(strActiveLine, " "));

			CLSTEntry *pLSTEntry = new CLSTEntry;
			pLSTEntry->setName(deqTokens[0]);
			deqTokens.pop_front();
			pLSTEntry->setValues(deqTokens);
			pLSTActiveSection->addEntry(pLSTEntry);
		}
	}
}

void						CLSTFormat::serialize(void)
{
	// todo
}

CLSTSection*				CLSTFormat::getEntryByName(string strName)
{
	strName = CStringUtility::toUpperCase(strName);
	for (auto pSection : getEntries())
	{
		if (CStringUtility::toUpperCase(pSection->getName()) == strName)
		{
			return pSection;
		}
	}
	return nullptr;
}

bool						CLSTFormat::doesEntryExistByName(string strName)
{
	return getEntryByName(strName) != nullptr;
}