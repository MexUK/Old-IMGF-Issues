#ifndef CIDEEntry_PATH_H
#define CIDEEntry_PATH_H

#include "Types.h"
#include "IDE/Entry/CIDEEntry_Data.h"
#include "IDE/eIDEPathType.h"

class CIDEEntry_PATH_Group;

class CIDEEntry_PATH : public CIDEEntry_Data
{
public:
	CIDEEntry_PATH(CIDEFormat *pIDEFormat, eIDEPathType eIDEPathTypeValue) :
		CIDEEntry_Data(pIDEFormat, IDE_SECTION_PATH),
		m_eIDEPathType(eIDEPathTypeValue)
	{
	};

	virtual void					unserialize(void);
	virtual void					serialize(void);

	void							setIDEPathType(eIDEPathType eIDEPathTypeValue) { m_eIDEPathType = eIDEPathTypeValue; }
	eIDEPathType					getIDEPathType(void) { return m_eIDEPathType; }

	static void						setLatestPathGroup(CIDEEntry_PATH_Group* pLatestPathGroup) { m_pLatestPathGroup = pLatestPathGroup; }
	static CIDEEntry_PATH_Group*	getLatestPathGroup(void) { return m_pLatestPathGroup; }

private:
	static CIDEEntry_PATH_Group*	m_pLatestPathGroup;

	eIDEPathType					m_eIDEPathType;
};

#endif