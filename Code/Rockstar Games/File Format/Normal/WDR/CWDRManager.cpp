#include "CWDRManager.h"
#include "CWDRFormat.h"
#include "Model/CIntermediateModelFormat.h"
#include "Model/CIntermediateModel.h"
#include "Model/Data/CIntermediateGeometry.h"
#include "Model/Data/CIntermediateBoundingObject.h"
#include "CWDRBlock_Model.h"
#include "CWDRBlock_Geometry.h"
#include "Math/CMathUtility.h"

using namespace std;

CWDRManager::CWDRManager(void)
{
}

void						CWDRManager::init(void)
{
}
void						CWDRManager::uninit(void)
{
}

CWDRFormat*					CWDRManager::convertIntermediateModelFileToWDRFile(CIntermediateModelFormat *pGeneralModelFile)
{
	CWDRFormat *pWDRFile = new CWDRFormat;

	for (CIntermediateModel *pGeneralModel : pGeneralModelFile->getEntries())
	{
		CWDRBlock_Model *pWDRBlock_Model = new CWDRBlock_Model;
		for (CIntermediateGeometry *pGeneralGeometry : pGeneralModel->getEntries())
		{
			CWDRBlock_Geometry *pWDRBlock_Geometry = CWDRFormat::convertIntermediateGeometryToWDRGeometry(pGeneralGeometry);
			pWDRBlock_Model->m_vecGeometries.push_back(pWDRBlock_Geometry);
		}
		pWDRFile->m_vecModels.push_back(pWDRBlock_Model);
	}

	if (pGeneralModelFile->getEntryCount() > 0 && pGeneralModelFile->getEntryByIndex(0)->getEntryCount() > 0)
	{
		// bounding object
		CIntermediateBoundingObject *pBoundingObject = pGeneralModelFile->getEntryByIndex(0)->getEntryByIndex(0)->getBoundingObject();
		if (pBoundingObject->getBoundingObjectType() == BOUNDING_OBJECT_TYPE_SPHERE)
		{
			CVector3D vecMin = CMathUtility::getBoundingCuboidMinFromSphere(pBoundingObject->getCenter(), pBoundingObject->getRadius());
			CVector3D vecMax = CMathUtility::getBoundingCuboidMinFromSphere(pBoundingObject->getCenter(), pBoundingObject->getRadius());
			pWDRFile->m_boundingObject.m_fMin[0] = vecMin.m_x;
			pWDRFile->m_boundingObject.m_fMin[1] = vecMin.m_y;
			pWDRFile->m_boundingObject.m_fMin[2] = vecMin.m_z;
			pWDRFile->m_boundingObject.m_fMin[3] = 1.0f;
			pWDRFile->m_boundingObject.m_fMax[0] = vecMax.m_x;
			pWDRFile->m_boundingObject.m_fMax[1] = vecMax.m_y;
			pWDRFile->m_boundingObject.m_fMax[2] = vecMax.m_z;
			pWDRFile->m_boundingObject.m_fMax[3] = 1.0f;
			// todo - this takes 4 floats so is probably in a different format
		}
		else if (pBoundingObject->getBoundingObjectType() == BOUNDING_OBJECT_TYPE_CUBOID)
		{
			pWDRFile->m_boundingObject.m_fMin[0] = pBoundingObject->getMin().m_x;
			pWDRFile->m_boundingObject.m_fMin[1] = pBoundingObject->getMin().m_y;
			pWDRFile->m_boundingObject.m_fMin[2] = pBoundingObject->getMin().m_z;
			pWDRFile->m_boundingObject.m_fMin[3] = pBoundingObject->getMin().m_w;
			pWDRFile->m_boundingObject.m_fMax[0] = pBoundingObject->getMax().m_x;
			pWDRFile->m_boundingObject.m_fMax[1] = pBoundingObject->getMax().m_y;
			pWDRFile->m_boundingObject.m_fMax[2] = pBoundingObject->getMax().m_z;
			pWDRFile->m_boundingObject.m_fMax[3] = pBoundingObject->getMax().m_w;
		}
	}

	return pWDRFile;
}

uint32						CWDRManager::getPackedOffset(uint32 uiOffset)
{
	return uiOffset | (5 << 28);
}

uint32						CWDRManager::getPackedDataOffset(uint32 uiOffset)
{
	return uiOffset | (6 << 28);
}