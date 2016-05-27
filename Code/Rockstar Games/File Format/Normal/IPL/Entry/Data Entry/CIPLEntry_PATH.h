#ifndef CIPLEntry_PATH_H
#define CIPLEntry_PATH_H

#include "Types.h"
#include "IPL/Entry/CIPLEntry_Data.h"
#include "IPL/eIPLPathType.h"

class CIPLEntry_PATH_Group;

class CIPLEntry_PATH : public CIPLEntry_Data
{
public:
	CIPLEntry_PATH(CIPLFormat *pIPLFormat, eIPLPathType eIPLPathTypeValue) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_PATH),
		m_eIPLPathType(eIPLPathTypeValue)
	{
	};

	virtual void					unserialize(void);
	virtual void					serialize(void);

	void							setIPLPathType(eIPLPathType eIPLPathTypeValue) { m_eIPLPathType = eIPLPathTypeValue; }
	eIPLPathType					getIPLPathType(void) { return m_eIPLPathType; }

	static void						setLatestPathGroup(CIPLEntry_PATH_Group* pLatestPathGroup) { m_pLatestPathGroup = pLatestPathGroup; }
	static CIPLEntry_PATH_Group*	getLatestPathGroup(void) { return m_pLatestPathGroup; }

private:
	static CIPLEntry_PATH_Group*	m_pLatestPathGroup;

	eIPLPathType					m_eIPLPathType;
};

#endif