#include "CLSTEntry.h"
#include "String/CStringUtility.h"

using namespace std;

string					CLSTEntry::getValue(uint32 uiValueIndex)
{
	return m_deqValues[uiValueIndex];
}

string					CLSTEntry::getLine(void)
{
	return m_strName + " " + CStringUtility::join(m_deqValues, " ");
}

string					CLSTEntry::getValuesLine(void)
{
	return CStringUtility::join(m_deqValues, " ");
}