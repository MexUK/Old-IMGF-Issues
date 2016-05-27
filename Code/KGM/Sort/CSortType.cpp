#include "CSortType.h"
#include "String/CStringUtility.h"

using namespace std;

string					CSortType::getText(void)
{
	return CStringUtility::replace(m_strText, "&", "");
}

string					CSortType::getTextForMenu(void)
{
	return m_strText;
}