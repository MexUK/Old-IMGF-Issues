#include "CDataType.h"
#include "eDataType.h"

using namespace std;

eDataType						CDataType::getDataTypeFromText(string& strDataFormat)
{
	if (strDataFormat == "s")
	{
		return DATATYPE_STRING;
	}
	else if (strDataFormat == "uint8")
	{
		return DATATYPE_UINT8;
	}
	else if (strDataFormat == "uint16")
	{
		return DATATYPE_UINT16;
	}
	else if (strDataFormat == "uint32")
	{
		return DATATYPE_UINT32;
	}
	else if (strDataFormat == "int8")
	{
		return DATATYPE_INT8;
	}
	else if (strDataFormat == "int16")
	{
		return DATATYPE_INT16;
	}
	else if (strDataFormat == "int32")
	{
		return DATATYPE_INT32;
	}
	else if (strDataFormat == "f32")
	{
		return DATATYPE_FLOAT32;
	}
	else if (strDataFormat == "f64")
	{
		return DATATYPE_FLOAT64;
	}
	else if (strDataFormat == "f80")
	{
		return DATATYPE_FLOAT80;
	}
}