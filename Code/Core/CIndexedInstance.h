#ifndef CIndexedInstance_H
#define CIndexedInstance_H

#include <vector>

template<class Class>
class CIndexedInstance
{
public:
	static Class*						getInstance(uint32 uiInstanceIndex = 0);
	static std::vector<Class*>&			getInstances(void) { return m_vecInstances; }

private:
	static std::vector<Class*>			m_vecInstances;
};

template <class Class>
std::vector<Class*>						CIndexedInstance<Class>::m_vecInstances;

template <class Class>
Class*									CIndexedInstance<Class>::getInstance(uint32 uiInstanceIndex)
{
	if (uiInstanceIndex >= m_vecInstances.size())
	{
		m_vecInstances.resize(uiInstanceIndex + 1);
		m_vecInstances[uiInstanceIndex] = new Class;
	}
	return m_vecInstances[uiInstanceIndex];
}

#endif