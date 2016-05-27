#ifndef CLocalizationFormat_H
#define CLocalizationFormat_H

#include "CFormat.h"
#include "eLanguage.h"
#include <string>
#include <unordered_map>

class CLocalizationFormat : public CFormat
{
public:
	CLocalizationFormat(void) : CFormat(false) {};

	void											setLanguage(eLanguage eActiveLanguage) { m_eLanguage = eActiveLanguage; }
	eLanguage										setLanguage(void) { return m_eLanguage; }

	std::unordered_map<std::string, std::string>	getTranslatedText(void) { return m_umapTranslatedText; }

private:
	void											unserialize(void);
	void											serialize(void); 
	
	void											unserializeLine(void);

private:
	eLanguage										m_eLanguage;
	std::unordered_map<std::string, std::string>	m_umapTranslatedText;
};

#endif