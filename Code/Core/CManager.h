#ifndef CManager_H
#define CManager_H

class CManager
{
public:
	virtual void				init(void) = 0;
	virtual void				uninit(void) = 0;
};

#endif