#ifndef CWindowScrollPool_H
#define CWindowScrollPool_H

#include "Pool/CVectorPool.h"
#include "GUI/Controls/CScrollControl.h"

class CWindowScrollPool : public CVectorPool<CScrollControl*> {};

#endif