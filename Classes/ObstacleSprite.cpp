#ifndef __ObstacleSprite_H__
#define __ObstacleSprite_H__

#include "cocos2d.h"
#include "BallSprite.h"

USING_NS_CC;


class  ObstacleSprite :  public BallSprite
{
public:	
	// 创建一个障碍物
	static ObstacleSprite *create(float, float); 
	// isObstacle表示否是障碍物
	CC_SYNTHESIZE(bool, isObstacle, IsObstacle);
};

#endif