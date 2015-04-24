#ifndef __BallSprite_H__
#define __BallSprite_H__

#include "cocos2d.h"

USING_NS_CC;


class BallSprite :  public Sprite
{
public:
	BallSprite():imgIndex(-1),is3(false),isObstacle(false){}
	
	// ����һ��С����
	static BallSprite *create(float, float); 


	// imgIndexΪС�������
    CC_SYNTHESIZE(int, imgIndex, ImgIndex);
	// is3��ʾС��������С��֮һ
	CC_SYNTHESIZE(bool, is3, Is3);
	// isObstacle��ʾ�����ϰ���
	CC_SYNTHESIZE(bool, isObstacle, IsObstacle);
};

#endif