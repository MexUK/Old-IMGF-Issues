#ifndef CVector2_H
#define CVector2_H

#include "Types.h"
#include "Vector/Layer 1/CVector.h"

template<typename T>
class CVector2 : public CVector<T>
{
public:
	CVector2(void)
	{};
	CVector2(T x, T y) :
		m_x(x),
		m_y(y)
	{};

public:
	T				m_x;
	T				m_y;
};

#endif