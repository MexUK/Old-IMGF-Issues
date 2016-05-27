#ifndef CRWSection_GeometryList_H
#define CRWSection_GeometryList_H

#include "../CRWSection.h"

class CRWSection_GeometryList : public CRWSection
{
public:
	CRWSection_GeometryList(void);

	void							unserialize(void);
	void							serialize(void);

	void							setGeometryCount(uint32 uiGeometryCount) { m_uiGeometryCount = uiGeometryCount; }
	uint32							getGeometryCount(void) { return m_uiGeometryCount; }

private:
	uint32							m_uiGeometryCount;
};

#endif