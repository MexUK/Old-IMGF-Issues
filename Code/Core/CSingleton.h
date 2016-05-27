#ifndef CSingleton_H
#define CSingleton_H

template<class SingletonClass>
class CSingleton
{
public:
	static SingletonClass*		getInstance(void);

private:
	static SingletonClass*		m_pInstance;
};

template <class SingletonClass>
SingletonClass*	CSingleton<SingletonClass>::m_pInstance = nullptr;

template <class SingletonClass>
SingletonClass*	CSingleton<SingletonClass>::getInstance(void)
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new SingletonClass;
	}
	return m_pInstance;
}

#endif