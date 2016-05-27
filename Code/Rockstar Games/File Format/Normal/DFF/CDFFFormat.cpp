#include "CDFFFormat.h"
#include "RW/CTextureEntry.h"
#include "String/CStringUtility.h"
#include "Engine/RW/CRWVersion.h"
#include "RW/Sections/CRWSection_Geometry.h"
#include "File/CFileUtility.h"
#include "Model/CIntermediateModel.h"
#include "RW/Sections/CRWSection_ExtraVertColour.h"
#include "RW/Sections/CRWSection_2dEffect.h"
#include "RW/Sections/CRWSection_Extension.h"
#include "Testing/CDebugger.h"

using namespace std;

CIntermediateModelFormat*	CDFFFormat::convertToIntermediateModelFormat(void)
{
	CIntermediateModelFormat *pGeneralModelFile = new CIntermediateModelFormat;

	CIntermediateModel *pGeneralModel = new CIntermediateModel;
	for (CRWSection *pRWSection : getSectionsByType(RW_SECTION_GEOMETRY))
	{
		CRWSection_Geometry *pRWSection_Geometry = (CRWSection_Geometry*) pRWSection;
		CIntermediateGeometry *pGeneralGeometry = pRWSection_Geometry->convertToIntermediateGeometry();
		pGeneralModel->addEntry(pGeneralGeometry);
	}
	pGeneralModelFile->addEntry(pGeneralModel);

	return pGeneralModelFile;
}