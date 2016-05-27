#ifndef CLanguageManager_H
#define CLanguageManager_H

#include "CManager.h"
#include "Localization/eLanguage.h"
#include "Pool/CVectorPool.h"

class CLanguage;

class CLanguageManager : public CManager, public CVectorPool<CLanguage*>
{
public:
	void							init(void);
	void							uninit(void);

	CLanguage*						getLanguageById(eLanguage eLanguageValue);

private:
	void							initLanguages(void);
	void							uninitLanguages(void);
};

#endif