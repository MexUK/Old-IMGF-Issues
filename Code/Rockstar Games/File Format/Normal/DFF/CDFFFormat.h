#ifndef CDFFFormat_H
#define CDFFFormat_H

#include "RW/CRWFormat.h"
#include "RW/CRWSection.h"
#include "Model/CIntermediateModelFormat.h"
#include <string>
#include <vector>
#include <unordered_map>

class CRWVersion;
class CTextureEntry;
class C2dEffect;

class CDFFFormat : public CRWFormat
{
public:
	CIntermediateModelFormat*									convertToIntermediateModelFormat(void);
};

#endif