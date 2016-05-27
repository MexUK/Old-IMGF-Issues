#ifndef CIntermediateIndexData_H
#define CIntermediateIndexData_H

#include "Types.h"
#include "CVector3ui32.h"

class CIntermediateIndexData
{
public:
	CIntermediateIndexData(void)
	{
		m_vecVertexIndices.m_x = 0;
		m_vecVertexIndices.m_y = 0;
		m_vecVertexIndices.m_z = 0;
	};

	void								setVertexIndices(CVector3ui32& vecVertexIndices) { m_vecVertexIndices = vecVertexIndices; }
	CVector3ui32&						getVertexIndices(void) { return m_vecVertexIndices; }

private:
	CVector3ui32						m_vecVertexIndices;
};

#endif