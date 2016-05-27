#ifndef CVector3_H
#define CVector3_H

#include "Types.h"
#include "Vector/Layer 1/CVector.h"

template<typename T>
class CVector3 : public CVector<T>
{
public:
	T				m_x;
	T				m_y;
	T				m_z;
};

#endif