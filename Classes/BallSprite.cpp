#include "BallSprite.h"
#include "cocos2d.h"
#include <memory>
USING_NS_CC;
#define TOTAL_BALL_TYPE (4)
static const char *BallPng[] = {
	"01.png",
	"02.png",
	"03.png",
	"04.png",
};
BallSprite *BallSprite::create(float col, float row) 
{ 
    BallSprite *ball = new BallSprite(); 
	//产生0-TOTAL_BALL_TYPE-1的随机数
    ball->imgIndex = random() %TOTAL_BALL_TYPE; 
	//初始化ball
	ball->initWithFile(BallPng[ball->imgIndex]);
	ball->setPosition(col,row);   
    ball->autorelease(); 
    return ball; 
}