#include "HRocker.h"
#include "Hero.h"

//��ʼ����
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
	//���ҡ�˱���
	this->setAnchorPoint(Vec2(0, 0));//������һ��ê��
	auto spRockerBg = Sprite::create(rockerBGImageName);
	spRockerBg->setVisible(false);
	spRockerBg->setAnchorPoint(Vec2(0.5f, 0.5f));
	spRockerBg->setPosition(position);
	addChild(spRockerBg, 0,tag_rockerBG);
	//���ҡ�˵��ƶ��ĵ�
	auto spRocker = Sprite::create(rockerImageName);
	spRocker->setVisible(false);
	spRocker->setAnchorPoint(Vec2(0.5f,0.5f));
	spRocker->setScale(1.1);
	spRocker->setPosition(position);
	spRocker->setScale(0.5);
	addChild(spRocker, 1, tag_rocker);
	spRocker->setOpacity(180);//͸����
	//ҡ�˱���ͼ����
	rockerBGPosition = position;
	//ҡ�˱����뾶
	rockerBG_r = spRocker->getContentSize().width*0.5;//ʹ֮����ͼ���С�仯���ı�
	listener = EventListenerTouchOneByOne::create();
	listenerKeyBoard = EventListenerKeyboard::create();
	//�������¼�
	listener->onTouchBegan = CC_CALLBACK_2(HRocker::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(HRocker::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(HRocker::onTouchEnded,this);

	listenerKeyBoard->onKeyPressed = CC_CALLBACK_2(HRocker::onKeyPressed, this);
	listenerKeyBoard->onKeyReleased = CC_CALLBACK_2(HRocker::onKeyReleased, this);
	//listener->setSwallowTouches(true);//���ﻹ�ǲ����̲�
	//������С
	scale = 0.5;
	spRockerBg->setScale(scale);
	spRocker->setScale(scale);
	rockerBG_r = rockerBG_r * scale;//�����Ÿı�
	this->hero = hero;
	log("initrocker");
}

//�ƶ�ƽ̨��
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
	//������߱߳�
	float x = p2_x - p1_x;
	float y = p2_y - p1_y;
	//���������
	float distance = sqrt(x * x + y * y);
	//�����Һ���
	float rad = acos(x / distance);    //[0,pi]
	//�����Ƕ�
	if (p1_y > p2_y)
	{
		rad = -rad;
	}
	return rad;
}

bool HRocker::onTouchBegan(Touch * touch, Event * event)
{
	auto sp = static_cast <Sprite*> (getChildByTag(tag_rocker));
	//�õ�����������
	Point point = touch->getLocation();
	//�ж��Ƿ񵥻���sp�������
	if (sp->boundingBox().containsPoint(point))//�жϰ��������
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
	//�õ�����������
	Point point = touch->getLocation();
	dx = point.x - rockerBGPosition.x;
	dy = point.y - rockerBGPosition.y;
	if (sqrt(pow(dx, 2) + pow(dy, 2)) >= rockerBG_r)
	{
		//�õ�������ҡ�˱���Բ�ĵļн�
		float angle = getRad(rockerBGPosition, point);
		//ȷ��СԲ�˶���Χ�ڱ�����
		dx = rockerBG_r * cos(angle);
		dy = rockerBG_r * sin(angle);
		sp->setPosition(ccpAdd(getAnglePosition(rockerBG_r, angle), ccp(rockerBGPosition.x, rockerBGPosition.y)));
	}
	else
	{
		//�����ڱ������˶�
		sp->setPosition(point);
	}
	dx = dx/ rockerBG_r;
	dy = dy/ rockerBG_r;//ÿ�ε�������ƶ�һ������
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
	rocker->runAction(MoveTo::create(0.08, rockerBG->getPosition()));  //�ص�Բ��
	Enabled = false;
	dx = 0;
	dy = 0;
}

//PCƽ̨��
void HRocker::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)//����
{
	log("key!");
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:
		dy += 1; //ÿ�ε����ƶ�һ������
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

void HRocker::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)//�ɿ�
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

//��������ҡ�˵�����:
void HRocker::startTouchRocker()
{
	//�������ÿɼ������ü���
	Sprite* rocker = static_cast <Sprite*> (getChildByTag(tag_rocker));
	rocker->setVisible(true);
	Sprite* rockerBG = static_cast <Sprite*> (getChildByTag(tag_rockerBG));
	rockerBG->setVisible(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void HRocker::stopTouchRocker()
{
	//�������ò��ɼ���ȡ������
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
