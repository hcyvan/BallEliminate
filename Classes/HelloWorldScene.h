#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "BallSprite.h"
#include "cocos2d.h"
#include <utility>
USING_NS_CC;
#define ROW_NUM (9)					//����
#define COL_NUM (9)					//����
#define FALL_DIST (100.0)			//С���������
#define FALL_TIME (0.2)				//С������ʱ��
#define ADD_BALL_TOP_TIME (0.2)		//�Ϸ����С���ʱ��
#define ADD_BALL_MID_TIME (0.02)		//�м����С���ʱ��
#define SWAP_TIME (0.2)				//С�򽻻�ʱ��

class HelloWorld : public cocos2d::Layer
{
public:
	// ���캯��
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
	

	// ��ʼ������ʾС��ľ���
	void ballMatrixInit(float x,float y);
	// ��scheduleUpdate()����
	virtual void update(float d) override;
	// ���ɾ��������
	void checkDeleteDown();
	// ���²�����Ƿ������������������С��
	bool checkUpdateIs3();
	// ���С��(���Ϸ�/�м�
	void addBallTop();
	void addBallMid();
	// ����ϰ���
	void createObstacle(int,int);
	// �ж���Ļ���Ƿ���С�����ƶ�
	bool updateIsMoving();
	// ����Ļ�ϵ���������ת����С���ڶ�ά�����е����꣬�����ء������������û����С�����Χ��������(-1,-1)��
	std::pair<int,int> indexOfMatrix(Vec2);
	// ����¼��Ĵ�����
	bool touchBeginHandler(cocos2d::Touch *t,cocos2d::Event *e);
	//	�����洢��swapPair�е������������С��
	void swapBall();
	// �ƶ�С��
	void moveBall(int, int, int, int, float);


	// ��Ļ�߶ȺͿ��
	Size visibleSize;
	// С��Ŀ��/�߶�
	float ballWidth;
	// ��һ��������Ļ��λ��
	float matrixX,matrixY;
	// �Ƿ��õ�һ����������С��
	bool swapPairFirstOK;
	// �Ƿ��õڶ�����������С��
	bool swapPairSecondOK;
	// ������Ҫ������С��������Ƿ�׼�����
	bool isSwapPairOK;



	// ����С��Ķ�ά����
	BallSprite * ballMatrix[ROW_NUM][COL_NUM];
	// �洢�Ⱥ���������С���ڶ�ά�����е�����
	std::pair<std::pair<int,int>,std::pair<int,int>> swapPair;			
};

#endif 
