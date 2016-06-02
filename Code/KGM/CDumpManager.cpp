#pragma warning(disable : 4005)

#include "CDumpManager.h"
#include "CKGM.h"
#include "Globals.h"
#include "GUI/Editors/CIMGEditor.h"
#include "GUI/Editors/Tab/CIMGEditorTab.h"
#include "CPopupGUIManager.h"
#include "IMG/CIMGFormat.h"
#include "GUI/CGUIUtility.h"
#include "Path/CPathUtility.h"
#include "String/CStringUtility.h"
#include "File/CFileUtility.h"
#include "IMG/CIMGManager.h"
#include "IMG/CIMGEntry.h"
#include "DAT/Loader/CDATLoaderManager.h"
#include "DAT/Loader/CDATLoaderFormat.h"
#include "DAT/Loader/CDATLoaderEntry.h"
#include "TXD/CTXDManager.h"
#include "TXD/CTXDFormat.h"
#include "RW/Sections/CRWSection_TextureNative.h"
#include "BMP/CBMPManager.h"
#include "BMP/CBMPFormat.h"
#include "CUR/CCURManager.h"
#include "CUR/CCURFormat.h"
#include "ICO/CICOManager.h"
#include "ICO/CICOFormat.h"
#include "Bitmap.h"
#include "Testing/CDebugger.h"
#include "Vector/CVectorUtility.h"
#include "Task/CTaskManager.h"
#include "Task/CTaskDispatchManager.h"
#include "IMG/CIMGEntry.h"
#include "WTD/CWTDManager.h"
#include "WTD/CWTDFormat.h"
#include "WTD/CWTDEntry.h"
#include "Image/CImageManager.h"
#include "Image/CImageUtility.h"
#include "DDS/CDDSFormat.h"
#include "Localization/CLocalizationManager.h"
#include "CSettingsManager.h"
#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;

INT GetEncoderClsid(const WCHAR* format, CLSID* pClsid);  // helper function

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

void		CDumpManager::process(void)
{
	getKGM()->getTaskManager()->onTaskPause();
	CDumpDialogData *pDumpDialogData = getKGM()->getPopupGUIManager()->showDumpDialog(getKGM()->getLastUsedDirectory("DUMP__DAT"), getKGM()->getLastUsedDirectory("DUMP__Destination"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (!pDumpDialogData->m_bResult)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestDump", true);
		delete pDumpDialogData;
		return;
	}
	if (pDumpDialogData->m_uiDumpType == 2)
	{
		getKGM()->setLastUsedDirectory("DUMP__DAT", pDumpDialogData->m_strDATPath);
	}
	getKGM()->setLastUsedDirectory("DUMP__Destination", pDumpDialogData->m_strDumpDestinationFolderPath);

	// choose img files
	vector<CIMGFormat*> veCIMGFormats;
	if (pDumpDialogData->m_uiDumpType == 0) // All entries in active tab
	{
		if (getKGM()->getEntryListTab() == nullptr)
		{
			delete pDumpDialogData;
			getKGM()->getTaskManager()->onTaskEnd("onRequestDump", true);
			return;
		}

		veCIMGFormats.push_back(getKGM()->getEntryListTab()->getIMGFile());
	}
	else if (pDumpDialogData->m_uiDumpType == 4) // Selected entries in active tab
	{
		if (getKGM()->getEntryListTab() == nullptr)
		{
			delete pDumpDialogData;
			getKGM()->getTaskManager()->onTaskEnd("onRequestDump", true);
			return;
		}

		veCIMGFormats.push_back(getKGM()->getEntryListTab()->getIMGFile());
	}
	else if (pDumpDialogData->m_uiDumpType == 1) // All entries in all tabs
	{
		veCIMGFormats = getKGM()->getIMGEditor()->getAllMainWindowTabsIMGFiles();
	}
	else if (pDumpDialogData->m_uiDumpType == 2) // DAT file
	{
		ePlatformedGame ePlatformedGameValue = PLATFORMED_GAME_UNKNOWN;
		switch (pDumpDialogData->m_uiDATOptionIndex)
		{
		case 0: // GTA III
			ePlatformedGameValue = PLATFORMED_GAME_PC_GTA_III;
			break;
		case 1: // GTA VC
			ePlatformedGameValue = PLATFORMED_GAME_PC_GTA_VC;
			break;
		case 2: // GTA SA
			ePlatformedGameValue = PLATFORMED_GAME_PC_GTA_SA;
			break;
		case 3: // SOL
			ePlatformedGameValue = PLATFORMED_GAME_PC_SOL;
			break;
		case 4: // Other
			break;
		}
		string strDATPath = pDumpDialogData->m_strGameDirectoryPath + CDATLoaderManager::getDefaultGameDATSubPath(ePlatformedGameValue);

		CDATLoaderFormat *pDATFile = CDATLoaderManager::getInstance()->parseViaFile(strDATPath);
		if (!pDATFile->doesHaveError())
		{
			veCIMGFormats = pDATFile->parseIMGFiles(pDumpDialogData->m_strGameDirectoryPath);
		}
		pDATFile->unload();
		delete pDATFile;

		vector<string> vecGameIMGPaths = CIMGManager::getDefaultGameIMGSubPaths(ePlatformedGameValue);

		for (auto strIMGRelativePath : vecGameIMGPaths)
		{
			string strIMGPath = pDumpDialogData->m_strGameDirectoryPath + strIMGRelativePath;
			if (CFileUtility::doesFileExist(strIMGPath))
			{
				CIMGFormat *pIMGFile = CIMGManager::getInstance()->parseViaFile(strIMGPath);
				if(!pIMGFile->doesHaveError())
				{
					veCIMGFormats.push_back(pIMGFile);
				}
			}
		}
	}
	else if (pDumpDialogData->m_uiDumpType == 3) // Game IMGs
	{
		ePlatformedGame ePlatformedGameValue2 = PLATFORMED_GAME_UNKNOWN;
		switch (pDumpDialogData->m_uiDATOptionIndex)
		{
		case 0: // GTA III
			ePlatformedGameValue2 = PLATFORMED_GAME_PC_GTA_III;
			break;
		case 1: // GTA VC
			ePlatformedGameValue2 = PLATFORMED_GAME_PC_GTA_VC;
			break;
		case 2: // GTA SA
			ePlatformedGameValue2 = PLATFORMED_GAME_PC_GTA_SA;
			break;
		case 3: // SOL
			ePlatformedGameValue2 = PLATFORMED_GAME_PC_SOL;
			break;
		case 4: // Other
			break;
		}

		vector<string> vecGameIMGPaths = CIMGManager::getDefaultGameIMGSubPaths(ePlatformedGameValue2);

		for (auto strIMGRelativePath : vecGameIMGPaths)
		{
			string strIMGPath = pDumpDialogData->m_strGameDirectoryPath + strIMGRelativePath;
			if (CFileUtility::doesFileExist(strIMGPath))
			{
				CIMGFormat *pIMGFile = CIMGManager::getInstance()->parseViaFile(strIMGPath);
				if(!pIMGFile->doesHaveError())
				{
					veCIMGFormats.push_back(pIMGFile);
				}
			}
		}
	}

	// dump folder path
	pDumpDialogData->m_strDumpDestinationFolderPath = CPathUtility::addSlashToEnd(pDumpDialogData->m_strDumpDestinationFolderPath);

	// progress bar
	uint32 uiProgressMaxTicks = 0;
	for (auto pIMGFile : veCIMGFormats)
	{
		uiProgressMaxTicks += pIMGFile->getEntryCount();
	}
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(uiProgressMaxTicks);

	// dump
	vector<string>
		vecDumpedEntryNames,
		vecTextureNames,
		vecCorruptTXDs,
		vecTooLargeTXDs,
		vecInvalidResolutionTXDs,
		vecInvalidTextureNames,
		vecTXDsContainingTooManyTextures,
		vecMipmapSkippedEntries;
	uint32
		uiIMGsEntryCount = 0,
		uiDumpedTextureCount = 0;
		//uiMipmapSkippedCount = 0;

	for (auto pIMGFile : veCIMGFormats)
	{
		// choose IMG entries to dump
		vector<CIMGEntry*> vecIMGEntries;
		if (pDumpDialogData->m_uiDumpType == 4) // selected entries
		{
			vecIMGEntries = getKGM()->getEntryListTab()->getSelectedEntries();
		}
		else
		{
			vecIMGEntries = pIMGFile->getEntries();
		}

		// dump IMG entries
		for (auto pIMGEntry : vecIMGEntries)
		{
			string strExtension = CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName()));
			if (strExtension == "COL" || strExtension == "WBN" || strExtension == "WBD")
			{
				if (std::find(pDumpDialogData->m_vecDumpExtensions.begin(), pDumpDialogData->m_vecDumpExtensions.end(), "COL") != pDumpDialogData->m_vecDumpExtensions.end())
				{
					vecDumpedEntryNames.push_back(pIMGEntry->getEntryName());
					pIMGFile->exportSingle(pIMGEntry, pDumpDialogData->m_strDumpDestinationFolderPath + strExtension + "/");
				}
			}
			else if (strExtension == "DFF" || strExtension == "BSP" || strExtension == "MDL" || strExtension == "WDR" || strExtension == "WDD")
			{
				if (std::find(pDumpDialogData->m_vecDumpExtensions.begin(), pDumpDialogData->m_vecDumpExtensions.end(), "DFF") != pDumpDialogData->m_vecDumpExtensions.end())
				{
					vecDumpedEntryNames.push_back(pIMGEntry->getEntryName());
					pIMGFile->exportSingle(pIMGEntry, pDumpDialogData->m_strDumpDestinationFolderPath + strExtension + "/");
				}
			}
			else if (strExtension == "IPL" || strExtension == "WPL")
			{
				if (std::find(pDumpDialogData->m_vecDumpExtensions.begin(), pDumpDialogData->m_vecDumpExtensions.end(), "IPL") != pDumpDialogData->m_vecDumpExtensions.end())
				{
					vecDumpedEntryNames.push_back(pIMGEntry->getEntryName());
					pIMGFile->exportSingle(pIMGEntry, pDumpDialogData->m_strDumpDestinationFolderPath + strExtension + "/");
				}
			}
			else if (strExtension == "TXD" || strExtension == "WTD")
			{
				if (std::find(pDumpDialogData->m_vecDumpExtensions.begin(), pDumpDialogData->m_vecDumpExtensions.end(), "TXD") != pDumpDialogData->m_vecDumpExtensions.end())
				{
					vecDumpedEntryNames.push_back(pIMGEntry->getEntryName());
					pIMGFile->exportSingle(pIMGEntry, pDumpDialogData->m_strDumpDestinationFolderPath + strExtension + "/");

					string strTXDData = pIMGEntry->getEntryData();
					if (!CTXDFormat::isTXDSizeValid(strTXDData.size()))
					{
						vecTooLargeTXDs.push_back(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_TXD_MaxSize", pIMGEntry->getEntryName().c_str()));
					}

					CTXDFormat *pTXDFile = CTXDManager::getInstance()->parseViaMemory(strTXDData);
					if (pTXDFile->doesHaveError())
					{
						vecCorruptTXDs.push_back(pIMGEntry->getEntryName());
					}

					if (!CTXDFormat::isTextureCountValid(pTXDFile->getTextures().size(), pTXDFile->getGames()))
					{
						vecTXDsContainingTooManyTextures.push_back(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_TextureCount", pIMGEntry->getEntryName().c_str(), pTXDFile->getTextures().size()));
					}

					uint32 uiTextureIndex = 0;
					for (auto pTexture : pTXDFile->getTextures())
					{
						if (!CTXDFormat::isTextureResolutionValid(pTexture->getImageSize().m_x, pTexture->getImageSize().m_y, pTXDFile->getGames()))
						{
							vecInvalidResolutionTXDs.push_back("[" + pIMGEntry->getEntryName() + "] " + pTexture->getDiffuseName() + " (" + CStringUtility::toString(pTexture->getImageSize().m_x) + " x " + CStringUtility::toString(pTexture->getImageSize().m_y) + ")");
						}

						if (!CTXDFormat::isTextureNameValid(pTexture->getDiffuseName()) || !CTXDFormat::isTextureNameValid(pTexture->getAlphaName(), true))
						{
							vecInvalidTextureNames.push_back(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_TextureIndex", pIMGEntry->getEntryName().c_str(), uiTextureIndex + 1));
						}

						uiTextureIndex++;
					}
					
					pTXDFile->unload();
					delete pTXDFile;
				}

				if (std::find(pDumpDialogData->m_vecDumpExtensions.begin(), pDumpDialogData->m_vecDumpExtensions.end(), "Texture Images") != pDumpDialogData->m_vecDumpExtensions.end())
				{
					//vector<CIMGEntry*> vecIMGEntries;
					//vecIMGEntries.push_back(pIMGEntry);
					//CIMGManager::getInstance()->exportEntries(pIMGFile, vecIMGEntries, pDumpDialogData->m_strDumpDestinationFolderPath + strExtension + "/");

					string strEntryExtensionUpper = CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName()));
					if (strEntryExtensionUpper != "TXD" && strEntryExtensionUpper != "WTD")
					{
						getKGM()->getTaskManager()->onTaskProgressTick();
						continue;
					}


					if (strEntryExtensionUpper == "TXD")
					{
						string strTXDData = pIMGEntry->getEntryData();

						if (!CTXDFormat::isTXDSizeValid(strTXDData.size()))
						{
							vecTooLargeTXDs.push_back(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_TXD_MaxSize", pIMGEntry->getEntryName().c_str()));
						}
						CTXDFormat *pTXDFile = CTXDManager::getInstance()->parseViaMemory(strTXDData);
						if (pTXDFile->doesHaveError())
						{
							vecCorruptTXDs.push_back(pIMGEntry->getEntryName());
							getKGM()->getTaskManager()->onTaskProgressTick();
							continue;
						}
						if (!CTXDFormat::isTextureCountValid(pTXDFile->getTextures().size(), pTXDFile->getGames()))
						{
							vecTXDsContainingTooManyTextures.push_back(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_TextureCount", pIMGEntry->getEntryName().c_str(), pTXDFile->getTextures().size()));
						}

						for (auto strImageType : pDumpDialogData->m_vecDumpImageTypes)
						{
							uint32 uiTextureIndex = 0;
							for (auto pTexture : pTXDFile->getTextures())
							{
								if (pTexture->getDiffuseName().length() == 0)
								{
									continue;
								}

								/*
								CDebugger::log("pTexture->m_strDiffuseName: [" + pIMGEntry->getEntryName() + "] " + pTexture->getDiffuseName());
								CDebugger::log("pTexture->m_usWidth: " + CStringUtility::toString(pTexture->getImageSize(true)));
								CDebugger::log("pTexture->m_usHeight: " + CStringUtility::toString(pTexture->getImageSize(false)));
								CDebugger::log("pTexture->m_uiRasterFormat: " + CStringUtility::toString(pTexture->getRasterFormat()));
								CDebugger::log("pTexture->m_ucBPP: " + CStringUtility::toString(pTexture->getBPP()));
								*/

								if (!CTXDFormat::isTextureResolutionValid((uint16)pTexture->getImageSize().m_x, (uint16)pTexture->getImageSize().m_y, pTXDFile->getGames()))
								{
									vecInvalidResolutionTXDs.push_back("[" + pIMGEntry->getEntryName() + "] " + pTexture->getDiffuseName() + " (" + CStringUtility::toString(pTexture->getImageSize().m_x) + " x " + CStringUtility::toString(pTexture->getImageSize().m_y) + ")");
								}

								if (!CTXDFormat::isTextureNameValid(pTexture->getDiffuseName()) || !CTXDFormat::isTextureNameValid(pTexture->getAlphaName(), true))
								{
									vecInvalidTextureNames.push_back(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_TextureIndex", pIMGEntry->getEntryName().c_str(), uiTextureIndex + 1));
								}

								string strTextureNamesLogLine;
								if (pTexture->doesHaveDiffuse())
								{
									if (pTexture->doesHaveAlpha())
									{
										strTextureNamesLogLine = pTexture->getDiffuseName() + " (" + CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_AlphaTextureName", pTexture->getAlphaName().c_str()) + ")";
									}
									else
									{
										strTextureNamesLogLine = pTexture->getDiffuseName();
									}
								}
								else
								{
									if (pTexture->doesHaveAlpha())
									{
										strTextureNamesLogLine = CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_AlphaTextureName", pTexture->getAlphaName().c_str());
									}
									else
									{
										strTextureNamesLogLine = CLocalizationManager::getInstance()->getTranslatedText("Log_TextureNameNotFound");
									}
								}
								vecTextureNames.push_back(strTextureNamesLogLine);

								// choose whether to dump all mipmaps in the texture, or just the first mipmap in the texture
								vector<CRWEntry_TextureNative_MipMap*> vecTextureMipmapsToDump;
								if (pDumpDialogData->m_bDumpAllTextureMipmaps)
								{
									vecTextureMipmapsToDump = pTexture->getMipMaps().getEntries();
								}
								else
								{
									if (pTexture->getEntryCount() > 0)
									{
										vecTextureMipmapsToDump.push_back(pTexture->getMipMaps().getEntries()[0]);
									}
								}

								// dump mipmap(s)
								uint32 uiMipmapIndex = 0;
								for (auto pMipmap : vecTextureMipmapsToDump)
								{
									string strImageDataBGRA = pMipmap->getRasterDataBGRA32();
									if (strImageDataBGRA == "")
									{
										vecMipmapSkippedEntries.push_back(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_TextureInfo", pIMGEntry->getEntryName().c_str(), uiTextureIndex + 1, pTexture->getDiffuseName().c_str(), uiMipmapIndex + 1, pMipmap->getImageSize().m_x, pMipmap->getImageSize().m_y));
										//uiMipmapSkippedCount++;
										uiMipmapIndex++;
										continue;
									}

									CBMPFormat *pBMPFile = new CBMPFormat;
									pBMPFile->setWidth(pMipmap->getImageSize().m_x);
									pBMPFile->setHeight(pMipmap->getImageSize().m_y);
									pBMPFile->setBPP(32);

									pBMPFile->setRasterData(strImageDataBGRA);
									pBMPFile->swapRows();

									if (strImageType == "BMP")
									{
										string strBMPFilePath;
										if (pDumpDialogData->m_bDumpTextureImagesAsFolders)
										{
											strBMPFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + pTexture->getDiffuseName() + ".BMP";
										}
										else
										{
											strBMPFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + pTexture->getDiffuseName() + ".BMP";
										}
										strBMPFilePath = CPathUtility::getNextFileName(strBMPFilePath, uiMipmapIndex, "-Mipmap");

										pBMPFile->setBMPVersion(3);
										pBMPFile->serializeViaFile(strBMPFilePath);
									}
									else if (strImageType == "ICO")
									{
										string strICOFilePath;
										if (pDumpDialogData->m_bDumpTextureImagesAsFolders)
										{
											strICOFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + pTexture->getDiffuseName() + ".ICO";
										}
										else
										{
											strICOFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + pTexture->getDiffuseName() + ".ICO";
										}
										strICOFilePath = CPathUtility::getNextFileName(strICOFilePath, uiMipmapIndex, "-Mipmap");

										CICOFormat *pICOFormat = CICOManager::getInstance()->createFormatFromBMP(pBMPFile);
										pICOFormat->serializeViaFile(strICOFilePath);
										pICOFormat->unload();
										delete pICOFormat;
									}
									else if (strImageType == "CUR")
									{
										string strCURFilePath;
										if (pDumpDialogData->m_bDumpTextureImagesAsFolders)
										{
											strCURFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + pTexture->getDiffuseName() + ".CUR";
										}
										else
										{
											strCURFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + pTexture->getDiffuseName() + ".CUR";
										}
										strCURFilePath = CPathUtility::getNextFileName(strCURFilePath, uiMipmapIndex, "-Mipmap");
										
										CCURFormat *pCURFormat = CCURManager::getInstance()->createFormatFromBMP(pBMPFile);
										pCURFormat->serializeViaFile(strCURFilePath);
										pCURFormat->unload();
										delete pCURFormat;
									}
									else if (strImageType == "DDS")
									{
										string strDDSFilePath;
										if (pDumpDialogData->m_bDumpTextureImagesAsFolders)
										{
											strDDSFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + pTexture->getDiffuseName() + ".DDS";
										}
										else
										{
											strDDSFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + pTexture->getDiffuseName() + ".DDS";
										}
										strDDSFilePath = CPathUtility::getNextFileName(strDDSFilePath, uiMipmapIndex, "-Mipmap");

										CDDSFormat ddsFile;
										ddsFile.m_uiWidth = pMipmap->getImageSize().m_x;
										ddsFile.m_uiHeight = pMipmap->getImageSize().m_y;
										//ddsFile.m_strRasterData = CImageManager::swapRowsAndColumns(CImageManager::convertBGRA32ToRGBA32(strImageDataBGRA), ddsFile.m_uiWidth, ddsFile.m_uiHeight);
										ddsFile.m_strRasterData = CImageUtility::convertBGRA32ToDXT(CImageUtility::swapRowsAndColumns(strImageDataBGRA, ddsFile.m_uiWidth, ddsFile.m_uiHeight), DXT_1, ddsFile.m_uiWidth, ddsFile.m_uiHeight);
										ddsFile.serializeViaFile(strDDSFilePath);
									}
									else if (strImageType == "TGA")
									{
										string
											strTempBMPPath,
											strImagePath;
										if (pDumpDialogData->m_bDumpTextureImagesAsFolders)
										{
											strTempBMPPath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + "__TEMP_" + pTexture->getDiffuseName() + ".BMP";
											strImagePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + pTexture->getDiffuseName() + "." + strImageType;
										}
										else
										{
											strTempBMPPath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/__TEMP_" + pTexture->getDiffuseName() + ".BMP";
											strImagePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + pTexture->getDiffuseName() + "." + strImageType;
										}
										strImagePath = CPathUtility::getNextFileName(strImagePath, uiMipmapIndex, "-Mipmap");

										pBMPFile->setBMPVersion(3);
										pBMPFile->serializeViaFile(strTempBMPPath);

										DHPOBitmap *pBmp = new DHPOBitmap;
										pBmp->loadImage(CStringUtility::convertStdStringToStdWString(strTempBMPPath).c_str());
										pBmp->saveImage(CStringUtility::convertStdStringToStdWString(strImagePath).c_str());
										delete pBmp;

										CFileUtility::removeFile(strTempBMPPath);
									}
									else
									{
										// PNG
										string
											strTempBMPPath,
											strImagePath;
										if (pDumpDialogData->m_bDumpTextureImagesAsFolders)
										{
											strTempBMPPath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + "__TEMP_" + pTexture->getDiffuseName() + ".BMP";
											strImagePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + pTexture->getDiffuseName() + "." + strImageType;
										}
										else
										{
											strTempBMPPath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/__TEMP_" + pTexture->getDiffuseName() + ".BMP";
											strImagePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + pTexture->getDiffuseName() + "." + strImageType;
										}
										strImagePath = CPathUtility::getNextFileName(strImagePath, uiMipmapIndex, "-Mipmap");

										pBMPFile->setBMPVersion(3);
										pBMPFile->serializeViaFile(strTempBMPPath);

										GdiplusStartupInput gdiplusStartupInput;
										ULONG_PTR gdiplusToken;
										GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

										// Create an Image object based on a PNG file.
										Image  *image = new Image(CStringUtility::convertStdStringToStdWString(strTempBMPPath).c_str());

										// Save the image.
										CLSID gifClsid;
										GetEncoderClsid(CStringUtility::convertStdStringToStdWString(getEncoderClassIdFromImageExtension(strImageType)).c_str(), &gifClsid);
										image->Save(CStringUtility::convertStdStringToStdWString(strImagePath).c_str(), &gifClsid, NULL);

										delete image;
										GdiplusShutdown(gdiplusToken);

										CFileUtility::removeFile(strTempBMPPath);
									}

									pBMPFile->unload();
									delete pBMPFile;
									uiMipmapIndex++;
								}
								uiTextureIndex++;
							}
						}

						pTXDFile->unload();
						delete pTXDFile;
					}
					else if (strEntryExtensionUpper == "WTD")
					{
						CWTDFormat *pWTDFile = CWTDManager::getInstance()->parseViaMemory(pIMGEntry->getEntryData());
						
						if (pWTDFile->doesHaveError())
						{
							vecCorruptTXDs.push_back(pIMGEntry->getEntryName());
							getKGM()->getTaskManager()->onTaskProgressTick();
							continue;
						}

						for (auto strImageType : pDumpDialogData->m_vecDumpImageTypes)
						{
							uint32 uiTextureIndex = 0;
							for (auto pWTDEntry : pWTDFile->getEntries())
							{
								if (pWTDEntry->getEntryName().length() == 0)
								{
									continue;
								}

								string strTextureNamesLogLine = pWTDEntry->getEntryName();
								vecTextureNames.push_back(strTextureNamesLogLine);

								// choose whether to dump all mipmaps in the texture, or just the first mipmap in the texture
								vector<CWTDMipmap*> vecTextureMipmapsToDump;
								if (pDumpDialogData->m_bDumpAllTextureMipmaps)
								{
									vecTextureMipmapsToDump = pWTDEntry->getEntries();
								}
								else
								{
									if (pWTDEntry->getEntryCount() > 0)
									{
										vecTextureMipmapsToDump.push_back(pWTDEntry->getEntries()[0]);
									}
								}

								// dump mipmap(s)
								uint32 uiMipmapIndex = 0;
								for (auto pMipmap : vecTextureMipmapsToDump)
								{
									string strImageDataBGRA = pMipmap->getRasterDataBGRA32();
									if (strImageDataBGRA == "")
									{
										vecMipmapSkippedEntries.push_back(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_TextureInfo", pIMGEntry->getEntryName().c_str(), uiTextureIndex + 1, pWTDEntry->getEntryName().c_str(), uiMipmapIndex + 1, pMipmap->getImageSize(true), pMipmap->getImageSize(false)));
										//uiMipmapSkippedCount++;
										uiMipmapIndex++;
										continue;
									}

									CBMPFormat *pBMPFile = new CBMPFormat;
									pBMPFile->setWidth(pMipmap->getImageSize(true));
									pBMPFile->setHeight(pMipmap->getImageSize(false));
									pBMPFile->setBPP(32);

									pBMPFile->setRasterData(strImageDataBGRA);
									pBMPFile->swapRows();

									if (strImageType == "BMP")
									{
										string strBMPFilePath;
										if (pDumpDialogData->m_bDumpTextureImagesAsFolders)
										{
											strBMPFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + pWTDEntry->getEntryName() + ".BMP";
										}
										else
										{
											strBMPFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + pWTDEntry->getEntryName() + ".BMP";
										}
										strBMPFilePath = CPathUtility::getNextFileName(strBMPFilePath, uiMipmapIndex, "-Mipmap");
										
										pBMPFile->setBMPVersion(3);
										pBMPFile->serializeViaFile(strBMPFilePath);
									}
									else if (strImageType == "ICO")
									{
										string strICOFilePath;
										if (pDumpDialogData->m_bDumpTextureImagesAsFolders)
										{
											strICOFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + pWTDEntry->getEntryName() + ".ICO";
										}
										else
										{
											strICOFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + pWTDEntry->getEntryName() + ".ICO";
										}
										strICOFilePath = CPathUtility::getNextFileName(strICOFilePath, uiMipmapIndex, "-Mipmap");

										string strICOHeaderData = "";
										strICOHeaderData += CStringUtility::packUint16(0, false);
										strICOHeaderData += CStringUtility::packUint16(1, false); // 1 for icon, 2 for cursor
										strICOHeaderData += CStringUtility::packUint16(1, false); // image count
										strICOHeaderData += CStringUtility::packUint8((uint8)pBMPFile->getWidth());
										strICOHeaderData += CStringUtility::packUint8((uint8)pBMPFile->getHeight());
										strICOHeaderData += CStringUtility::packUint8(0); // palette colour count
										strICOHeaderData += CStringUtility::packUint8(0); // reserved
										strICOHeaderData += CStringUtility::packUint16(0, false); // colour planes
										strICOHeaderData += CStringUtility::packUint16(32, false); // BPP
										strICOHeaderData += CStringUtility::packUint32((pBMPFile->getWidth() * pBMPFile->getHeight()) * (32 / 8), false); // BPP
										strICOHeaderData += CStringUtility::packUint32(22, false); // offset to BMP data from beginning of file
										
										pBMPFile->setSkipBMPFileHeaderForSerialize(true);
										pBMPFile->setBMPVersion(3);
										CFileUtility::storeFile(strICOFilePath, strICOHeaderData + pBMPFile->serializeViaMemory(), false, true);
									}
									else if (strImageType == "CUR")
									{
										string strCURFilePath;
										if (pDumpDialogData->m_bDumpTextureImagesAsFolders)
										{
											strCURFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + pWTDEntry->getEntryName() + ".CUR";
										}
										else
										{
											strCURFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + pWTDEntry->getEntryName() + ".CUR";
										}
										strCURFilePath = CPathUtility::getNextFileName(strCURFilePath, uiMipmapIndex, "-Mipmap");

										string strCURHeaderData = "";
										strCURHeaderData += CStringUtility::packUint16(0, false);
										strCURHeaderData += CStringUtility::packUint16(2, false); // 1 for icon, 2 for cursor
										strCURHeaderData += CStringUtility::packUint16(1, false); // image count
										strCURHeaderData += CStringUtility::packUint8((uint8)pBMPFile->getWidth());
										strCURHeaderData += CStringUtility::packUint8((uint8)pBMPFile->getHeight());
										strCURHeaderData += CStringUtility::packUint8(0); // palette colour count
										strCURHeaderData += CStringUtility::packUint8(0); // reserved
										strCURHeaderData += CStringUtility::packUint16((uint8)pBMPFile->getWidth() / 2, false); // x hotspot
										strCURHeaderData += CStringUtility::packUint16((uint8)pBMPFile->getHeight() / 2, false); // y hotspot
										strCURHeaderData += CStringUtility::packUint32((pBMPFile->getWidth() * pBMPFile->getHeight()) * (32 / 8), false); // BPP
										strCURHeaderData += CStringUtility::packUint32(22, false); // offset to BMP data from beginning of file
										
										pBMPFile->setSkipBMPFileHeaderForSerialize(true);
										pBMPFile->setBMPVersion(3);
										CFileUtility::storeFile(strCURFilePath, strCURHeaderData + pBMPFile->serializeViaMemory(), false, true);
									}
									else if (strImageType == "DDS")
									{
										string strDDSFilePath;
										if (pDumpDialogData->m_bDumpTextureImagesAsFolders)
										{
											strDDSFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + pWTDEntry->getEntryName() + ".DDS";
										}
										else
										{
											strDDSFilePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + pWTDEntry->getEntryName() + ".DDS";
										}

										strDDSFilePath = CPathUtility::getNextFileName(strDDSFilePath, uiMipmapIndex, "-Mipmap");

										CDDSFormat ddsFile;
										ddsFile.m_uiWidth = pMipmap->getImageSize(true);
										ddsFile.m_uiHeight = pMipmap->getImageSize(false);
										//ddsFile.m_strRasterData = CImageManager::swapRowsAndColumns(CImageManager::convertBGRA32ToRGBA32(strImageDataBGRA), ddsFile.m_uiWidth, ddsFile.m_uiHeight);
										ddsFile.m_strRasterData = CImageUtility::convertBGRA32ToDXT(CImageUtility::swapRowsAndColumns(strImageDataBGRA, ddsFile.m_uiWidth, ddsFile.m_uiHeight), DXT_1, ddsFile.m_uiWidth, ddsFile.m_uiHeight);
										ddsFile.serializeViaFile(strDDSFilePath);
									}
									else if (strImageType == "TGA")
									{
										string
											strTempBMPPath,
											strImagePath;
										if (pDumpDialogData->m_bDumpTextureImagesAsFolders)
										{
											strTempBMPPath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + "__TEMP_" + pWTDEntry->getEntryName() + ".BMP";
											strImagePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + pWTDEntry->getEntryName() + "." + strImageType;
										}
										else
										{
											strTempBMPPath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/__TEMP_" + pWTDEntry->getEntryName() + ".BMP";
											strImagePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + pWTDEntry->getEntryName() + "." + strImageType;
										}
										strImagePath = CPathUtility::getNextFileName(strImagePath, uiMipmapIndex, "-Mipmap");

										pBMPFile->setBMPVersion(3);
										pBMPFile->serializeViaFile(strTempBMPPath);

										DHPOBitmap *pBmp = new DHPOBitmap;
										pBmp->loadImage(CStringUtility::convertStdStringToStdWString(strTempBMPPath).c_str());
										pBmp->saveImage(CStringUtility::convertStdStringToStdWString(strImagePath).c_str());
										delete pBmp;

										CFileUtility::removeFile(strTempBMPPath);
									}
									else
									{
										// PNG
										string
											strTempBMPPath,
											strImagePath;
										if (pDumpDialogData->m_bDumpTextureImagesAsFolders)
										{
											strTempBMPPath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + "__TEMP_" + pWTDEntry->getEntryName() + ".BMP";
											strImagePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "/" + pWTDEntry->getEntryName() + "." + strImageType;
										}
										else
										{
											strTempBMPPath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/__TEMP_" + pWTDEntry->getEntryName() + ".BMP";
											strImagePath = pDumpDialogData->m_strDumpDestinationFolderPath + "Texture Images/" + pWTDEntry->getEntryName() + "." + strImageType;
										}
										strImagePath = CPathUtility::getNextFileName(strImagePath, uiMipmapIndex, "-Mipmap");
										
										pBMPFile->setBMPVersion(3);
										pBMPFile->serializeViaFile(strTempBMPPath);

										GdiplusStartupInput gdiplusStartupInput;
										ULONG_PTR gdiplusToken;
										GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

										// Create an Image object based on a PNG file.
										Image  *image = new Image(CStringUtility::convertStdStringToStdWString(strTempBMPPath).c_str());

										// Save the image.
										CLSID gifClsid;
										GetEncoderClsid(CStringUtility::convertStdStringToStdWString(getEncoderClassIdFromImageExtension(strImageType)).c_str(), &gifClsid);
										image->Save(CStringUtility::convertStdStringToStdWString(strImagePath).c_str(), &gifClsid, NULL);

										delete image;
										GdiplusShutdown(gdiplusToken);

										CFileUtility::removeFile(strTempBMPPath);
									}

									pBMPFile->unload();
									delete pBMPFile;
									uiMipmapIndex++;
								}
								uiTextureIndex++;
							}
						}
						
						pWTDFile->unload();
						delete pWTDFile;
					}

					uiDumpedTextureCount++;
				}
			}
			else
			{
				if (std::find(pDumpDialogData->m_vecDumpExtensions.begin(), pDumpDialogData->m_vecDumpExtensions.end(), "Other") != pDumpDialogData->m_vecDumpExtensions.end())
				{
					vecDumpedEntryNames.push_back(pIMGEntry->getEntryName());
					pIMGFile->exportSingle(pIMGEntry, pDumpDialogData->m_strDumpDestinationFolderPath + "Other/");
				}
			}

			getKGM()->getTaskManager()->onTaskProgressTick();
		}
	}

	vecCorruptTXDs = CVectorUtility::removeDuplicates(vecCorruptTXDs);
	vecTooLargeTXDs = CVectorUtility::removeDuplicates(vecTooLargeTXDs);
	vecInvalidResolutionTXDs = CVectorUtility::removeDuplicates(vecInvalidResolutionTXDs);
	vecInvalidTextureNames = CVectorUtility::removeDuplicates(vecInvalidTextureNames);
	vecDumpedEntryNames = CVectorUtility::removeDuplicates(vecDumpedEntryNames);
	vecTextureNames = CVectorUtility::removeDuplicates(vecTextureNames);
	vecTXDsContainingTooManyTextures = CVectorUtility::removeDuplicates(vecTXDsContainingTooManyTextures);
	vecMipmapSkippedEntries = CVectorUtility::removeDuplicates(vecMipmapSkippedEntries);

	// log
	if (getKGM()->getEntryListTab() != nullptr)
	{
		uint32 uiDumpedFileCount = vecDumpedEntryNames.size() + uiDumpedTextureCount;
		uint32 uiIMGFileCount = veCIMGFormats.size();
		if (pDumpDialogData->m_uiDumpType == 2)
		{
			getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_10", uiDumpedFileCount, uiIMGFileCount));
		}
		else
		{
			getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_11", uiDumpedFileCount, uiIMGFileCount));
		}
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_12"), true);
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_13", vecDumpedEntryNames.size()), true);
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_14", uiDumpedTextureCount), true);
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_15"), true);
		getKGM()->getEntryListTab()->log(CStringUtility::join(vecCorruptTXDs, "\n"), true);
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_16"), true);
		getKGM()->getEntryListTab()->log(CStringUtility::join(vecTooLargeTXDs, "\n"), true);
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_17"), true);
		getKGM()->getEntryListTab()->log(CStringUtility::join(vecTXDsContainingTooManyTextures, "\n"), true);
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_18"), true);
		getKGM()->getEntryListTab()->log(CStringUtility::join(vecInvalidResolutionTXDs, "\n"), true);
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_19"), true);
		getKGM()->getEntryListTab()->log(CStringUtility::join(vecInvalidTextureNames, "\n"), true);
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_20"), true);
		getKGM()->getEntryListTab()->log(CStringUtility::join(vecDumpedEntryNames, "\n"), true);
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_21"), true);
		getKGM()->getEntryListTab()->log(CStringUtility::join(vecTextureNames, "\n"), true);
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_22"), true);
		getKGM()->getEntryListTab()->log(CStringUtility::join(vecMipmapSkippedEntries, "\n"), true);
	}
	else
	{
		uint32 uiDumpedFileCount = vecDumpedEntryNames.size() + uiDumpedTextureCount;
		uint32 uiIMGFileCount = veCIMGFormats.size();
		if (pDumpDialogData->m_uiDumpType == 2)
		{
			getKGM()->getIMGEditor()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_10", uiDumpedFileCount, uiIMGFileCount));
		}
		else
		{
			getKGM()->getIMGEditor()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_11", uiDumpedFileCount, uiIMGFileCount));
		}
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedText("Log_12"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_13", vecDumpedEntryNames.size()), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_14", uiDumpedTextureCount), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedText("Log_15"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CStringUtility::join(vecCorruptTXDs, "\n"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedText("Log_16"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CStringUtility::join(vecTooLargeTXDs, "\n"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedText("Log_17"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CStringUtility::join(vecTXDsContainingTooManyTextures, "\n"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedText("Log_18"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CStringUtility::join(vecInvalidResolutionTXDs, "\n"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedText("Log_19"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CStringUtility::join(vecInvalidTextureNames, "\n"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedText("Log_20"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CStringUtility::join(vecDumpedEntryNames, "\n"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedText("Log_21"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CStringUtility::join(vecTextureNames, "\n"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedText("Log_22"), true);
		getKGM()->getIMGEditor()->logWithNoTabsOpen(CStringUtility::join(vecMipmapSkippedEntries, "\n"), true);
	}

	// results popup
	bool bSuccessfulResult =
		vecCorruptTXDs.size() == 0 &&
		vecTooLargeTXDs.size() == 0 &&
		vecInvalidResolutionTXDs.size() == 0 &&
		vecInvalidTextureNames.size() == 0 &&
		vecTXDsContainingTooManyTextures.size() == 0 &&
		vecMipmapSkippedEntries.size() == 0;
	if (bSuccessfulResult)
	{
		getKGM()->getTaskManager()->onTaskPause();
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_1"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_1"));
		getKGM()->getTaskManager()->onTaskUnpause();
	}
	else
	{
		CDumpResultsDialogData *pDumpResultsDialogData = new CDumpResultsDialogData;
		pDumpResultsDialogData->m_uiCorruptTXDCount = vecCorruptTXDs.size();
		pDumpResultsDialogData->m_uiTooLargeTXDCount = vecTooLargeTXDs.size();
		pDumpResultsDialogData->m_uiInvalidResolutionTXDCount = vecInvalidResolutionTXDs.size();
		pDumpResultsDialogData->m_uiInvalidTextureNameCount = vecInvalidTextureNames.size();
		pDumpResultsDialogData->m_uiTXDCountExceededTextureCountLimit = vecTXDsContainingTooManyTextures.size();
		pDumpResultsDialogData->m_uiMipmapSkippedCount = vecMipmapSkippedEntries.size();
		getKGM()->getTaskManager()->onTaskPause();
		getKGM()->getPopupGUIManager()->showDumpResultsDialog(pDumpResultsDialogData);
		getKGM()->getTaskManager()->onTaskUnpause();

		if (pDumpResultsDialogData->m_bOpenAdvancedLog)
		{
			string strFilePath = getKGM()->getSettingsManager()->getSettingString("AutomaticLoggingPath") + CStringUtility::getDateTextForFolder() + " / " + CLocalizationManager::getInstance()->getTranslatedText("LogFilename_Extended");

			if (getKGM()->getSettingsManager()->getSettingBool("AutomaticLoggingExtended") && CFileUtility::doesFileExist(strFilePath))
			{
				// automatic logging is enabled, simply open the txt file for the user
				ShellExecute(NULL, NULL, CStringUtility::convertStdStringToStdWString(strFilePath).c_str(), NULL, NULL, SW_SHOWNORMAL);
			}
			else
			{
				// ask user to save the extended log file, then open the txt file
				getKGM()->getTaskManager()->onTaskPause();
				string strSaveFilePath = getKGM()->getTaskManager()->getDispatch()->onRequestSaveLog(true, false);
				getKGM()->getTaskManager()->onTaskUnpause();
				if (strSaveFilePath != "")
				{
					ShellExecute(NULL, NULL, CStringUtility::convertStdStringToStdWString(strSaveFilePath).c_str(), NULL, NULL, SW_SHOWNORMAL);
				}
			}
		}

		delete pDumpResultsDialogData;
	}

	// clean up
	if (pDumpDialogData->m_uiDumpType == 2 || pDumpDialogData->m_uiDumpType == 3)
	{
		for (auto pIMGFile : veCIMGFormats)
		{
			pIMGFile->unload();
			delete pIMGFile;
		}
	}

	delete pDumpDialogData;
}

string			CDumpManager::getEncoderClassIdFromImageExtension(string strFileExtension)
{
	strFileExtension = CStringUtility::toUpperCase(strFileExtension);
	if (strFileExtension == "GIF")
	{
		return "image/gif";
	}
	else if (strFileExtension == "JPG")
	{
		return "image/jpeg";
	}
	else if (strFileExtension == "PNG")
	{
		return "image/png";
	}
	else if (strFileExtension == "TIFF")
	{
		return "image/tiff";
	}
	else
	{
		return "";
	}
}