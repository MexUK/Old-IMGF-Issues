#ifndef CICOFormat_H
#define CICOFormat_H

#include "Types.h"
#include "../BMP/CBMPFormat.h"

class CICOFormat : public CBMPFormat
{
public:
	CICOFormat(void);
	
	void							unload(void) {};
	
private:
	void							unserialize(void);
	void							serialize(void);
};

#endif