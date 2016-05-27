#include "CIDEManager.h"
#include "CIDEFormat.h"
#include "eIDESection.h"
#include "String/CStringUtility.h"
#include "Vector/CVectorUtility.h"
#include "Testing/CDebugger.h"

using namespace std;

CIDEManager::CIDEManager(void)
{
}

void			CIDEManager::init(void)
{
}
void			CIDEManager::uninit(void)
{
}

vector<string>		CIDEManager::getIDEEntryNamesWithoutExtension(vector<string> vecIDEPaths, bool bModelNames, bool bTXDNames)
{
	vector<string> vecEntryNamesWithoutExtension;
	CIDEFormat *pIDEFile = nullptr;
	for (auto strIDEPath : vecIDEPaths)
	{
		pIDEFile = CIDEManager::getInstance()->parseViaFile(strIDEPath);
		if (pIDEFile->doesHaveError())
		{
			pIDEFile->unload();
			delete pIDEFile;
			continue;
		}
		vector<string>
			vecModelNames,
			vecTextureNames;
		if (bModelNames)
		{
			vecModelNames = pIDEFile->getModelNames();
		}
		if (bTXDNames)
		{
			vecTextureNames = pIDEFile->getTXDNames();
		}
		vecEntryNamesWithoutExtension = CVectorUtility::combineVectors(vecEntryNamesWithoutExtension, CVectorUtility::combineVectors(vecModelNames, vecTextureNames));
		pIDEFile->unload();
		delete pIDEFile;
	}
	return CVectorUtility::removeDuplicates(vecEntryNamesWithoutExtension);
}