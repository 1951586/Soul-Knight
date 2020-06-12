#include "HRocker.h"
#include "Hero.h"

//初始化：
HRocker * HRocker::createHRocker(const char * rockerImageName, 
					const char * rockerBGImageName, Point posistion,Hero* hero)
{
	HRocker * layer = HRocker::create();
	if (layer)
	{
		layer->rockerInit(rockerImageName, rockerBGImageName, posistion, hero);
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

void HRocker::rockerInit(const char * rockerImageName, 
					const char * rockerBGImageName, Point position,Hero* hero)
{
	//添加摇杆背景
	this->setAnchorPoint(Vec2(0, 0));//调整了一下锚点
	auto spRockerBg = Sprite::create(rockerBGImageName);
	spRockerBg->setVisible(false);
	spRockerBg->setAnchorPoint(Vec2(0.5f, 0.5f));
	spRockerBg->setPosition(position);
	addChild(spRockerBg, 0,tag_rockerBG);
	//添加摇杆的移动的点
	auto spRocker = Sprite::create(rockerImageName);
	spRocker->setVisible(false);
	spRocker->setAnchorPoint(Vec2(0.5f,0.5f));
	spRocker->setScale(1.1);
	spRocker->setPosition(position);
	spRocker->setScale(0.5);
	addChild(spRocker, 1, tag_rocker);
	spRocker->setOpacity(180);//透明度
	//摇杆背景图坐标
	rockerBGPosition = position;
	//摇杆背景半径
	rockerBG_r = spRocker->getContentSize().width*0.5;//使之不随图像大小变化而改变
	listener = EventListenerTouchOneByOne::create();
	listenerKeyBoard = EventListenerKeyboard::create();
	//监听绑定事件
	listener->onTouchBegan = CC_CALLBACK_2(HRocker::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(HRocker::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(HRocker::onTouchEnded,this);

	listenerKeyBoard->onKeyPressed = CC_CALLBACK_2(HRocker::onKeyPressed, this);
	listenerKeyBoard->onKeyReleased = CC_CALLBACK_2(HRocker::onKeyReleased, this);
	//listener->setSwallowTouches(true);//这里还是不能吞并
	//调整大小
	scale = 0.5;
	spRockerBg->setScale(scale);
	spRocker->setScale(scale);
	rockerBG_r = rockerBG_r * scale;//跟随着改变
	this->hero = hero;
	log("initrocker");
}

//移动平台：
Point HRocker::getAnglePosition(float r, float angel)
{
	return ccp(r*cos(angel), r*sin(angel));
}

float HRocker::getRad(Point p1, Point p2)
{
	float p1_x = p1.x;
	float p1_y = p1.y;
	float p2_x = p2.x;
	float p2_y = p2.y;
	//求出两边边长
	float x = p2_x - p1_x;
	float y = p2_y - p1_y;
	//求两点距离
	float distance = sqrt(x * x + y * y);
	//反余弦函数
	float rad = acos(x / distance);    //[0,pi]
	//调整角度
	if (p1_y > p2_y)
	{
		rad = -rad;
	}
	return rad;
}

bool HRocker::onTouchBegan(Touch * touch, Event * event)
{
	auto sp = static_cast <Sprite*> (getChildByTag(tag_rocker));
	//得到触屏点坐标
	Point point = touch->getLocation();
	//判断是否单击到sp这个精灵
	if (sp->boundingBox().containsPoint(point))//判断包含这个点
	{
		Enabled = true;
	}
	return true;
}

void HRocker::onTouchMoved(Touch * touch, Event * event)
{
	if (!Enabled)
	{
		return;
	}
	auto sp = static_cast <Sprite*> (getChildByTag(tag_rocker));
	//得到触屏点坐标
	Point point = touch->getLocation();
	dx = point.x - rockerBGPosition.x;
	dy = point.y - rockerBGPosition.y;
	if (sqrt(pow(dx, 2) + pow(dy, 2)) >= rockerBG_r)
	{
		//得到出点与摇杆背景圆心的夹角
		float angle = getRad(rockerBGPosition, point);
		//确保小圆运动范围在背景内
		dx = rockerBG_r * cos(angle);
		dy = rockerBG_r * sin(angle);
		sp->setPosition(ccpAdd(getAnglePosition(rockerBG_r, angle), ccp(rockerBGPosition.x, rockerBGPosition.y)));
	}
	else
	{
		//触点在背景内运动
		sp->setPosition(point);
	}
	dx = dx/ rockerBG_r;
	dy = dy/ rockerBG_r;//每次调用最多移动一个像素
}

void HRocker::onTouchEnded(Touch * touch, Event * event)
{
	if (!Enabled)
	{
		return;
	}
	auto rocker = static_cast <Sprite*> (getChildByTag(tag_rocker));
	auto rockerBG = static_cast <Sprite*> (getChildByTag(tag_rockerBG));
	rocker->stopAllActions();
	rocker->runAction(MoveTo::create(0.08, rockerBG->getPosition()));  //回到圆心
	Enabled = false;
	dx = 0;
	dy = 0;
}

//PC平台：
void HRocker::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)//按下
{
	log("key!");
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:
		dy += 1; //每次调用移动一个像素
		break;
	case EventKeyboard::KeyCode::KEY_D:
		dx += 1;
		break;
	case EventKeyboard::KeyCode::KEY_S:
		dy -= 1;
		break;
	case EventKeyboard::KeyCode::KEY_A:
		dx -= 1;
		break;
	case EventKeyboard::KeyCode::KEY_P:
		if (hero->isAbility)
		{
			hero->ability();
			hero->isAbility = false;
		}
	}
}

void HRocker::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)//松开
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:
		dy -= 1;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		dx -= 1;
		break;
	case EventKeyboard::KeyCode::KEY_S:
		dy += 1;
		break;
	case EventKeyboard::KeyCode::KEY_A:
		dx += 1;
		break;
	}
}

//控制启动摇杆的类型:
void HRocker::startTouchRocker()
{
	//精灵设置可见，设置监听
	Sprite* rocker = static_cast <Sprite*> (getChildByTag(tag_rocker));
	rocker->setVisible(true);
	Sprite* rockerBG = static_cast <Sprite*> (getChildByTag(tag_rockerBG));
	rockerBG->setVisible(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void HRocker::stopTouchRocker()
{
	//精灵设置不可见，取消监听
	Sprite* rocker = static_cast <Sprite*> (getChildByTag(tag_rocker));
	rocker->setVisible(false);
	Sprite* rockerBG = static_cast <Sprite*> (getChildByTag(tag_rockerBG));
	rockerBG->setVisible(false);
	Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
}

void HRocker::startKeyRocker()
{
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerKeyBoard, this);
}

void HRocker::stopKeyRocker()
{
	Director::getInstance()->getEventDispatcher()->removeEventListener(listenerKeyBoard);
}
