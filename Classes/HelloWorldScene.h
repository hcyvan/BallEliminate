#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "BallSprite.h"
#include "cocos2d.h"
#include <utility>
USING_NS_CC;
#define ROW_NUM (9)					//行数
#define COL_NUM (9)					//列数
#define FALL_DIST (100.0)			//小球下落距离
#define FALL_TIME (0.2)				//小球下落时间
#define ADD_BALL_TOP_TIME (0.2)		//上方添加小球的时间
#define ADD_BALL_MID_TIME (0.02)		//中间添加小球的时间
#define SWAP_TIME (0.2)				//小球交换时间

class HelloWorld : public cocos2d::Layer
{
public:
	// 构造函数
	HelloWorld():Layer(),
				visibleSize(Director::getInstance()->getVisibleSize()),
				ballWidth(BallSprite::create(0,0)->getContentSize().width),
				matrixX((visibleSize.width-ballWidth*(COL_NUM-1))/2),
				matrixY((visibleSize.height-ballWidth*(ROW_NUM-1))/2),
				swapPairFirstOK(false),
				swapPairSecondOK(false),
				isSwapPairOK(false)
				{};

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	

	// 初始化并显示小球的矩阵
	void ballMatrixInit(float x,float y);
	// 由scheduleUpdate()调用
	virtual void update(float d) override;
	// 检查删除和下落
	void checkDeleteDown();
	// 更新并检查是否存在相连的三个连续小球
	bool checkUpdateIs3();
	// 添加小球(从上方/中间
	void addBallTop();
	void addBallMid();
	// 添加障碍物
	void createObstacle(int,int);
	// 判断屏幕上是否有小球在移动
	bool updateIsMoving();
	// 将屏幕上的像素坐标转换成小球在二维数组中的坐标，并返回。如果像素坐标没有在小球矩阵范围内怎返回(-1,-1)）
	std::pair<int,int> indexOfMatrix(Vec2);
	// 点击事件的处理函数
	bool touchBeginHandler(cocos2d::Touch *t,cocos2d::Event *e);
	//	交换存储在swapPair中的连个坐标出的小球
	void swapBall();
	// 移动小球
	void moveBall(int, int, int, int, float);


	// 屏幕高度和宽度
	Size visibleSize;
	// 小球的宽度/高度
	float ballWidth;
	// 第一个球在屏幕的位置
	float matrixX,matrixY;
	// 是否获得第一个将交换的小球
	bool swapPairFirstOK;
	// 是否获得第二个将交换的小球
	bool swapPairSecondOK;
	// 两个将要交换的小球的坐标是否准备完毕
	bool isSwapPairOK;



	// 管理小球的二维数组
	BallSprite * ballMatrix[ROW_NUM][COL_NUM];
	// 存储先后点击的两个小球在二维数组中的坐标
	std::pair<std::pair<int,int>,std::pair<int,int>> swapPair;			
};

#endif 
