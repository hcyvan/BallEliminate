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
	// �������� 
	auto background =Sprite::create("background.png");
	background->setPosition(visibleSize/2);
	this->addChild(background);
	// ��ʼ������ʾС��ľ���
	ballMatrixInit(matrixX,matrixY);

	// ��ʱ��⡢ɾ�������䡢���
	scheduleUpdate();

	// ���õ���¼�����
	auto listener=EventListenerTouchOneByOne::create();
	listener->onTouchBegan=CC_CALLBACK_2(HelloWorld::touchBeginHandler,this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,background);

	// �رհ�ť�ļ�����
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
			//��ballMatrix�е�BallSprite* ��ӵ�HelloWorld����
			this->addChild(ballMatrix[row][col]);
		}
	}
}

void HelloWorld::update(float d)
{
    //��û����Actionʱ�����ɾ�������BallSprite
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
// ���ɾ��������С��
void HelloWorld::checkDeleteDown()
{
	checkUpdateIs3();
	// ����
	for(int j=0;j<COL_NUM;++j){
		for(int i=0;i<ROW_NUM;++i){
			if(ballMatrix[i][j]!=nullptr && ballMatrix[i][j]->getIs3()==true){
				this->removeChild(ballMatrix[i][j]);
				// �˴��Զ����ж�������
				this->ballMatrix[i][j]=nullptr;
			}
		}
	}
	// ����
	int nullptr_num=0;
	for(int i=0;i<COL_NUM;++i){
		nullptr_num=0;
		for (int j=0;j<ROW_NUM;++j){
			if(ballMatrix[j][i]==nullptr){
				++nullptr_num;
			}else{
				// ��BallSpriet����
				if(nullptr_num!=0){
					ballMatrix[j-nullptr_num][i]=ballMatrix[j][i];
					ballMatrix[j-nullptr_num][i]->runAction(MoveBy::create(FALL_TIME,Point(0,-nullptr_num*ballWidth)));
				}
			}
		}
		// �������ballMatrix�е�ָ����0
		for(int j=ROW_NUM-nullptr_num;j<ROW_NUM;++j){
			ballMatrix[j][i]=nullptr;
		}
	}
}

bool HelloWorld::checkUpdateIs3()
{
	bool hasIs3=false;
	// check��
	for(int j=0;j<ROW_NUM;++j){
		for(int i=0;i<COL_NUM;++i){
			if(	i<(COL_NUM-2) &&
				// ȷ�� ballMatrix���ָ�벿λnullptr
				ballMatrix[j][i]!=nullptr &&
				ballMatrix[j][i+1]!=nullptr &&
				ballMatrix[j][i+2]!=nullptr &&
				// �ж��Ƿ���������������ͬ
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
	// check��
	for(int i=0;i<COL_NUM;++i){
		for(int j=0;j<ROW_NUM;++j){
			if( j<(ROW_NUM-2) &&
				// ȷ�� ballMatrix���ָ�벿λnullptr
				ballMatrix[j][i]!=nullptr &&
				ballMatrix[j+1][i]!=nullptr &&
				ballMatrix[j+2][i]!=nullptr &&
				// �ж��Ƿ���������������ͬ
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
	// check ��T��
	for(int j=0;j<ROW_NUM;++j){
		for(int i=0;i<COL_NUM;++i){
			if(	i<(COL_NUM-2) &&
				// ȷ�� ballMatrix���ָ�벿λnullptr
				ballMatrix[j][i]!=nullptr &&
				ballMatrix[j][i+1]!=nullptr &&
				ballMatrix[j][i-1]!=nullptr &&
				ballMatrix[j+1][i]!=nullptr &&
				ballMatrix[j-1][i]!=nullptr &&
				// �ж��Ƿ���������������ͬ
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

// ���С��
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

// ������С��
void HelloWorld::swapBall()
{
	if(isSwapPairOK){
		int i0=swapPair.first.first;
		int j0=swapPair.first.second;
		int i1=swapPair.second.first;
		int j1=swapPair.second.second;
		// ֻ�����ڵ�������ſ��Խ���
		if((i0==i1 && ((j0==j1+1)||(j0==j1-1))) ||
			(j0==j1 && ((i0==i1+1)||(i0==i1-1)))
			){
			auto temp=ballMatrix[j0][i0];
			ballMatrix[j0][i0]=ballMatrix[j1][i1];
			ballMatrix[j1][i1]=temp;
			// �����������ܹ���������ȡ������
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
// ����ϰ���
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