#include "CRWFormat.h"
#include "Engine/RW/CRWManager.h"
#include "Engine/RW/CRWVersionManager.h"
#include "CRWSection.h"
#include "Sections/CRWSection_String.h"
#include "Sections/CRWSection_TextureNative.h"
#include "CTextureEntry.h"
#include "String/CStringUtility.h"
#include "eExceptionCode.h"

using namespace std;

void					CRWFormat::unserialize(void)
{
	CRWSectionContainer::unserialize();

	if (getEntryCount() > 0)
	{
		setRWVersion(CRWManager::getInstance()->getVersionManager()->getEntryByVersionCC(getEntryByIndex(0)->getSectionRWVersion()));
	}
	else
	{
		setRWVersion(nullptr);
	}

	if (getRWVersion() != nullptr)
	{
		setGames(getRWVersion()->getGames());
	}

	if (doesHaveTextureWithInvalidTXDRasterDataFormat())
	{
		throw EXCEPTION_UNKNOWN_FORMAT;
	}
}

void					CRWFormat::serialize(void)
{
	uint32 uiRWVersionCC;
	if (getEntryCount() == 0)
	{
		uiRWVersionCC = CRWManager::getInstance()->getVersionManager()->getEntryByVersionId(RW_VERSION_3_4_0_3)->getVersionCC();
	}
	else
	{
		uiRWVersionCC = getEntryByIndex(0)->getSectionRWVersion();
	}

	CRWManager::getInstance()->setRWVersionCCForSerialization(uiRWVersionCC);

	CRWSectionContainer::serialize();
}

void					CRWFormat::loadTextureEntries(void)
{
	static CTextureEntry *pTextureEntry = nullptr;
	static bool bIsDiffuseTexture = true;

	for (CRWSection *pRWSection : getSectionsByType(RW_SECTION_STRING))
	{
		CRWSection_String *pRWSection_String = (CRWSection_String*)pRWSection;

		if (pRWSection_String->getData().length() != 0)
		{
			if (pTextureEntry == nullptr)
			{
				pTextureEntry = new CTextureEntry;
				pTextureEntry->setRWTextureSection((CRWSection_Texture*)getParentNode()->getParentNode());
			}

			if (bIsDiffuseTexture)
			{
				pTextureEntry->setHasDiffuse(true);
				pTextureEntry->setDiffuseName(pRWSection_String->getData());
			}
			else
			{
				pTextureEntry->setHasAlpha(true);
				pTextureEntry->setAlphaName(pRWSection_String->getData());
			}
		}

		if (!bIsDiffuseTexture)
		{
			m_vecTextureEntries.push_back(pTextureEntry);
			pTextureEntry = nullptr;
		}

		bIsDiffuseTexture = !bIsDiffuseTexture;
	}
}

void									CRWFormat::setRWVersion(CRWVersion *pRWVersion, eRWSection eRWSectionValue)
{
	if (eRWSectionValue == -1)
	{
		m_pRWVersion = pRWVersion;
	}
	else
	{
		// todo
	}
}

CRWVersion*								CRWFormat::getRWVersion(eRWSection eRWSectionValue)
{
	if (eRWSectionValue == -1)
	{
		return m_pRWVersion;
	}
	else
	{
		// todo
	}
}

void				CRWFormat::fixAlphaTextureStates(void)
{
	vector<CTextureEntry*> vecTexturesToRemove;
	for (auto pTextureEntry : m_vecTextureEntries)
	{
		if (pTextureEntry->doesHaveDiffuse() && CStringUtility::toUpperCase(pTextureEntry->getDiffuseName()).c_str()[pTextureEntry->getDiffuseName().length() - 1] == 'A')
		{
			CTextureEntry *pTextureEntry2 = getTextureByDiffuseName(pTextureEntry->getDiffuseName().substr(0, pTextureEntry->getDiffuseName().length() - 1));
			if (pTextureEntry2 != nullptr)
			{
				vecTexturesToRemove.push_back(pTextureEntry);
				pTextureEntry2->setHasAlpha(true);
				pTextureEntry2->setAlphaName(pTextureEntry->getDiffuseName());
			}
		}
	}
	for (auto pTextureEntry : vecTexturesToRemove)
	{
		auto it = std::find(m_vecTextureEntries.begin(), m_vecTextureEntries.end(), pTextureEntry);
		m_vecTextureEntries.erase(it);
		delete pTextureEntry;
	}
}

void				CRWFormat::removeTextureEntry(CTextureEntry *pTextureEntry)
{
	string strDiffuseName = CStringUtility::toUpperCase(pTextureEntry->getDiffuseName());
	string strAlphaName = CStringUtility::toUpperCase(pTextureEntry->getAlphaName());
	auto it = std::find(m_vecTextureEntries.begin(), m_vecTextureEntries.end(), pTextureEntry);
	if (it != m_vecTextureEntries.end())
	{
		m_vecTextureEntries.erase(it);

		if (m_umapTexturesByNameUpper.count(strDiffuseName) == 1)
		{
			m_umapTexturesByNameUpper.erase(strDiffuseName);
		}
		if (m_umapTexturesByNameUpper.count(strAlphaName) == 1)
		{
			m_umapTexturesByNameUpper.erase(strAlphaName);
		}
	}
}

CTextureEntry*		CRWFormat::getTextureByDiffuseName(string strTextureDiffuseName)
{
	strTextureDiffuseName = CStringUtility::toUpperCase(strTextureDiffuseName);
	for (auto pTextureEntry : m_vecTextureEntries)
	{
		if (pTextureEntry->doesHaveDiffuse() && CStringUtility::toUpperCase(pTextureEntry->getDiffuseName()) == strTextureDiffuseName)
		{
			return pTextureEntry;
		}
	}
	return nullptr;
}

CTextureEntry*		CRWFormat::getTextureByDiffuseOrAlphaName(string strTextureName)
{
	strTextureName = CStringUtility::toUpperCase(strTextureName);
	if (m_umapTexturesByNameUpper.count(strTextureName) == 1)
	{
		return m_umapTexturesByNameUpper[strTextureName];
	}
	else
	{
		return nullptr;
	}
}

vector<string>		CRWFormat::getTextureNames(void)
{
	vector<string> vecTextureNames;
	for (auto pTextureEntry : m_vecTextureEntries)
	{
		if (pTextureEntry->doesHaveDiffuse())
		{
			vecTextureNames.push_back(pTextureEntry->getDiffuseName());
		}
		if (pTextureEntry->doesHaveAlpha())
		{
			vecTextureNames.push_back(pTextureEntry->getAlphaName());
		}
	}
	return vecTextureNames;
}

bool				CRWFormat::doesHaveTextureWithInvalidTXDRasterDataFormat(void)
{
	for (auto pTexture : getSectionsByType(RW_SECTION_TEXTURE_NATIVE))
	{
		if (!((CRWSection_TextureNative*)pTexture)->doesHaveValidTXDRasterDataFormat())
		{
			return true;
		}
	}
	return false;
}