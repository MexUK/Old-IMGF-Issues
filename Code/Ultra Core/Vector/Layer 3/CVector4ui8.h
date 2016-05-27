#ifndef CVector4ui8_H
#define CVector4ui8_H

#include "Types.h"
#include "Vector/Layer 2/CVector4.h"

class CVector4ui8 : public CVector4<uint8>
{
public:
	uint32					convertToUint32(void)
	{
		return m_x +
			(m_y * 256) +
			(m_z * 65536) +
			(m_w * 16777216);
		// todo - make like: uint32 CMathUtility::getUint32(ui8_1, ui8_2, ui8_3, ui8_4, bool bBigEndian = false)
		// todo - and make this function take a bool for endian type
	};
};

#endif