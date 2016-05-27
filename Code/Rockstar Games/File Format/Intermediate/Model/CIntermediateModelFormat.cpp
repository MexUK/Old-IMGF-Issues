#include "CIntermediateModelFormat.h"
#include "CIntermediateModel.h"
#include "Data/CIntermediateGeometry.h"

void				CIntermediateModelFormat::unload(void)
{
	for (auto pModel : getEntries())
	{
		for (auto pGeometry : pModel->getEntries())
		{
			pGeometry->getVertices().clear();
			pGeometry->getTriangles().clear();
			delete pGeometry;
		}

		pModel->removeAllEntries();
		delete pModel;
	}

	removeAllEntries();
}