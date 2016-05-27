#ifndef CWDRFormat_H
#define CWDRFormat_H

#include "CFormat.h"
#include "CWDRBoundingObject.h"
#include <string>
#include <vector>

class CWDRBlock_Geometry;
class CIntermediateGeometry;
class CWDRBlock_Model;

class CWDRFormat : public CFormat
{
public:
	CWDRFormat(void) : CFormat(true, LITTLE_ENDIAN) {};

	void								unload(void);

	static CWDRBlock_Geometry*			convertIntermediateGeometryToWDRGeometry(CIntermediateGeometry *pGeneralGeometry);

public:
	void								unserialize(void);
	void								serialize(void);

public: // todo - make the properties private
	CWDRBoundingObject					m_boundingObject;
	std::vector<CWDRBlock_Model*>		m_vecModels;
};

#endif