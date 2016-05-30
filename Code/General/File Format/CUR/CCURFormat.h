#ifndef CCURFormat_H
#define CCURFormat_H

#include "Types.h"
#include "../BMP/CBMPFormat.h"

class CCURFormat : public CBMPFormat
{
public:
	CCURFormat(void);
	
	void							unload(void) {};
	
private:
	void							unserialize(void);
	void							serialize(void);
};

#endif