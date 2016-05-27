#include "CLocalizationManager.h"
#include "CLocalizationFormat.h"
#include "String/CStringUtility.h"
#include "Path/CPathUtility.h"
#include "File/CFileUtility.h"

using namespace std;

void											CLocalizationManager::init(void)
{
}
void											CLocalizationManager::uninit(void)
{
}

void											CLocalizationManager::loadTranslatedText(void)
{
	CLocalizationFormat *pLocalizationFile = nullptr;

	eLanguage eActiveLanguage = getActiveLanguage();

	string
		strActiveLanguageName = getActiveLanguageName(),
		strInstallationPath = getInstallationPath(),
		strLocalizationFilePath_ActiveLanguage = "KGM Localization - " + strActiveLanguageName + ".txt",
		strLocalizationFilePath_English = "KGM Localization - English.txt"
	;
	bool
		bLocalizationFilesExistInExeFolder = CFileUtility::doesFileExist(strLocalizationFilePath_ActiveLanguage) && CFileUtility::doesFileExist(strLocalizationFilePath_English);

	if (!bLocalizationFilesExistInExeFolder)
	{
		strInstallationPath = CPathUtility::addSlashToEnd(strInstallationPath);
		strLocalizationFilePath_ActiveLanguage = strInstallationPath + "Localization/KGM Localization - " + strActiveLanguageName + ".txt";
		strLocalizationFilePath_English = strInstallationPath + "Localization/KGM Localization - English.txt";
	}

	// load text for active language
	pLocalizationFile = new CLocalizationFormat;
	pLocalizationFile->unserializeViaFile(strLocalizationFilePath_ActiveLanguage);
	if(!pLocalizationFile->doesHaveError())
	{
		m_umapTranslatedText_ActiveLanguage = pLocalizationFile->getTranslatedText();
	}
	delete pLocalizationFile;

	// load text for English (fallback language)
	pLocalizationFile = new CLocalizationFormat;
	pLocalizationFile->unserializeViaFile(strLocalizationFilePath_English);
	if(!pLocalizationFile->doesHaveError())
	{
		m_umapTranslatedText_English = pLocalizationFile->getTranslatedText();
	}
	delete pLocalizationFile;
}

unordered_map<string, string>					CLocalizationManager::getTranslatedTextEntries(void)
{
	unordered_map<string, string> umapTranslatedTextEntries;

	for (auto it : m_umapTranslatedText_ActiveLanguage)
	{
		string strInternalName = it.first;
		string strLanguageText = it.second;

		umapTranslatedTextEntries[strInternalName] = CStringUtility::replace(strLanguageText, "[newline]", "\n");
	}

	for (auto it : m_umapTranslatedText_English)
	{
		string strInternalName = it.first;
		string strLanguageText = it.second;

		if (umapTranslatedTextEntries.find(strInternalName) == umapTranslatedTextEntries.end())
		{
			umapTranslatedTextEntries[strInternalName] = CStringUtility::replace(strLanguageText, "[newline]", "\n");
		}
	}

	return umapTranslatedTextEntries;
}

string											CLocalizationManager::getTranslatedText(string strInternalName)
{
	if (m_umapTranslatedText_ActiveLanguage.find(strInternalName) != m_umapTranslatedText_ActiveLanguage.end())
	{
		// match in active language
		return CStringUtility::replace(m_umapTranslatedText_ActiveLanguage[strInternalName], "[newline]", "\n");
	}
	else if (m_umapTranslatedText_English.find(strInternalName) != m_umapTranslatedText_English.end())
	{
		// match in English
		return CStringUtility::replace(m_umapTranslatedText_English[strInternalName], "[newline]", "\n");
	}
	else
	{
		// no match
		if (strInternalName == "TranslatedTextNotFound")
		{
			return "Translated Text Not Found";
		}
		else
		{
			return getTranslatedText("TranslatedTextNotFound");
		}
	}
}

wstring											CLocalizationManager::getTranslatedTextW(string strInternalName)
{
	return CStringUtility::convertStdStringToStdWString(getTranslatedText(strInternalName));
}

string											CLocalizationManager::getTranslatedFormattedText(string strInternalName, ...)
{
	va_list vlArguments;
	va_start(vlArguments, strInternalName);
	string strResult = getTranslatedFormattedTextVA(strInternalName, vlArguments);
	va_end(vlArguments);
	return strResult;
}

string											CLocalizationManager::getTranslatedFormattedTextVA(string strInternalName, va_list vlArguments)
{
	/*
	return CStringUtility::sprintf(strInternalName, vlArguments);
	*/
	///*
	char *pDestinationBuffer = new char[4096];
	memset(pDestinationBuffer, '\0', 4096);

	vsprintf_s(pDestinationBuffer, 4096, getTranslatedText(strInternalName).c_str(), vlArguments);

	string strResult = "";
	strResult.append(pDestinationBuffer, strlen(pDestinationBuffer));
	delete[] pDestinationBuffer;

	return strResult;
	//*/
}

wstring											CLocalizationManager::getTranslatedFormattedTextW(string strInternalName, ...)
{
	va_list vlArguments;
	va_start(vlArguments, strInternalName);
	wstring wstrResult = CStringUtility::convertStdStringToStdWString(getTranslatedFormattedTextVA(strInternalName, vlArguments));
	va_end(vlArguments);
	return wstrResult;
}