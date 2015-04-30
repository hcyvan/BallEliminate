#ifndef __BallSprite_H__
#define __BallSprite_H__

#include "cocos2d.h"

USING_NS_CC;


class BallSprite :  public Sprite
{
public:
	BallSprite():imgIndex(-1),is3(false){}
	
	// 创建一个小球精灵
	static BallSprite *create(float, float); 


	// imgIndex为小球的类型
    CC_SYNTHESIZE(int, imgIndex, ImgIndex);
	// is3表示小球是三连小球之一
	CC_SYNTHESIZE(bool, is3, Is3);
	// isObstacle表示否是障碍物
	//CC_SYNTHESIZE(bool, isObstacle, IsObstacle);
};

#endif