#ifndef CVector3_H
#define CVector3_H

#include "Types.h"
#include "Vector/Layer 1/CVector.h"

template<typename T>
class CVector3 : public CVector<T>
{
public:
	CVector3(void) :
		m_x((T) 0),
		m_y((T) 0),
		m_z((T) 0)
	{
	}
	CVector3(T x, T y, T z) :
		m_x(x),
		m_y(y),
		m_z(z)
	{
	}

public:
	T				m_x;
	T				m_y;
	T				m_z;
};

#endif