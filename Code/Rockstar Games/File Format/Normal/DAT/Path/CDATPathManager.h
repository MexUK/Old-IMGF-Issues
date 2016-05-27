#ifndef CDATPathManager_H
#define CDATPathManager_H

#include "CFormatManager.h"
#include "CSingleton.h"
#include "CVector2D.h"
#include "Game/ePlatformedGame.h"
#include <string>
#include <vector>

class CDATPathFormat;
class CDATEntry_Paths_General_PathNode;
class CDATEntry_Paths_General_NaviNode;

class CDATPathManager : public CFormatManager<CDATPathFormat>, public CSingleton<CDATPathManager>
{
public:
	CDATPathManager(void);

	void						init(void);
	void						uninit(void);

	static uint32		getAreaIdFromPosition(CVector2D& vecPosition, CVector2D& vecMapRangeMin, CVector2D& vecMapRangeMax, CVector2D& vecAreaSize);
	static uint32		getTileCount(CVector2D& vecMapRangeMin, CVector2D& vecMapRangeMax, CVector2D& vecAreaSize);

	static bool					findPathNode(std::vector<CDATPathFormat*>& veCDATFormatPaths, CDATEntry_Paths_General_PathNode& pathNodeToFind, uint32& uiAreaId_Out, uint32& uiNodeId_Out);
	static bool					findNaviNode(std::vector<CDATPathFormat*>& veCDATFormatPaths, CDATEntry_Paths_General_NaviNode& naviNodeToFind, uint32& uiAreaId_Out, uint32& uiNodeId_Out);
	static CDATEntry_Paths_General_PathNode					getLowestPathNode(std::vector<CDATEntry_Paths_General_PathNode>& vecPathNodes);
	static std::vector<CDATEntry_Paths_General_PathNode>	getTargetPathNodesForNaviNode(std::vector<CDATPathFormat*> veCDATFormats, CDATEntry_Paths_General_NaviNode& naviNode, uint32 uiNaviNodeAreaId, uint32 uiNaviNodeNodeId, uint32& uiLinkId_Out);
};

#endif