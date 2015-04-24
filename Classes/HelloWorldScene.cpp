#include "HelloWorldScene.h"
#include "BallSprite.h"
USING_NS_CC;


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	// 创建背景 
	auto background =Sprite::create("background.png");
	background->setPosition(visibleSize/2);
	this->addChild(background);
	// 初始化并显示小球的矩阵
	ballMatrixInit(matrixX,matrixY);

	// 定时检测、删除、下落、添加
	scheduleUpdate();

	// 设置点击事件监听
	auto listener=EventListenerTouchOneByOne::create();
	listener->onTouchBegan=CC_CALLBACK_2(HelloWorld::touchBeginHandler,this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,background);

	// 关闭按钮的监听器
	auto close=Sprite::create("CloseNormal.png");
	close->setPosition(matrixX+(COL_NUM+1)*ballWidth,matrixY);
	addChild(close);

	auto closeListener=EventListenerTouchOneByOne::create();
	closeListener->onTouchBegan=[this](Touch*t,Event*e){
		if(e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation()))
			Director::getInstance()->end();
		return false;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(closeListener,close);

	return true;
}

void HelloWorld::ballMatrixInit(float x,float y)
{
	for(int row=0;row<ROW_NUM;++row){
		for(int col=0;col<COL_NUM;++col){
			auto ball=ballMatrix[row][col]=BallSprite::create(x+ballWidth*col,y+ballWidth*row+FALL_DIST);
			ball->runAction(MoveBy::create(FALL_TIME,Point(0,-FALL_DIST)));
			//将ballMatrix中的BallSprite* 添加到HelloWorld层中
			this->addChild(ballMatrix[row][col]);
		}
	}
}

void HelloWorld::update(float d)
{
    //当没有任Action时，检测删除及添加BallSprite
    if (!updateIsMoving()) {
		checkDeleteDown();	
    }
	if (!updateIsMoving()) {
		addBall();		
    }
	if (!updateIsMoving()) {
		swapBall();		
    }
}
// 检测删除并落下小球
void HelloWorld::checkDeleteDown()
{
	checkUpdateIs3();
	// 消除
	for(int j=0;j<COL_NUM;++j){
		for(int i=0;i<ROW_NUM;++i){
			if(ballMatrix[i][j]!=nullptr && ballMatrix[i][j]->getIs3()==true){
				this->removeChild(ballMatrix[i][j]);
				// 此处自动进行对象销毁
				this->ballMatrix[i][j]=nullptr;
			}
		}
	}
	// 下落
	int nullptr_num=0;
	for(int i=0;i<COL_NUM;++i){
		nullptr_num=0;
		for (int j=0;j<ROW_NUM;++j){
			if(ballMatrix[j][i]==nullptr){
				++nullptr_num;
			}else{
				// 将BallSpriet下移
				if(nullptr_num!=0){
					ballMatrix[j-nullptr_num][i]=ballMatrix[j][i];
					ballMatrix[j-nullptr_num][i]->runAction(MoveBy::create(FALL_TIME,Point(0,-nullptr_num*ballWidth)));
				}
			}
		}
		// 将下落的ballMatrix中的指针置0
		for(int j=ROW_NUM-nullptr_num;j<ROW_NUM;++j){
			ballMatrix[j][i]=nullptr;
		}
	}
}

bool HelloWorld::checkUpdateIs3()
{
	bool hasIs3=false;
	// check行
	for(int j=0;j<ROW_NUM;++j){
		for(int i=0;i<COL_NUM;++i){
			if(	i<(COL_NUM-2) &&
				// 确保 ballMatrix里的指针部位nullptr
				ballMatrix[j][i]!=nullptr &&
				ballMatrix[j][i+1]!=nullptr &&
				ballMatrix[j][i+2]!=nullptr &&
				// 判断是否三个连续的球相同
				ballMatrix[j][i]->getImgIndex()==ballMatrix[j][i+1]->getImgIndex() &&
				ballMatrix[j][i]->getImgIndex()==ballMatrix[j][i+2]->getImgIndex()
				){
					hasIs3=true;
					ballMatrix[j][i]->setIs3(true);
					ballMatrix[j][i+1]->setIs3(true);
					ballMatrix[j][i+2]->setIs3(true);
			}
		}
	}
	// check列
	for(int i=0;i<COL_NUM;++i){
		for(int j=0;j<ROW_NUM;++j){
			if( j<(ROW_NUM-2) &&
				// 确保 ballMatrix里的指针部位nullptr
				ballMatrix[j][i]!=nullptr &&
				ballMatrix[j+1][i]!=nullptr &&
				ballMatrix[j+2][i]!=nullptr &&
				// 判断是否三个连续的球相同
				ballMatrix[j][i]->getImgIndex()==ballMatrix[j+1][i]->getImgIndex() &&
				ballMatrix[j][i]->getImgIndex()==ballMatrix[j+2][i]->getImgIndex()
				){
					hasIs3=true;
					ballMatrix[j][i]->setIs3(true);
					ballMatrix[j+1][i]->setIs3(true);
					ballMatrix[j+2][i]->setIs3(true);
			}
		}
	}
	// check 半T形
	for(int j=0;j<ROW_NUM;++j){
		for(int i=0;i<COL_NUM;++i){
			if(	i<(COL_NUM-2) &&
				// 确保 ballMatrix里的指针部位nullptr
				ballMatrix[j][i]!=nullptr &&
				ballMatrix[j][i+1]!=nullptr &&
				ballMatrix[j][i-1]!=nullptr &&
				ballMatrix[j+1][i]!=nullptr &&
				ballMatrix[j-1][i]!=nullptr &&
				// 判断是否三个连续的球相同
				ballMatrix[j][i]->getIs3()==true
				){
				auto index=ballMatrix[j][i]->getImgIndex();
				if(i+1<COL_NUM && index==ballMatrix[j][i+1]->getImgIndex())
					ballMatrix[j][i+1]->setIs3(true);
				if(i-1>0 && index==ballMatrix[j][i-1]->getImgIndex())
					ballMatrix[j][i-1]->setIs3(true);
				if(j+1<ROW_NUM && index==ballMatrix[j+1][i]->getImgIndex())
					ballMatrix[j+1][i]->setIs3(true);
				if(j-1>ROW_NUM && index==ballMatrix[j-1][i]->getImgIndex())
					ballMatrix[j-1][i]->setIs3(true);
			}
		}
	}

	return hasIs3;
}

// 添加小球
void HelloWorld::addBall()
{
	for(int i=0;i<COL_NUM;++i){
		for (int j=0;j<ROW_NUM;++j){
			if(ballMatrix[j][i]==nullptr){
				auto ball=ballMatrix[j][i]=BallSprite::create(matrixX+i*ballWidth,matrixY+j*ballWidth+FALL_DIST);
				ball->runAction(MoveBy::create(FALL_TIME,Point(0,-FALL_DIST)));
				this->addChild(ballMatrix[j][i]);
			}
		}
	}
}

// 交换两小球
void HelloWorld::swapBall()
{
	if(isSwapPairOK){
		int i0=swapPair.first.first;
		int j0=swapPair.first.second;
		int i1=swapPair.second.first;
		int j1=swapPair.second.second;
		// 只有相邻的两个球才可以交换
		if((i0==i1 && ((j0==j1+1)||(j0==j1-1))) ||
			(j0==j1 && ((i0==i1+1)||(i0==i1-1)))
			){
			auto temp=ballMatrix[j0][i0];
			ballMatrix[j0][i0]=ballMatrix[j1][i1];
			ballMatrix[j1][i1]=temp;
			// 若果交换后不能够消除，则取消交换
			if(!checkUpdateIs3()){
				temp=ballMatrix[j0][i0];
				ballMatrix[j0][i0]=ballMatrix[j1][i1];
				ballMatrix[j1][i1]=temp;				
			}else{
				ballMatrix[j0][i0]->runAction(MoveTo::create(SWAP_TIME,Vec2(ballMatrix[j1][i1]->getPosition())));
				ballMatrix[j1][i1]->runAction(MoveTo::create(SWAP_TIME,Vec2(ballMatrix[j0][i0]->getPosition())));
			}
		}
		isSwapPairOK=false;
	}
}

bool HelloWorld::updateIsMoving()
{
	 for (int j = 0; j < ROW_NUM; ++j) {
		for(int i=0;i<COL_NUM;++i){
			BallSprite* ball= ballMatrix[j][i];
			if (ball && ball->getNumberOfRunningActions() > 0) {
				return true;
			}
		}
	}
	return false;
}

std::pair<int,int> HelloWorld::indexOfMatrix(Vec2 touchPoint)
{
	std::pair<int,int> index(-1,-1);
	for(int j=0;j<ROW_NUM;++j){
		for(int i=0;i<COL_NUM;++i){
			auto ball=ballMatrix[j][i];
			if(	
				ballMatrix[j][i]!=nullptr &&
				ballMatrix[j][i]->getPosition().x-ballWidth/2<=touchPoint.x &&
				ballMatrix[j][i]->getPosition().x+ballWidth/2>=touchPoint.x &&
				ballMatrix[j][i]->getPosition().y-ballWidth/2<=touchPoint.y &&
				ballMatrix[j][i]->getPosition().y+ballWidth/2>=touchPoint.y 
				){
					index.first=i;
					index.second=j;
					return index;
			}
		}
	}
	return index;
}


bool HelloWorld::touchBeginHandler(cocos2d::Touch *t,cocos2d::Event *e)
{
	if(!swapPairFirstOK && !swapPairSecondOK && this->indexOfMatrix(t->getLocation()).first>=0){
		swapPair.first=this->indexOfMatrix(t->getLocation());
		swapPairFirstOK=true;
		return false;
	}
	if(swapPairFirstOK && !swapPairSecondOK && this->indexOfMatrix(t->getLocation()).first>=0){
		swapPair.second=this->indexOfMatrix(t->getLocation());
		swapPairSecondOK=true;
		swapPairFirstOK=false;
		swapPairSecondOK=false;
		isSwapPairOK=true;
		return false;
	}
}

/*
// 添加障碍物
void HelloWorld::addObstacle(int i,int j)
{
	auto obstacle=BallSprite::create(matrixX+i*ballWidth,matrixY+j*ballWidth+FALL_DIST);
	obstacle->setIsObstacle(true);
	obstacle->setImgIndex(10);
	obstacle->setTexture("obstacle.png");
	obstacle->runAction(MoveBy::create(FALL_TIME,Point(0,-FALL_DIST)));
	addChild(obstacle);
}
*/