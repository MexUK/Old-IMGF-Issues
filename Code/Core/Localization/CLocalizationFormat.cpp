#include "CLocalizationFormat.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "String/CStringUtility.h"

using namespace std;

void			CLocalizationFormat::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	pDataReader->readAndStoreLines();
	while (pDataReader->iterateLines())
	{
		unserializeLine();
	}
	pDataReader->resetLines();
}
void			CLocalizationFormat::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	// todo
}

void			CLocalizationFormat::unserializeLine(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	string strLine = *pDataReader->getActiveLine();
	strLine = CStringUtility::trim(strLine, " ");
	strLine = CStringUtility::trim(strLine, "\t");

	if (strLine == "")
	{
		// blank line
		return;
	}

	else if (strLine.c_str()[0] == '#')
	{
		// comment line
		return;
	}

	else
	{
		// remove comment from end of line
		string strComment = "";
		size_t uiCommentPosition = strLine.find('#');
		if (uiCommentPosition != string::npos)
		{
			strComment = strLine.substr(uiCommentPosition + 1);
			strLine = strLine.substr(0, uiCommentPosition);
		}

		uint32 uiFirstCommaPosition = strLine.find(',');
		if (uiFirstCommaPosition == string::npos)
		{
			// line does not have comma
			return;
		}

		string strInternalName = strLine.substr(0, uiFirstCommaPosition);
		string strLanguageText = strLine.substr(uiFirstCommaPosition + 1);

		strInternalName = CStringUtility::trim(strInternalName, " ");
		strInternalName = CStringUtility::trim(strInternalName, "\t");

		strLanguageText = CStringUtility::trim(strLanguageText, " ");
		strLanguageText = CStringUtility::trim(strLanguageText, "\t");

		if (strLanguageText == "")
		{
			// line does not have language text set
			return;
		}
		
		m_umapTranslatedText[strInternalName] = strLanguageText;
	}
}