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

	//
	return true;
}

void HelloWorld::ballMatrixInit(float x,float y)
{
	// 初始化ballMatrix
	for(int row=0;row<ROW_NUM;++row){
		for(int col=0;col<COL_NUM;++col){
			ballMatrix[row][col]=nullptr;		
		}
	}
	// 添加障碍物
	createObstacle(4,2);
	createObstacle(5,3);

	createObstacle(3,4);
	createObstacle(4,5);
	createObstacle(5,6);
	createObstacle(6,7);

	createObstacle(1,2);
	createObstacle(2,2);




	
	// 添加小球
	addBallTop();
}

void HelloWorld::update(float d)
{
    //当没有任Action时，检测删除及添加BallSprite
    if (!updateIsMoving()) {
		checkDeleteDown();	
    }
	if (!updateIsMoving()) {
		addBallTop();		
    }
	if (!updateIsMoving()) {
		addBallMid();		
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
	for(int i=0;i<COL_NUM;++i){
		int span=0;
		for (int j=0;j<=ROW_NUM;++j){
			++span;
			if(j==ROW_NUM || (ballMatrix[j][i] != nullptr && ballMatrix[j][i]->getImgIndex()==10)){
				int nullptr_num=0;
				for(int k=j-span+1;k<j;++k){
					if(ballMatrix[k][i]==nullptr){
						++nullptr_num;
					}else{
					// 将BallSpriet下移
						if(nullptr_num!=0){
							ballMatrix[k-nullptr_num][i]=ballMatrix[k][i];					
							ballMatrix[k-nullptr_num][i]->runAction(MoveBy::create(ADD_BALL_TOP_TIME,Point(0,-nullptr_num*ballWidth)));
						}
					}
				}
				// 将下落的ballMatrix中的指针置0
				for(int k =j-nullptr_num;k<j;++k){
					ballMatrix[k][i]=nullptr;
				}
				span=0;
			}
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
				ballMatrix[j][i]->getImgIndex()==ballMatrix[j][i+2]->getImgIndex() &&
				// 保证不为障碍物
				ballMatrix[j][i]->getImgIndex()!=10
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
				ballMatrix[j][i]->getImgIndex()==ballMatrix[j+2][i]->getImgIndex() &&
				// 保证不为障碍物
				ballMatrix[j][i]->getImgIndex()!=10
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
void HelloWorld::addBallTop()
{
	for(int i=0;i<COL_NUM;++i){
		int span=0;
		int nullptr_num=0;
		for (int j=0;j<=ROW_NUM;++j){
			++span;
			if(j<ROW_NUM){
				if(ballMatrix[j][i]==nullptr){
					++nullptr_num;
				}else if(ballMatrix[j][i]->getImgIndex()==10){
					span=0;
					nullptr_num=0;
				}
			}else{
				// 从上方添加小球
				for(int k=j-nullptr_num;k<j;k++){
					ballMatrix[k][i]=BallSprite::create(matrixX+i*ballWidth,matrixY+k*ballWidth+FALL_DIST);
					ballMatrix[k][i]->runAction(MoveBy::create(ADD_BALL_TOP_TIME,Point(0,-FALL_DIST)));
					this->addChild(ballMatrix[k][i]);
				}
			}
		}
	}
}
void HelloWorld::addBallMid()
{
	for(int i=0;i<COL_NUM;++i){
		for (int j=0;j<ROW_NUM;++j){
			if(ballMatrix[j][i] != nullptr && ballMatrix[j][i]->getImgIndex()==10){
				int x=i-1+(random()%2)*2;
				if(ballMatrix[j-1][x]!=nullptr && ballMatrix[j-1][x]->getImgIndex()!=10)
					moveBall(x,j,i,j-1,ADD_BALL_MID_TIME);
			}
		}
	}
}
// 移动小球
void HelloWorld::moveBall(int x1, int y1, int x2,int y2, float t)
{
	if(x1>=0 && x1< COL_NUM && y1 >=0 && y1<COL_NUM &&
		x2>=0 && x2< COL_NUM && y2 >=0 && y2<COL_NUM &&
		ballMatrix[y1][x1]!=nullptr && ballMatrix[y2][x2]==nullptr &&
		ballMatrix[y1][x1]->getImgIndex()!=10 
		){
		ballMatrix[y2][x2]=ballMatrix[y1][x1];
		ballMatrix[y1][x1]=nullptr;
		ballMatrix[y2][x2]->runAction(MoveTo::create(t,Vec2(matrixX+x2*ballWidth,matrixY+y2*ballWidth)));
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
		log("first: %d %d",swapPair.first.first,swapPair.first.second);
		swapPairFirstOK=true;
		return false;
	}
	if(swapPairFirstOK && !swapPairSecondOK && this->indexOfMatrix(t->getLocation()).first>=0){
		swapPair.second=this->indexOfMatrix(t->getLocation());
		log("second: %d %d",swapPair.second.first,swapPair.second.second);
		swapPairSecondOK=true;
		swapPairFirstOK=false;
		swapPairSecondOK=false;
		isSwapPairOK=true;
		return false;
	}
}


// 添加障碍物
void HelloWorld::createObstacle(int i,int j)
{
	ballMatrix[j][i]=BallSprite::create(matrixX+i*ballWidth,matrixY+j*ballWidth);
	ballMatrix[j][i]->setImgIndex(10);
	ballMatrix[j][i]->setTexture("obstacle.png");
	addChild(ballMatrix[j][i]);
}
