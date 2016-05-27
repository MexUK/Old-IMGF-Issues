#ifndef CWDRBlock_Model_H
#define CWDRBlock_Model_H

#include <vector>

class CWDRBlock_Geometry;

class CWDRBlock_Model
{
public:
	std::vector<CWDRBlock_Geometry*>		m_vecGeometries;
};

#endif