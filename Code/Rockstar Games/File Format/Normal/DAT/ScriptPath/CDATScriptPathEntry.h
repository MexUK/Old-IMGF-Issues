#ifndef CDATScriptPathEntry_H
#define CDATScriptPathEntry_H

#include "Types.h"
#include "CVector3D.h"

class CDATScriptPathEntry
{
public:
	CDATScriptPathEntry(void);
	
	void					unload(void) {}

	void					setPosition(CVector3D& vecPosition) { m_vecPosition = vecPosition; }
	CVector3D&				getPosition(void) { return m_vecPosition; }
	
	void					unserialize(void); // todo
	void					serialize(void); // todo
	
private:
	CVector3D				m_vecPosition;
};

#endif