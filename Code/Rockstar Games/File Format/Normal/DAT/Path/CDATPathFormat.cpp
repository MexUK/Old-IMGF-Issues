#include "CDATPathFormat.h"
#include "Data Stream/CDataWriter.h"
#include "String/CStringUtility.h"
#include "Data Stream/CDataReader.h"
#include "CVector3D.h"

using namespace std;

CDATPathFormat::CDATPathFormat(void) :
	CFormat(true)
{
	m_header.m_ePathsFormat = DAT_PATH_UNKNOWN;
	m_header.m_uiPathNodeCount = 0;
	m_header.m_uiNaviNodeCount = 0;
	m_header.m_uiPedNodeCount = 0;
	m_header.m_uiVehicleNodeCount = 0;
	m_header.m_uiLinkCount = 0;
}

void							CDATPathFormat::unload(void)
{
}

void							CDATPathFormat::unserialize(void)
{
	setPathsFormat(detectPathsFormat());
	switch (getPathsFormat())
	{
	case DAT_PATH_DEFAULT:		return unserializeDefault();
	case DAT_PATH_FASTMAN92:	return unserializeFastman92();
	}
}

void							CDATPathFormat::serialize(void)
{
	switch (getPathsFormat())
	{
	case DAT_PATH_DEFAULT:		return serializeDefault();
	case DAT_PATH_FASTMAN92:	return serializeFastman92();
	}
}

void				CDATPathFormat::unserializeDefault(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	m_uiFileIndex = 0;// uiAreaId;
	uint32 uiSeek = 0;
	uint32 uiSize;

	CDATEntry_Paths_Default_Header *pHeader1 = pDataReader->readStruct<CDATEntry_Paths_Default_Header>();
	m_header.m_ePathsFormat = DAT_PATH_DEFAULT;
	m_header.m_uiPathNodeCount = pHeader1->m_uiPathNodeCount;
	m_header.m_uiVehicleNodeCount = pHeader1->m_uiVehicleNodeCount;
	m_header.m_uiPedNodeCount = pHeader1->m_uiPedNodeCount;
	m_header.m_uiNaviNodeCount = pHeader1->m_uiNaviNodeCount;
	m_header.m_uiLinkCount = pHeader1->m_uiLinkCount;
	uiSize = 20;
	uiSeek += uiSize;

	m_vecPathNodes.resize(m_header.m_uiPathNodeCount);
	m_vecNaviNodes.resize(m_header.m_uiNaviNodeCount);
	m_vecLinks.resize(m_header.m_uiLinkCount);

	CDATEntry_Paths_Default_PathNode *pPathNodes = new CDATEntry_Paths_Default_PathNode[m_header.m_uiPathNodeCount];
	CDATEntry_Paths_Default_NaviNode *pNaviNodes = new CDATEntry_Paths_Default_NaviNode[m_header.m_uiNaviNodeCount];
	CDATEntry_Paths_Default_Link *pLinks = new CDATEntry_Paths_Default_Link[m_header.m_uiLinkCount];
	CDATEntry_Paths_Default_NaviLink *pNaviLinks = new CDATEntry_Paths_Default_NaviLink[m_header.m_uiLinkCount];
	CDATEntry_Paths_Default_LinkLength *pLinkLengths = new CDATEntry_Paths_Default_LinkLength[m_header.m_uiLinkCount];

	uiSize = m_header.m_uiPathNodeCount * 28;
	memcpy(pPathNodes, pDataReader->readCString(uiSize), uiSize);
	uiSeek += uiSize;

	uiSize = m_header.m_uiNaviNodeCount * 14;
	memcpy(pNaviNodes, pDataReader->readCString(uiSize), uiSize);
	uiSeek += uiSize;

	uiSize = m_header.m_uiLinkCount * 4;
	memcpy(pLinks, pDataReader->readCString(uiSize), uiSize);
	uiSeek += uiSize;
	uiSeek += 768;

	uiSize = m_header.m_uiLinkCount * 2;
	memcpy(pNaviLinks, pDataReader->readCString(uiSize), uiSize);
	uiSeek += uiSize;

	uiSize = m_header.m_uiLinkCount * 1;
	memcpy(pLinkLengths, pDataReader->readCString(uiSize), uiSize);
	uiSeek += uiSize;

	for (uint32 i = 0, j = m_header.m_uiPathNodeCount; i < j; i++)
	{
		CDATEntry_Paths_Default_PathNode &pPathNode_Default = *(pPathNodes + i);
		CDATEntry_Paths_General_PathNode &pPathNode_General = m_vecPathNodes[i];
		pPathNode_General.m_vecPosition.x = pPathNode_Default.m_iPosition[0];
		pPathNode_General.m_vecPosition.y = pPathNode_Default.m_iPosition[1];
		pPathNode_General.m_vecPosition.z = pPathNode_Default.m_iPosition[2];
		pPathNode_General.m_wLinkId = pPathNode_Default.m_usLinkId;
		pPathNode_General.m_wNodeId = pPathNode_Default.m_usNodeId;
		pPathNode_General.m_wAreaId = pPathNode_Default.m_usAreaId;
		pPathNode_General.m_ucPathWidth = pPathNode_Default.m_ucPathWidth;
		pPathNode_General.m_ucNodeType = pPathNode_Default.m_ucNodeType;
		pPathNode_General.m_uiFlags = pPathNode_Default.m_uiFlags;
	}
	for (uint32 i = 0, j = m_header.m_uiNaviNodeCount; i < j; i++)
	{
		CDATEntry_Paths_Default_NaviNode &pNaviNode_Default = *(pNaviNodes + i);
		CDATEntry_Paths_General_NaviNode &pNaviNode_General = m_vecNaviNodes[i];
		pNaviNode_General.m_iPosition[0] = pNaviNode_Default.m_iPosition[0];
		pNaviNode_General.m_iPosition[1] = pNaviNode_Default.m_iPosition[1];
		pNaviNode_General.m_ucDirection[0] = pNaviNode_Default.m_ucDirection[0];
		pNaviNode_General.m_ucDirection[1] = pNaviNode_Default.m_ucDirection[1];
		pNaviNode_General.m_uiFlags = pNaviNode_Default.m_uiFlags;
		pNaviNode_General.m_usTargetNode_NodeId = pNaviNode_Default.m_usNodeId;
		pNaviNode_General.m_usTargetNode_AreaId = pNaviNode_Default.m_usAreaId;
	}
	for (uint32 i = 0, j = m_header.m_uiLinkCount; i < j; i++)
	{
		CDATEntry_Paths_Default_Link			&pLink_Default = *(pLinks + i);
		CDATEntry_Paths_Default_NaviLink		&pNaviLink_Default = *(pNaviLinks + i);
		CDATEntry_Paths_Default_LinkLength		&pLinkLength_Default = *(pLinkLengths + i);
		CDATEntry_Paths_General_Link			&pLink_General = m_vecLinks[i];

		pLink_General.m_usAreaId = pLink_Default.m_usAreaId;
		pLink_General.m_usNodeId = pLink_Default.m_usNodeId;
		pLink_General.m_usNaviNodeId = pNaviLink_Default.m_usData & 0x3FF; // lower 10 bits
		pLink_General.m_usNaviAreaId = (pNaviLink_Default.m_usData >> 10) & 0x3F; // upper 6 bits
		pLink_General.m_ucLength = pLinkLength_Default.m_ucLength;
	}

	delete[] pHeader1;

	delete[] pPathNodes;
	delete[] pNaviNodes;
	delete[] pLinks;
	delete[] pNaviLinks;
	delete[] pLinkLengths;
}

void				CDATPathFormat::unserializeFastman92(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	m_uiFileIndex = 0; // uiAreaId;
	//uint32 uiSeek = 0;
	//uint32 uiSize;

	m_header.m_ePathsFormat = DAT_PATH_FASTMAN92;
	uint32 uiFourCC = pDataReader->readUint32(); // 0xFFFFFFFF
	string strFormat = pDataReader->readString(4); // FM92
	uint8 ucNicknameLength = pDataReader->readUint8();
	string strNickname = pDataReader->readString(ucNicknameLength);
	uint32 uiFormatVersion = pDataReader->readUint32();
	m_header.m_uiPathNodeCount = pDataReader->readUint32();
	m_header.m_uiVehicleNodeCount = pDataReader->readUint32();
	m_header.m_uiPedNodeCount = pDataReader->readUint32();
	m_header.m_uiNaviNodeCount = pDataReader->readUint32();
	m_header.m_uiLinkCount = pDataReader->readUint32();
	//uiSize = pDataReader->getByteCountRead();
	//uiSeek += uiSize;

	m_vecPathNodes.resize(m_header.m_uiPathNodeCount);
	m_vecNaviNodes.resize(m_header.m_uiNaviNodeCount);
	m_vecLinks.resize(m_header.m_uiLinkCount);

	CDATEntry_Paths_Fastman92_PathNode_extended *pPathNodes = new CDATEntry_Paths_Fastman92_PathNode_extended[m_header.m_uiPathNodeCount];
	CDATEntry_Paths_Fastman92_NaviNode_extended *pNaviNodes = new CDATEntry_Paths_Fastman92_NaviNode_extended[m_header.m_uiNaviNodeCount];
	CDATEntry_Paths_Fastman92_Link *pLinks = new CDATEntry_Paths_Fastman92_Link[m_header.m_uiLinkCount];
	CDATEntry_Paths_Fastman92_NaviLink *pNaviLinks = new CDATEntry_Paths_Fastman92_NaviLink[m_header.m_uiLinkCount];
	CDATEntry_Paths_Fastman92_LinkLength *pLinkLengths = new CDATEntry_Paths_Fastman92_LinkLength[m_header.m_uiLinkCount];
	CDATEntry_Paths_Fastman92_PathIntersectionFlags *pPathIntersectionFlags = new CDATEntry_Paths_Fastman92_PathIntersectionFlags[m_header.m_uiLinkCount];

	/*
	uiSize = pDATFile_Paths->m_header.m_uiPathNodeCount * 40;
	memcpy(pPathNodes, strFileData.c_str() + uiSeek, uiSize);
	uiSeek += uiSize;

	uiSize = pDATFile_Paths->m_header.m_uiNaviNodeCount * 22;
	memcpy(pNaviNodes, strFileData.c_str() + uiSeek, uiSize);
	uiSeek += uiSize;

	uiSize = pDATFile_Paths->m_header.m_uiLinkCount * 4;
	memcpy(pLinks, strFileData.c_str() + uiSeek, uiSize);
	uiSeek += uiSize;
	uiSeek += 768;

	uiSize = pDATFile_Paths->m_header.m_uiLinkCount * 4;
	memcpy(pNaviLinks, strFileData.c_str() + uiSeek, uiSize);
	uiSeek += uiSize;

	uiSize = pDATFile_Paths->m_header.m_uiLinkCount * 1;
	memcpy(pLinkLengths, strFileData.c_str() + uiSeek, uiSize);
	uiSeek += uiSize;
	*/

	for (uint32 i = 0, j = m_header.m_uiPathNodeCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_PathNode_extended *pPathNode_Fastman92 = pPathNodes + i;
		pPathNode_Fastman92->m_pPrev = 0;
		pDataReader->readUint32();
		pPathNode_Fastman92->m_ppNext = 0;
		pDataReader->readUint32();
		pPathNode_Fastman92->m_posn.x = pDataReader->readUint16();
		pPathNode_Fastman92->m_posn.y = pDataReader->readUint16();
		pPathNode_Fastman92->m_posn.z = pDataReader->readUint16();
		pPathNode_Fastman92->m_wSearchList = pDataReader->readUint16();
		pPathNode_Fastman92->m_wConnectedNodesStartId = pDataReader->readUint16();
		pPathNode_Fastman92->m_wAreaId = pDataReader->readUint16();
		pPathNode_Fastman92->m_wNodeId = pDataReader->readUint16();
		pPathNode_Fastman92->m_nPathWidth = pDataReader->readUint8();
		pPathNode_Fastman92->m_nNodeType = pDataReader->readUint8();
		pPathNode_Fastman92->m_dwFlags = pDataReader->readUint32();
		pPathNode_Fastman92->m_extended_posn.x = pDataReader->readUint32();
		pPathNode_Fastman92->m_extended_posn.y = pDataReader->readUint32();
		pPathNode_Fastman92->m_extended_posn.z = pDataReader->readUint32();
	}

	for (uint32 i = 0, j = m_header.m_uiNaviNodeCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_NaviNode_extended *pNaviNode_Fastman92 = pNaviNodes + i;
		pNaviNode_Fastman92->posX = pDataReader->readUint16();
		pNaviNode_Fastman92->posY = pDataReader->readUint16();
		pNaviNode_Fastman92->info.areaId = pDataReader->readUint16();
		pNaviNode_Fastman92->info.nodeId = pDataReader->readUint16();
		pNaviNode_Fastman92->dirX = pDataReader->readUint8();
		pNaviNode_Fastman92->dirY = pDataReader->readUint8();
		pNaviNode_Fastman92->m_dwFlags = pDataReader->readUint32();
		pNaviNode_Fastman92->extended_posX = pDataReader->readUint32();
		pNaviNode_Fastman92->extended_posY = pDataReader->readUint32();
	}

	for (uint32 i = 0, j = m_header.m_uiLinkCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_Link *pLink_Fastman92 = pLinks + i;
		pLink_Fastman92->m_usAreaId = pDataReader->readUint16();
		pLink_Fastman92->m_usNodeId = pDataReader->readUint16();
	}

	if (m_header.m_uiLinkCount > 0)
	{
		string strFiller1 = pDataReader->readString(768);
	}

	for (uint32 i = 0, j = m_header.m_uiLinkCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_NaviLink *pNaviLink_Fastman92 = pNaviLinks + i;
		pNaviLink_Fastman92->m_uiData = pDataReader->readUint32();
	}

	for (uint32 i = 0, j = m_header.m_uiLinkCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_LinkLength *pLinkLength_Fastman92 = pLinkLengths + i;
		pLinkLength_Fastman92->m_ucLength = pDataReader->readUint8();
	}

	for (uint32 i = 0, j = m_header.m_uiLinkCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_PathIntersectionFlags *pPathIntersectionFlags_Fastman92 = pPathIntersectionFlags + i;
		uint8 ucByte = pDataReader->readUint8();
		//pPathIntersectionFlags_Fastman92->m_bRoadCross = ucByte & 1;
		//pPathIntersectionFlags_Fastman92->m_bPedTrafficLight = ucByte & 2;
		pPathIntersectionFlags_Fastman92->m_ucData = ucByte;
	}

	if (m_header.m_uiLinkCount > 0)
	{
		string strFiller2 = pDataReader->readString(192);
	}

	if (m_header.m_uiLinkCount > 0)
	{
		string strFiller3 = pDataReader->readString(192);
	}

	string strEOF = pDataReader->readString(4);
	//
	//if (strEOF.substr(0, 3) != "EOF")
	//{
	//CDebugger::log("PARSER FAILED on index " + CStringUtility::toString(pDATFile_Paths->m_uiFileIndex) + ", bytes read: " + CStringUtility::toString(pDataReader->getByteCountRead()));
	//}
	//

	// --

	for (uint32 i = 0, j = m_header.m_uiPathNodeCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_PathNode_extended &pPathNode_Fastman92 = *(pPathNodes + i);
		CDATEntry_Paths_General_PathNode &pPathNode_General = m_vecPathNodes[i];
		pPathNode_General.m_bIsVehiclePathNode = i < m_header.m_uiVehicleNodeCount;
		pPathNode_General.m_pPrev = 0;
		pPathNode_General.m_ppNext = 0;
		pPathNode_General.m_vecPosition.x = pPathNode_Fastman92.m_extended_posn.x;
		pPathNode_General.m_vecPosition.y = pPathNode_Fastman92.m_extended_posn.y;
		pPathNode_General.m_vecPosition.z = pPathNode_Fastman92.m_extended_posn.z;
		pPathNode_General.m_wSearchList = pPathNode_Fastman92.m_wSearchList;
		pPathNode_General.m_wConnectedNodesStartId = pPathNode_Fastman92.m_wConnectedNodesStartId;
		pPathNode_General.m_wAreaId = pPathNode_Fastman92.m_wAreaId;
		pPathNode_General.m_wNodeId = pPathNode_Fastman92.m_wNodeId;
		pPathNode_General.m_ucPathWidth = pPathNode_Fastman92.m_nPathWidth;
		pPathNode_General.m_ucNodeType = pPathNode_Fastman92.m_nNodeType;
		pPathNode_General.m_uiFlags = pPathNode_Fastman92.m_dwFlags;
	}
	for (uint32 i = 0, j = m_header.m_uiNaviNodeCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_NaviNode_extended &pNaviNode_Fastman92 = *(pNaviNodes + i);
		CDATEntry_Paths_General_NaviNode &pNaviNode_General = m_vecNaviNodes[i];
		pNaviNode_General.m_iPosition[0] = pNaviNode_Fastman92.extended_posX;
		pNaviNode_General.m_iPosition[1] = pNaviNode_Fastman92.extended_posY;
		pNaviNode_General.m_usTargetNode_AreaId = pNaviNode_Fastman92.info.areaId;
		pNaviNode_General.m_usTargetNode_NodeId = pNaviNode_Fastman92.info.nodeId;
		pNaviNode_General.m_ucDirection[0] = pNaviNode_Fastman92.dirX;
		pNaviNode_General.m_ucDirection[1] = pNaviNode_Fastman92.dirY;
		pNaviNode_General.m_uiFlags = pNaviNode_Fastman92.m_dwFlags;
	}
	for (uint32 i = 0, j = m_header.m_uiLinkCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_Link			&pLink_Fastman92 = *(pLinks + i);
		CDATEntry_Paths_Fastman92_NaviLink		&pNaviLink_Fastman92 = *(pNaviLinks + i);
		CDATEntry_Paths_Fastman92_LinkLength	&pLinkLength_Fastman92 = *(pLinkLengths + i);
		CDATEntry_Paths_Fastman92_PathIntersectionFlags		&pPathIntersectionFlags_Fastman92 = *(pPathIntersectionFlags + i);
		CDATEntry_Paths_General_Link			&pLink_General = m_vecLinks[i];

		pLink_General.m_usAreaId = pLink_Fastman92.m_usAreaId;
		pLink_General.m_usNodeId = pLink_Fastman92.m_usNodeId;
		pLink_General.m_usNaviNodeId = pNaviLink_Fastman92.m_uiData & 0xFFFF; // lower 16 bits
		pLink_General.m_usNaviAreaId = (pNaviLink_Fastman92.m_uiData >> 16) & 0xFFFF; // upper 16 bits
		pLink_General.m_ucLength = pLinkLength_Fastman92.m_ucLength;

		uint8 ucByte = 0;
		//ucByte |= pPathIntersectionFlags_Fastman92.m_bRoadCross ? 1 : 0;
		//ucByte |= pPathIntersectionFlags_Fastman92.m_bPedTrafficLight ? 2 : 0;
		pLink_General.m_ucPathIntersectionFlags = pPathIntersectionFlags_Fastman92.m_ucData;
	}

	/*
	// debug
	for (auto& pathNode : m_vecPathNodes)
	{
		CDebugger::log(
			"[path node]"
			+ string(" position: ") + CStringUtility::toString((float32)((float32)pathNode.m_vecPosition.x / 8.0)) + " "
			+ CStringUtility::toString((float32)((float32)pathNode.m_vecPosition.y / 8.0)) + " "
			+ CStringUtility::toString((float32)((float32)pathNode.m_vecPosition.z / 8.0)) + " "
			+ " search list: " + CStringUtility::toString(pathNode.m_wSearchList)
			+ " first link id: " + CStringUtility::toString(pathNode.m_wConnectedNodesStartId)
			+ " area id: " + CStringUtility::toString(pathNode.m_wAreaId) + " node id: " + CStringUtility::toString(pathNode.m_wNodeId)
			+ " path width: " + CStringUtility::toString(pathNode.m_ucPathWidth)
			+ " node type: " + CStringUtility::toString(pathNode.m_ucNodeType)
			+ " flags: " + CStringUtility::toString(pathNode.m_uiFlags)
			);
	}
	for (auto& naviNode : m_vecNaviNodes)
	{
		CDebugger::log(
			"[navi node]"
			+ string(" position: ") + CStringUtility::toString((float32)((float32)naviNode.m_iPosition[0] / 8.0)) + " "
			+ CStringUtility::toString((float32)((float32)naviNode.m_iPosition[1] / 8.0)) + " "
			+ " area id: " + CStringUtility::toString(naviNode.m_usTargetNode_AreaId) + " node id: " + CStringUtility::toString(naviNode.m_usTargetNode_NodeId)
			+ " direction x: " + CStringUtility::toString(naviNode.m_ucDirection[0]) + " direction y " + CStringUtility::toString(naviNode.m_ucDirection[1])
			+ " flags: " + CStringUtility::toString(naviNode.m_uiFlags)
			);
	}
	*/

	// clean up
	delete[] pPathNodes;
	delete[] pNaviNodes;
	delete[] pLinks;
	delete[] pNaviLinks;
	delete[] pLinkLengths;
	delete[] pPathIntersectionFlags;
}

void							CDATPathFormat::serializeDefault(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	//uint32 uiSeek = 0;
	uint32 uiSize;

	CDATEntry_Paths_Default_Header header;
	header.m_uiPathNodeCount = m_header.m_uiPathNodeCount;
	header.m_uiVehicleNodeCount = m_header.m_uiVehicleNodeCount;
	header.m_uiPedNodeCount = m_header.m_uiPedNodeCount;
	header.m_uiNaviNodeCount = m_header.m_uiNaviNodeCount;
	header.m_uiLinkCount = m_header.m_uiLinkCount;
	pDataWriter->writeCString((char*)&header, 20);
	//uiSize = 20;
	//uiSeek += uiSize;

	CDATEntry_Paths_Default_PathNode *pPathNodes = new CDATEntry_Paths_Default_PathNode[m_header.m_uiPathNodeCount];
	CDATEntry_Paths_Default_NaviNode *pNaviNodes = new CDATEntry_Paths_Default_NaviNode[m_header.m_uiNaviNodeCount];
	CDATEntry_Paths_Default_Link *pLinks = new CDATEntry_Paths_Default_Link[m_header.m_uiLinkCount];
	CDATEntry_Paths_Default_NaviLink *pNaviLinks = new CDATEntry_Paths_Default_NaviLink[m_header.m_uiLinkCount];
	CDATEntry_Paths_Default_LinkLength *pLinkLengths = new CDATEntry_Paths_Default_LinkLength[m_header.m_uiLinkCount];

	for (uint32 i = 0, j = m_header.m_uiPathNodeCount; i < j; i++)
	{
		CDATEntry_Paths_Default_PathNode &pPathNode_Default = *(pPathNodes + i);
		CDATEntry_Paths_General_PathNode &pPathNode_General = m_vecPathNodes[i];
		pPathNode_Default.m_iPosition[0] = pPathNode_General.m_vecPosition.x;
		pPathNode_Default.m_iPosition[1] = pPathNode_General.m_vecPosition.y;
		pPathNode_Default.m_iPosition[2] = pPathNode_General.m_vecPosition.z;
		pPathNode_Default.m_usLinkId = pPathNode_General.m_wLinkId;
		pPathNode_Default.m_usNodeId = pPathNode_General.m_wNodeId;
		pPathNode_Default.m_usAreaId = pPathNode_General.m_wAreaId;
		pPathNode_Default.m_ucPathWidth = pPathNode_General.m_ucPathWidth;
		pPathNode_Default.m_ucNodeType = pPathNode_General.m_ucNodeType;
		pPathNode_Default.m_uiFlags = pPathNode_General.m_uiFlags;
	}
	for (uint32 i = 0, j = m_header.m_uiNaviNodeCount; i < j; i++)
	{
		CDATEntry_Paths_Default_NaviNode &pNaviNode_Default = *(pNaviNodes + i);
		CDATEntry_Paths_General_NaviNode &pNaviNode_General = m_vecNaviNodes[i];
		pNaviNode_Default.m_iPosition[0] = pNaviNode_General.m_iPosition[0];
		pNaviNode_Default.m_iPosition[1] = pNaviNode_General.m_iPosition[1];
		pNaviNode_Default.m_ucDirection[0] = pNaviNode_General.m_ucDirection[0];
		pNaviNode_Default.m_ucDirection[1] = pNaviNode_General.m_ucDirection[1];
		pNaviNode_Default.m_uiFlags = pNaviNode_General.m_uiFlags;
		pNaviNode_Default.m_usNodeId = pNaviNode_General.m_usTargetNode_NodeId;
		pNaviNode_Default.m_usAreaId = pNaviNode_General.m_usTargetNode_AreaId;
	}
	for (uint32 i = 0, j = m_header.m_uiLinkCount; i < j; i++)
	{
		CDATEntry_Paths_Default_Link			&pLink_Default = *(pLinks + i);
		CDATEntry_Paths_Default_NaviLink		&pNaviLink_Default = *(pNaviLinks + i);
		CDATEntry_Paths_Default_LinkLength		&pLinkLength_Default = *(pLinkLengths + i);
		CDATEntry_Paths_General_Link			&pLink_General = m_vecLinks[i];

		pLink_Default.m_usAreaId = pLink_General.m_usAreaId;
		pLink_Default.m_usNodeId = pLink_General.m_usNodeId;
		pNaviLink_Default.m_usData = (pLink_General.m_usNaviNodeId & 0x3FF) | ((pLink_General.m_usNaviAreaId & 0x3F) << 10); // lower 10 bits and upper 6 bits
		pLink_General.m_ucLength = pLinkLength_Default.m_ucLength;
	}

	uint32 uiSize2 = m_header.m_uiPathNodeCount * 28
		+ m_header.m_uiNaviNodeCount * 14
		+ m_header.m_uiLinkCount * 4
		+ 768
		+ m_header.m_uiLinkCount * 2
		+ m_header.m_uiLinkCount * 1;

	char *pOutputData = new char[uiSize2];
	char *pOutputDataStart = (pOutputData + 1) - 1;
	memset(pOutputData, '\0', uiSize2);

	uiSize = m_header.m_uiPathNodeCount * 28;
	memcpy(pOutputData, pPathNodes, uiSize);
	pOutputData += uiSize;

	uiSize = m_header.m_uiNaviNodeCount * 14;
	memcpy(pOutputData, pNaviNodes, uiSize);
	pOutputData += uiSize;

	uiSize = m_header.m_uiLinkCount * 4;
	memcpy(pOutputData, pLinks, uiSize);
	pOutputData += uiSize;
	pOutputData += 768;

	uiSize = m_header.m_uiLinkCount * 2;
	memcpy(pOutputData, pNaviLinks, uiSize);
	pOutputData += uiSize;

	uiSize = m_header.m_uiLinkCount * 1;
	memcpy(pOutputData, pLinkLengths, uiSize);
	pOutputData += uiSize;

	pDataWriter->writeString(pOutputDataStart, uiSize2);
	//uiSeek += uiSize2;
	delete[] pOutputDataStart;

	delete[] pPathNodes;
	delete[] pNaviNodes;
	delete[] pLinks;
	delete[] pNaviLinks;
	delete[] pLinkLengths;
}

void							CDATPathFormat::serializeFastman92(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->writeUint32(0xFFFFFFFF);
	pDataWriter->writeString("FM92");
	pDataWriter->writeUint8(11);
	string strNickname = "";
	strNickname.append(1, 0);
	strNickname.append("fastman92", 9);
	strNickname.append(1, 0);
	pDataWriter->writeString(strNickname);
	pDataWriter->writeString("VER2");
	pDataWriter->writeUint32(m_header.m_uiPathNodeCount);
	pDataWriter->writeUint32(m_header.m_uiVehicleNodeCount);
	pDataWriter->writeUint32(m_header.m_uiPedNodeCount);
	pDataWriter->writeUint32(m_header.m_uiNaviNodeCount);
	pDataWriter->writeUint32(m_header.m_uiLinkCount);

	CDATEntry_Paths_Fastman92_PathNode_extended *pPathNodes = new CDATEntry_Paths_Fastman92_PathNode_extended[m_header.m_uiPathNodeCount];
	CDATEntry_Paths_Fastman92_NaviNode_extended *pNaviNodes = new CDATEntry_Paths_Fastman92_NaviNode_extended[m_header.m_uiNaviNodeCount];
	CDATEntry_Paths_Fastman92_Link *pLinks = new CDATEntry_Paths_Fastman92_Link[m_header.m_uiLinkCount];
	CDATEntry_Paths_Fastman92_NaviLink *pNaviLinks = new CDATEntry_Paths_Fastman92_NaviLink[m_header.m_uiLinkCount];
	CDATEntry_Paths_Fastman92_LinkLength *pLinkLengths = new CDATEntry_Paths_Fastman92_LinkLength[m_header.m_uiLinkCount];
	CDATEntry_Paths_Fastman92_PathIntersectionFlags *pPathIntersectionFlags = new CDATEntry_Paths_Fastman92_PathIntersectionFlags[m_header.m_uiLinkCount];

	for (uint32 i = 0, j = m_header.m_uiPathNodeCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_PathNode_extended &pPathNode_Fastman92 = *(pPathNodes + i);
		CDATEntry_Paths_General_PathNode &pPathNode_General = m_vecPathNodes[i];
		pPathNode_Fastman92.m_pPrev = 0;
		pPathNode_Fastman92.m_ppNext = 0;
		pPathNode_Fastman92.m_posn.x = 0.0f;
		pPathNode_Fastman92.m_posn.y = 0.0f;
		pPathNode_Fastman92.m_posn.z = 0.0f;
		pPathNode_Fastman92.m_wSearchList = pPathNode_General.m_wSearchList;
		pPathNode_Fastman92.m_wConnectedNodesStartId = pPathNode_General.m_wConnectedNodesStartId;
		pPathNode_Fastman92.m_wNodeId = pPathNode_General.m_wNodeId;
		pPathNode_Fastman92.m_wAreaId = pPathNode_General.m_wAreaId;
		pPathNode_Fastman92.m_nPathWidth = pPathNode_General.m_ucPathWidth;
		pPathNode_Fastman92.m_nNodeType = pPathNode_General.m_ucNodeType;
		pPathNode_Fastman92.m_dwFlags = pPathNode_General.m_uiFlags;
		pPathNode_Fastman92.m_extended_posn.x = pPathNode_General.m_vecPosition.x;
		pPathNode_Fastman92.m_extended_posn.y = pPathNode_General.m_vecPosition.y;
		pPathNode_Fastman92.m_extended_posn.z = pPathNode_General.m_vecPosition.z;
	}
	for (uint32 i = 0, j = m_header.m_uiNaviNodeCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_NaviNode_extended &pNaviNode_Fastman92 = *(pNaviNodes + i);
		CDATEntry_Paths_General_NaviNode &pNaviNode_General = m_vecNaviNodes[i];
		pNaviNode_Fastman92.posX = 0.0f;
		pNaviNode_Fastman92.posY = 0.0f;
		pNaviNode_Fastman92.info.nodeId = pNaviNode_General.m_usTargetNode_NodeId;
		pNaviNode_Fastman92.info.areaId = pNaviNode_General.m_usTargetNode_AreaId;
		pNaviNode_Fastman92.dirX = pNaviNode_General.m_ucDirection[0];
		pNaviNode_Fastman92.dirY = pNaviNode_General.m_ucDirection[1];
		pNaviNode_Fastman92.m_dwFlags = pNaviNode_General.m_uiFlags;
		pNaviNode_Fastman92.extended_posX = pNaviNode_General.m_iPosition[0];
		pNaviNode_Fastman92.extended_posY = pNaviNode_General.m_iPosition[1];
	}
	for (uint32 i = 0, j = m_header.m_uiLinkCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_Link			&pLink_Fastman92 = *(pLinks + i);
		CDATEntry_Paths_Fastman92_NaviLink		&pNaviLink_Fastman92 = *(pNaviLinks + i);
		CDATEntry_Paths_Fastman92_LinkLength	&pLinkLength_Fastman92 = *(pLinkLengths + i);
		CDATEntry_Paths_Fastman92_PathIntersectionFlags		&pPathIntersectionFlags_Fastman92 = *(pPathIntersectionFlags + i);
		CDATEntry_Paths_General_Link			&pLink_General = m_vecLinks[i];

		pLink_Fastman92.m_usAreaId = pLink_General.m_usAreaId;
		pLink_Fastman92.m_usNodeId = pLink_General.m_usNodeId;
		pNaviLink_Fastman92.m_uiData = (pLink_General.m_usNaviNodeId & 0xFFFF) | ((pLink_General.m_usNaviAreaId & 0xFFFF) << 16); // lower 16 bits and upper 16 bits
		pLinkLength_Fastman92.m_ucLength = pLink_General.m_ucLength;
		
		pPathIntersectionFlags_Fastman92.m_ucData = pLink_General.m_ucPathIntersectionFlags;
		//pPathIntersectionFlags_Fastman92.m_bRoadCross = pLink_General.m_ucPathIntersectionFlags & 1;
		//pPathIntersectionFlags_Fastman92.m_bPedTrafficLight = pLink_General.m_ucPathIntersectionFlags & 2;
	}

	// --

	for (uint32 i = 0, j = m_header.m_uiPathNodeCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_PathNode_extended *pPathNode_Fastman92 = pPathNodes + i;
		pDataWriter->writeUint32(0); // pPathNode_Fastman92->m_pPrev = 0;
		pDataWriter->writeUint32(0); // pPathNode_Fastman92->m_ppNext = 0;
		pDataWriter->writeUint16(pPathNode_Fastman92->m_posn.x);
		pDataWriter->writeUint16(pPathNode_Fastman92->m_posn.y);
		pDataWriter->writeUint16(pPathNode_Fastman92->m_posn.z);
		pDataWriter->writeUint16(pPathNode_Fastman92->m_wSearchList);
		pDataWriter->writeUint16(pPathNode_Fastman92->m_wConnectedNodesStartId);
		pDataWriter->writeUint16(pPathNode_Fastman92->m_wAreaId);
		pDataWriter->writeUint16(pPathNode_Fastman92->m_wNodeId);
		pDataWriter->writeUint8(pPathNode_Fastman92->m_nPathWidth);
		pDataWriter->writeUint8(pPathNode_Fastman92->m_nNodeType);
		pDataWriter->writeUint32(pPathNode_Fastman92->m_dwFlags);
		pDataWriter->writeUint32(pPathNode_Fastman92->m_extended_posn.x);
		pDataWriter->writeUint32(pPathNode_Fastman92->m_extended_posn.y);
		pDataWriter->writeUint32(pPathNode_Fastman92->m_extended_posn.z);
	}

	for (uint32 i = 0, j = m_header.m_uiNaviNodeCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_NaviNode_extended *pNaviNode_Fastman92 = pNaviNodes + i;
		pDataWriter->writeUint16(pNaviNode_Fastman92->posX);
		pDataWriter->writeUint16(pNaviNode_Fastman92->posY);
		pDataWriter->writeUint16(pNaviNode_Fastman92->info.areaId);
		pDataWriter->writeUint16(pNaviNode_Fastman92->info.nodeId);
		pDataWriter->writeUint8(pNaviNode_Fastman92->dirX);
		pDataWriter->writeUint8(pNaviNode_Fastman92->dirY);
		pDataWriter->writeUint32(pNaviNode_Fastman92->m_dwFlags);
		pDataWriter->writeUint32(pNaviNode_Fastman92->extended_posX);
		pDataWriter->writeUint32(pNaviNode_Fastman92->extended_posY);
	}

	for (uint32 i = 0, j = m_header.m_uiLinkCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_Link *pLink_Fastman92 = pLinks + i;
		pDataWriter->writeUint16(pLink_Fastman92->m_usAreaId);
		pDataWriter->writeUint16(pLink_Fastman92->m_usNodeId);
	}

	if (m_header.m_uiLinkCount > 0)
	{
		pDataWriter->writeString("", 768);
	}

	for (uint32 i = 0, j = m_header.m_uiLinkCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_NaviLink *pNaviLink_Fastman92 = pNaviLinks + i;
		pDataWriter->writeUint32(pNaviLink_Fastman92->m_uiData);
	}

	for (uint32 i = 0, j = m_header.m_uiLinkCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_LinkLength *pLinkLength_Fastman92 = pLinkLengths + i;
		pDataWriter->writeUint8(pLinkLength_Fastman92->m_ucLength);
	}

	for (uint32 i = 0, j = m_header.m_uiLinkCount; i < j; i++)
	{
		CDATEntry_Paths_Fastman92_PathIntersectionFlags *pPathIntersectionFlags_Fastman92 = pPathIntersectionFlags + i;
		
		uint8 ucByte = pPathIntersectionFlags_Fastman92->m_ucData;
		//ucByte |= pPathIntersectionFlags_Fastman92->m_bRoadCross ? 1 : 0;
		//ucByte |= pPathIntersectionFlags_Fastman92->m_bPedTrafficLight ? 2 : 0;
		pDataWriter->writeUint8(ucByte);
	}

	if (m_header.m_uiLinkCount > 0)
	{
		pDataWriter->writeString("", 192);
	}

	if (m_header.m_uiLinkCount > 0)
	{
		pDataWriter->writeString("", 192);
	}

	string strEOF = "EOF";
	strEOF.append(1, 0);
	pDataWriter->writeString(strEOF);

	/*
	//////////////////////// commented
	uint32 uiSize2 = m_header.m_uiPathNodeCount * 40
		+ m_header.m_uiNaviNodeCount * 22
		+ m_header.m_uiLinkCount * 4
		+ 768
		+ m_header.m_uiLinkCount * 4
		+ m_header.m_uiLinkCount * 1;

	char *pOutputData = new char[uiSize2];
	char *pOutputDataStart = (pOutputData + 1) - 1;
	memset(pOutputData, '\0', uiSize2);

	uiSize = m_header.m_uiPathNodeCount * 40;
	memcpy(pOutputData, pPathNodes, uiSize);
	pOutputData += uiSize;

	uiSize = m_header.m_uiNaviNodeCount * 22;
	memcpy(pOutputData, pNaviNodes, uiSize);
	pOutputData += uiSize;

	uiSize = m_header.m_uiLinkCount * 4;
	memcpy(pOutputData, pLinks, uiSize);
	pOutputData += uiSize;
	pOutputData += 768;

	uiSize = m_header.m_uiLinkCount * 4;
	memcpy(pOutputData, pNaviLinks, uiSize);
	pOutputData += uiSize;

	uiSize = m_header.m_uiLinkCount * 1;
	memcpy(pOutputData, pLinkLengths, uiSize);
	pOutputData += uiSize;
	//////////////////////////// commented end
	*/

	//pDataWriter->writeString(string(pOutputDataStart, uiSize2));
	//delete[] pOutputDataStart;

	delete[] pPathNodes;
	delete[] pNaviNodes;
	delete[] pLinks;
	delete[] pNaviLinks;
	delete[] pLinkLengths;
	delete[] pPathIntersectionFlags;
}

eDATPathFormat			CDATPathFormat::detectPathsFormat(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	eDATPathFormat eDATPathFormatValue;

	uint32 uiPreviousSeek = pDataReader->getSeek();
	pDataReader->setPeek(true);

	if (CStringUtility::unpackUint32(pDataReader->readString(4), false) == 0xFFFFFFFF && pDataReader->readString(4) == "FM92")
	{
		eDATPathFormatValue = DAT_PATH_FASTMAN92;
	}
	else
	{
		eDATPathFormatValue = DAT_PATH_DEFAULT;
	}

	pDataReader->setSeek(uiPreviousSeek);
	pDataReader->setPeek(false);

	return eDATPathFormatValue;
}

void					CDATPathFormat::applyOffsetToPositions(CVector3D& vecPositionOffset)
{
	for (auto& pathNode : m_vecPathNodes)
	{
		float32 x = (float32)pathNode.m_vecPosition.x / 8.0f;
		float32 y = (float32)pathNode.m_vecPosition.y / 8.0f;
		float32 z = (float32)pathNode.m_vecPosition.z / 8.0f;

		x += vecPositionOffset.m_x;
		y += vecPositionOffset.m_y;
		z += vecPositionOffset.m_z;

		pathNode.m_vecPosition.x = (int32)(x * 8.0f);
		pathNode.m_vecPosition.y = (int32)(y * 8.0f);
		pathNode.m_vecPosition.z = (int32)(z * 8.0f);
	}
	for (auto& naviNode : m_vecNaviNodes)
	{
		float32 x = (float32)naviNode.m_iPosition[0] / 8.0f;
		float32 y = (float32)naviNode.m_iPosition[1] / 8.0f;

		x += vecPositionOffset.m_x;
		y += vecPositionOffset.m_y;

		naviNode.m_iPosition[0] = (int32)(x * 8.0f);
		naviNode.m_iPosition[1] = (int32)(y * 8.0f);
	}
}

void					CDATPathFormat::addPathNode(CDATEntry_Paths_General_PathNode pathNode)
{
	m_header.m_uiPathNodeCount++;
	if (pathNode.m_bIsVehiclePathNode)
	{
		m_header.m_uiVehicleNodeCount++;
	}
	else
	{
		m_header.m_uiPedNodeCount++;
	}
	CDATEntry_Paths_General_PathNode pathNode2 = pathNode;
	pathNode2.m_wAreaId = m_uiFileIndex;
	pathNode2.m_wNodeId = m_vecPathNodes.size();
	m_vecPathNodes.push_back(pathNode2);
}

void					CDATPathFormat::addNaviNode(CDATEntry_Paths_General_NaviNode naviNode)
{
	m_header.m_uiNaviNodeCount++;
	CDATEntry_Paths_General_NaviNode naviNode2 = naviNode;
	//naviNode2.m_usAreaId = m_uiFileIndex;
	//naviNode2.m_usNodeId = m_vecNaviNodes.size();
	m_vecNaviNodes.push_back(naviNode2);
}

void					CDATPathFormat::addLink(CDATEntry_Paths_General_Link link)
{
	m_header.m_uiLinkCount++;
	CDATEntry_Paths_General_Link link2 = link;
	//link2.m_usAreaId = m_uiFileIndex;
	//link2.m_usNaviAreaId = m_uiFileIndex;
	//link2.m_usNodeId = 0;
	//link2.m_usNaviNodeId = 0;
	m_vecLinks.push_back(link2);
}