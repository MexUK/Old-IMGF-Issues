#ifndef CGUIStyles_H
#define CGUIStyles_H

#include "Types.h"
#include "Pool/CMultipleTypeValuesUMapContainer.h"
#include <string>

class CGUIStyles : public CMultipleTypeValuesUMapContainer<std::string>
{
public:
	CGUIStyles(void);

	void					init(void);
	void					uninit(void);

	bool					doesStyleExist(std::string strStyleName);

	void					setStyle(std::string strStyleName, bool bStyleValue);
	void					setStyle(std::string strStyleName, uint32 uiStyleValue);
	void					setStyle(std::string strStyleName, int32 iStyleValue);
	void					setStyle(std::string strStyleName, float32 fStyleValue);
	void					setStyle(std::string strStyleName, std::string strStyleValue);

	template <typename ValueType>
	ValueType				getStyle(std::string strStyleName);

	template <typename ValueType>
	ValueType				getStyleDefaultValue(std::string strStyleName);

	bool					doesHaveBorder(void);
	bool					doesHaveFill(void);

	void					setStyleNameOverwrite(std::string strStyleName, std::string strNewStyleName);
	void					restoreStyleNameOverwrites(void);
	std::string				getStyleNameOverwrite(std::string strStyleName);

	static CMultipleTypeValuesUMapContainer<std::string>&	getStyleDefaultValues(void) { return m_umapStyleDefaultValues; }

private:
	std::unordered_map<std::string, std::string>			m_umapStyleNameOverwrites;
	static CMultipleTypeValuesUMapContainer<std::string>	m_umapStyleDefaultValues;
};




template <typename ValueType>
ValueType				CGUIStyles::getStyle(std::string strStyleName)
{
	strStyleName = getStyleNameOverwrite(strStyleName);
	if (doesStyleExist(strStyleName))
	{
		return *getEntryPointer<ValueType>(strStyleName);
	}
	else
	{
		return getStyleDefaultValue<ValueType>(strStyleName);
	}
}

template <typename ValueType>
ValueType				CGUIStyles::getStyleDefaultValue(std::string strStyleName)
{
	strStyleName = getStyleNameOverwrite(strStyleName);
	if (getStyleDefaultValues().doesEntryExist(strStyleName))
	{
		return *(getStyleDefaultValues().getEntryPointer<ValueType>(strStyleName));
	}
	else
	{
		return (ValueType) 0;
	}
}

#endif