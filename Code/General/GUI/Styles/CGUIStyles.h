#ifndef CGUIStyles_H
#define CGUIStyles_H

#include "Types.h"
#include "CVector2i32.h"
#include "Pool/CMultipleTypeValuesUMapContainer.h"
#include <string>
#include <vector>

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
	void					setStyle(std::string strStyleName, ValueType value);
	template <typename ValueType>
	ValueType				getStyle(std::string strStyleName);

	template <typename ValueType>
	ValueType				getStyleDefaultValue(std::string strStyleName);

	bool					doesHaveBorder(void);
	bool					doesHaveLeftBorder(void);
	bool					doesHaveRightBorder(void);
	bool					doesHaveTopBorder(void);
	bool					doesHaveBottomBorder(void);
	bool					doesHaveFill(void);

	std::string				getTextAlignX(void);
	std::string				getTextAlignY(void);
	CVector2i32				getMinInnerSpacing(void);
	CVector2i32				getMaxInnerSpacing(void);
	uint32					getInnerSpacingTotalX(void);
	uint32					getInnerSpacingTotalY(void);

	void					restoreTemporaryStyleData(void);
	void					restoreStyleOverwrites(void);

	void					setItemComponent(std::string strItemComponent) { m_strItemComponent = strItemComponent; }
	std::string&			getItemComponent(void) { return m_strItemComponent; }

	void					setItemStatus(std::string strItemStatus) { m_strItemStatus = strItemStatus; }
	std::string&			getItemStatus(void) { return m_strItemStatus; }

	void					setHasFillOverwrite(bool bHasFillOverwrite) { m_bHasFillOverwrite = bHasFillOverwrite; }
	bool					doesHaveFillOverwrite(void) { return m_bHasFillOverwrite; }

	static CMultipleTypeValuesUMapContainer<std::string>&	getStyleDefaultValues(void) { return m_umapStyleDefaultValues; }

private:
	uint8													m_bHasFillOverwrite		: 1;
	std::string												m_strItemComponent;
	std::string												m_strItemStatus;
	static CMultipleTypeValuesUMapContainer<std::string>	m_umapStyleDefaultValues;
};




template <typename ValueType>
void				CGUIStyles::setStyle(std::string strStyleName, ValueType value)
{
	setEntry<ValueType>(strStyleName, value);
}

template <typename ValueType>
ValueType				CGUIStyles::getStyle(std::string strStyleName)
{
	std::string
		strStyleNameFullyResolved = (getItemComponent() == "" ? "default." : (getItemComponent() + ".")) + strStyleName + (getItemStatus() == "" ? "" : (":" + getItemStatus())), // with component and status
		strStyleNameWithStatus = strStyleName + (getItemStatus() == "" ? "" : (":" + getItemStatus())),
		strStyleNameWithComponent = (getItemComponent() == "" ? "default." : (getItemComponent() + ".")) + strStyleName;

	if (doesStyleExist(strStyleNameFullyResolved))
	{
		// e.g. drop-triangle.fill-colour:list-open
		return *getEntryPointer<ValueType>(strStyleNameFullyResolved);
	}
	else if (doesStyleExist(strStyleNameWithStatus))
	{
		// e.g. fill-colour:list-open
		return *getEntryPointer<ValueType>(strStyleNameWithStatus);
	}
	/*
	todo
	else if (getItemStatus() != "")
	{
		// e.g. fill-colour:list-open
		return getStyleDefaultValue<ValueType>(strStyleNameWithStatus);
	}
	*/
	else if (doesStyleExist(strStyleNameWithComponent))
	{
		// e.g. drop-triangle.fill-colour
		return *getEntryPointer<ValueType>(strStyleNameWithComponent);
	}
	else if (doesStyleExist(strStyleName))
	{
		// e.g. fill-colour
		return *getEntryPointer<ValueType>(strStyleName);
	}
	else
	{
		// e.g. fill-colour
		return getStyleDefaultValue<ValueType>(strStyleName);
	}
}

template <typename ValueType>
ValueType				CGUIStyles::getStyleDefaultValue(std::string strStyleName)
{
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