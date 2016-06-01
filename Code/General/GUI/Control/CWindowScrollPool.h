#ifndef CWindowScrollPool_H
#define CWindowScrollPool_H

#include "Pool/CVectorPool.h"
#include "GUI/Controls/CWindowControl_Scroll.h"

class CWindowScrollPool : public CVectorPool<CWindowControl_Scroll*> {};

#endif