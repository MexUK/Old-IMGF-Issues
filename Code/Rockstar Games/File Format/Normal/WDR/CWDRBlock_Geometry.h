#ifndef CWDRBlock_Geometry_H
#define CWDRBlock_Geometry_H

#include <vector>

class CWDRBlock_VertexData;
class CWDRBlock_IndexData;

class CWDRBlock_Geometry
{
public:
	std::vector<CWDRBlock_VertexData*>		m_vecVertices;
	std::vector<CWDRBlock_IndexData*>		m_vecIndices;
};

#endif