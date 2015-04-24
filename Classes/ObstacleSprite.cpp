#ifndef __ObstacleSprite_H__
#define __ObstacleSprite_H__

#include "cocos2d.h"
#include "BallSprite.h"

USING_NS_CC;


class  ObstacleSprite :  public BallSprite
{
public:	
	// ����һ���ϰ���
	static ObstacleSprite *create(float, float); 
	// isObstacle��ʾ�����ϰ���
	CC_SYNTHESIZE(bool, isObstacle, IsObstacle);
};

#endif