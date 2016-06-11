#ifndef CVector4_H
#define CVector4_H

#include "Types.h"
#include "Vector/Layer 1/CVector.h"

template<typename T>
class CVector4 : public CVector<T>
{
public:
	CVector4(void) :
		m_x((T) 0),
		m_y((T) 0),
		m_z((T) 0),
		m_w((T) 0)
	{
	}
	CVector4(T x, T y, T z, T w) :
		m_x(x),
		m_y(y),
		m_z(z),
		m_w(w)
	{
	}

public:
	T				m_x;
	T				m_y;
	T				m_z;
	T				m_w;
};

#endif