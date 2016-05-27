#ifndef CVector4_H
#define CVector4_H

#include "Types.h"
#include "Vector/Layer 1/CVector.h"

template<typename T>
class CVector4 : public CVector<T>
{
public:
	T				m_x;
	T				m_y;
	T				m_z;
	T				m_w;
};

#endif