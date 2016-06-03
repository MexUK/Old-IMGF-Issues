#ifndef CGUIScrollPool_H
#define CGUIScrollPool_H

#include "Pool/CVectorPool.h"
#include "GUI/Controls/CScrollControl.h"

class CGUIScrollPool : public CVectorPool<CScrollControl*> {};

#endif