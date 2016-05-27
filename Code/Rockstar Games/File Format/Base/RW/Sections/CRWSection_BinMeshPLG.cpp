#include "CRWSection_BinMeshPLG.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CRWSection_BinMeshPLG::CRWSection_BinMeshPLG(void) :
	m_uiFlags(0),
	m_uiMeshCount(0),
	m_uiIndexCount(0)
{
	setSectionId(RW_SECTION_BIN_MESH_PLG);
}

void							CRWSection_BinMeshPLG::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	m_uiFlags = pDataReader->readUint32();
	m_uiMeshCount = pDataReader->readUint32();
	m_uiIndexCount = pDataReader->readUint32();

	for (uint32 i = 0; i < m_uiMeshCount; i++)
	{
		CRWEntry_BinMeshPLG_Mesh *pRWEntry_BinMeshPLG_Mesh = new CRWEntry_BinMeshPLG_Mesh;
		m_vecMeshes.push_back(pRWEntry_BinMeshPLG_Mesh);
		
		pRWEntry_BinMeshPLG_Mesh->setIndexCount(pDataReader->readUint32());
		pRWEntry_BinMeshPLG_Mesh->setMaterialIndex(pDataReader->readUint32());

		for (uint32 i2 = 0; i2 < pRWEntry_BinMeshPLG_Mesh->getIndexCount(); i2++)
		{
			pRWEntry_BinMeshPLG_Mesh->getVertexIndices().push_back(pDataReader->readUint32());
		}
	}
}

void							CRWSection_BinMeshPLG::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->write(m_uiFlags);
	pDataWriter->write(m_uiMeshCount);
	pDataWriter->write(m_uiIndexCount);

	for (CRWEntry_BinMeshPLG_Mesh *pRWEntry_BinMeshPLG_Mesh : m_vecMeshes)
	{
		pDataWriter->write(pRWEntry_BinMeshPLG_Mesh->getIndexCount());
		pDataWriter->write(pRWEntry_BinMeshPLG_Mesh->getMaterialIndex());

		for (auto uiVertexIndex : pRWEntry_BinMeshPLG_Mesh->getVertexIndices())
		{
			pDataWriter->write(uiVertexIndex);
		}
	}
}