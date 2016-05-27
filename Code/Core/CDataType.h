#ifndef CDataType_H
#define CDataType_H

#include "eDataType.h"
#include <string>

class CDataType
{
public:
	static eDataType						getDataTypeFromText(std::string& strDataFormat);
};

#endif