#ifndef CDDSFormat_H
#define CDDSFormat_H

#include "CFormat.h"
#include "eEndian.h"
#include <string>

class CDDSFormat : public CFormat
{
public:
	CDDSFormat(void);

private:
	void								unserialize(void);
	void								serialize(void);

public:
	uint32								m_uiWidth;
	uint32								m_uiHeight;
	uint32								m_uiBPP;
	std::string							m_strRasterData;
};

#endif