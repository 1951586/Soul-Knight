#include<cmath>
#include<algorithm>
#include<sstream>
#include<string>
#include "Hero.h"
#include"HRocker.h"
USING_NS_CC;

bool Hero::init()
{
	if (!Layer::init())//�ж��Ƿ��ʼ���ɹ�
	{
		return false;
	}
	return true;
}

void Hero::moveBy(double dx, double dy)//�ƶ���ĳλ��
{
	//��ȡ�˶�ͼ��
	auto layer = static_cast<Layer*>(scene->getChildByTag(-3));
	if (dx == 0 && dy == 0)
	{
		if (withScene)
		{
			//�ж��Ƿ�λ�ڴ�������
			float x = position.x + layer->getPositionX();//��ȡ��Դ�������
			float y = position.y + layer->getPositionY();
			log("%f %f", layer->getPositionX(), layer->getPositionY());
			if (fabs(x - 600) > 8 || fabs(y - 400) > 8)//��������ȵķ�Χ�жϣ����ⶶ��
			{
				float r = sqrt(pow(x - 600, 2) + pow(y - 400, 2));
				float _dx = x - 600;
				float _dy = y - 400;
				//���������Ļ��
				double x_ = layer->getPositionX();
				double y_ = layer->getPositionY();
				layer->setPosition(x_ - 8 * _dx / r, y_ - 8 * _dy / r);
			}
		}
	}
	else
	{
		if (canMove)
		{
			//������Գ�����
			float x = this->getPositionX();
			float y = this->getPositionY();
			this->setPosition(x + dx, y + dy);
			if (withScene)
			{
				//���������Ļ��
				double x_ = layer->getPositionX();
				double y_ = layer->getPositionY();
				layer->setPosition(x_ - dx, y_ - dy);
			}
		}
		else
		{
			if (withScene)
			{
				float x = position.x + layer->getPositionX();//��ȡ��Դ�������
				float y = position.y + layer->getPositionY();
				int dis = sqrt(pow(fabs(x - 600), 2) + pow(fabs(y - 400), 2));//��ȡ�����ĵ�ľ���
				if(dis < 100)
				{
					//�������������Ļ����ֻ����һЩ
					double x_ = layer->getPositionX();
					double y_ = layer->getPositionY();
					layer->setPosition(x_ - dx / 4, y_ - dy / 4);
				}
			}
		}
	}
	//�޸�����
	position.x = sprite->getPositionX() * scale + this->getPositionX();
	position.y = sprite->getPositionY() * scale + this->getPositionY();
}

void Hero::setAction(int status)
{
	if (status == 1 || status == 3)
	{
		//�������룺
		sprite->stopActionByTag(99);
		sprite->setAnchorPoint(Vec2(0.5, 0));
		auto scaleTo1 = ScaleTo::create(0.3, 0.99, 1.03);
		auto scaleTo2 = ScaleTo::create(0.3, 1.01, 0.98);
		auto action = RepeatForever::create(Sequence::create(scaleTo1, scaleTo2, NULL)->clone());//����
		action->setTag(99);
		sprite->runAction(action);
		//�߶�������
		sprite->stopActionByTag(100);
		auto* animate = createAnimate(status);
		animate->setTag(100);
		sprite->runAction(animate);
	}
	else
	{
		sprite->stopActionByTag(99);
		sprite->stopActionByTag(100);
		auto* animate = createAnimate(status);
		animate->setTag(100);
		sprite->runAction(animate);
	}
}

bool Hero::isCanMove(int dx,int dy)
{
	//�жϴ��룺
	//�жϴ��룺
	/* ����meta�� */
	//meta = map->layerNamed("meta");
	//auto meta = map->getLayer("meta");
	//meta->setVisible(true);
	/* -----------------�ж��Ƿ񲻿�ͨ��---------------- */
	/* ��õ�ǰ�����ڵ�ͼ�еĸ���λ�� */
	//CCPoint tiledPos = tileCoordForPosition(ccp(dx, dy));
	//Point tiledPos = Point(dx, dy);

	/* ��ȡ��ͼ���ӵ�Ψһ��ʶ */
	//int tiledGid = meta->tileGIDAt(tiledPos);

	/* ��Ϊ0���������������� */
	//if (tiledGid != 0) {
		/*
			��ȡ�õ�ͼ���ӵ��������ԣ�Ŀǰ����ֻ��һ��Collidable����,
			����������meta��ģ���ͬʱҲ������������ͼ�ģ������ڻ�ȡ���ӵ���������ʱ��
			ͨ������Ψһ��ʶ�ڵ�ͼ��ȡ�á�
		*/
		//auto propertiesDict = map->propertiesForGID(tiledGid);
		//log("value:%s", propertiesDict.getDescription().c_str());

		//���ַ�����ʽ��ȡpropertiesDict�е�����
		//auto propertiesStr = propertiesDict.getDescription();

		//if (tiledGid)
		//{
			//�ж�propertiesDict�Ƿ�Ϊ��
		//	if (!propertiesDict.isNull())
			//{
				//�����ϰ�����þ����ƶ�
			//	if (propertiesStr.find("Collidable") != -1)
				//{
					//ȡ���ض��ַ���
			//		auto CollidableValue = propertiesDict.asValueMap().at("Collidable").asString().c_str();
				//	auto strTrue = "True";
				//	log("%s", CollidableValue);

					//�ж�ȡ�����ض��ַ����Ƿ���Ԥ�����
			//		if (CollidableValue && strcmp(CollidableValue, strTrue) == 0)//�谭��
			//		{
			//			return false;
			//		}
			//	}

			//}
		//}

		//auto propertiesDict=CC_PROPERTY(CCDictionary*, tiledGid, Properties);
		/* ȡ�ø��ӵ�Collidable����ֵ */
		//const CCString* prop = propertiesDict.valueForKey("Collidable");

		/* �ж�Collidable�����Ƿ�Ϊtrue,�ǵĻ�����������ƶ� */
		/*if (prop) {
			return false;
		}*/
	//}
	//��ȡ�˶�ͼ��
	auto layer = static_cast<Layer*>(scene->getChildByTag(-3));
	auto sprite= static_cast<Sprite*>(layer->getChildByTag(5));
	if (sprite->boundingBox().containsPoint(Point(position.x+dx,position.y+dy)))
	{
		log("True");
		//Director::getInstance()->popScene();
		//hero->canMove = false;
		return false;
	}
	return true;
}

void Hero::myUpdate(float dt)//��ʱ��
{
	//log("%f %f",position.x,position.y);
	dx = rocker->dx;
	dy = rocker->dy;
	if (pow(rocker->dx, 2) + pow(rocker->dy, 2) == 2)
	{
		dx = rocker->dx / sqrt(2);
	    dy = rocker->dy / sqrt(2);
	}
	dx = dx*velocity;
	dy = dy*velocity;//�����ٶ�
	if (dx != 0 || dy != 0)
	{
		dx_ = dx, dy_ = dy;
	}
	//�����߶�����
	if (dx == 0 && dy == 0 && isRun != 0)
	{
		if (faceDirection == 1)
			setAction(1);
		else
			setAction(3);
		isRun = 0;
	}
	else if (dx > 0 && isRun != 1)
	{
		setAction(2);
		isRun = 1;
		faceDirection = 1;
	}
	else if (dx < 0 && isRun != -1)
	{
		setAction(4);
		isRun = -1;
		faceDirection = -1;
	}
	else if (dx == 0 && dy != 0 && isRun != faceDirection)
	{
		if (faceDirection == 1)
			setAction(2);
		else
			setAction(4);
		isRun = faceDirection;
	}
	if (isCanMove(dx, dy))//�ж��ܷ��ƶ�
	{
		canMove = true;
	}
	else
	{
		canMove = false;
	}
	moveBy(dx, dy);//�ƶ�
	//ʵ�ּ��ܼ��
	if (!isAbility)
	{
		count++;
		if (count == abilityInterval)
		{
			isAbility = true;
			count = 0;
		}
	}
	//ʵ����Ϣ����
	if (isShow)
	{
		changeInformation();
	}
}

void Hero::startKeyRocker()
{
	rocker->startKeyRocker();
}

void Hero::stopKeyRocker()
{
	rocker->stopKeyRocker();
}

void Hero::startTouchRocker()
{
	rocker->startTouchRocker();
}

void Hero::stopTouchRocker()
{
	rocker->stopTouchRocker();
}
//��ʾ������Ϣ
void Hero::showInformation()
{
	//��ȡ��ֹͼ��
	auto layer = static_cast<Layer*>(scene->getChildByTag(-2));
	if (!layer->isVisible())
	{
		layer->setVisible(true);
		isShow = true;
	}
}

void Hero::hideInformation()
{
	//��ȡ��ֹͼ��
	auto layer = static_cast<Layer*>(scene->getChildByTag(-2));
	if (layer->isVisible())
	{
		layer->setVisible(false);
		isShow = false;
	}
}

void Hero::initInformation()
{
	//��ȡ��ֹͼ��
	auto layer = static_cast<Layer*>(scene->getChildByTag(-2));
	//��ʾ��
	auto board = Sprite::create("Map/Information.png");
	board->setAnchorPoint(Vec2(0, 1));
	board->setPosition(Vec2(0, 800));
	layer->addChild(board);
	Sprite* background[3];
	//����������
	for (int i = 0; i < 3; i++)
	{
		int distance = i * 33;
		background[i] = Sprite::create("Map/BG.png");
		background[i]->setAnchorPoint(Vec2(0, 0.5));
		background[i]->setPosition(Vec2(44, 780 - distance));
		layer->addChild(background[i]);
	}
	//����Ѫ������
	ProgressTimer* progress[3];
	auto HP = Sprite::create("Map/HP.png");
	progress[0] = ProgressTimer::create(HP);
	auto shield = Sprite::create("Map/shield.png");
	progress[1] = ProgressTimer::create(shield);
	auto MP = Sprite::create("Map/MP.png");
	progress[2] = ProgressTimer::create(MP);
	for (int i = 0; i < 3; i++)
	{
		float distance = i * 33.7;
		progress[i]->setType(ProgressTimer::Type::BAR);
		progress[i]->setMidpoint(Vec2(0, 0.5));//���ҵ������
		progress[i]->setBarChangeRate(Vec2(1, 0));//ֻ�ı�ˮƽ����
		progress[i]->setAnchorPoint(Vec2(0, 0.5));
		progress[i]->setPosition(Vec2(45, 780 - distance));
		layer->addChild(progress[i]);
		progress[i]->setTag(i);
		progress[i]->setPercentage(100);
	}
	//���ñ�ǩ
	std::ostringstream msg[3];//��ǩ����
	msg[0] << lifeNumberNow << "/" << lifeNumberMax;
	msg[1] << shieldNumberNow << "/" << shieldNumberMax;
	msg[2] << powerNumberNow << "/" << powerNumberMax;
	Label* label[3];
	for (int i = 0; i < 3; i++)
	{
		float distance = i * 34;
		label[i] = Label::createWithTTF(msg[i].str().c_str(), "fonts/arial.ttf", 24);
		label[i]->setAnchorPoint(Vec2(0.5, 0.5));
		label[i]->setPosition(Vec2(128, 780 - distance));
		layer->addChild(label[i]);
		label[i]->setTag(i + 3);
	}
	//���ý�Ҵ�
	auto pocket = Sprite::create("Map/pocket.png");
	pocket->setAnchorPoint(Vec2(0.5, 0.5));
	pocket->setPosition(Vec2(1040, 780));
	pocket->setScale(0.7);
	layer->addChild(pocket);
	std::ostringstream goldMsg;//��ǩ����
	goldMsg << goldCoin;
	auto coinLabel = Label::createWithTTF(goldMsg.str().c_str(), "fonts/arial.ttf", 24);
	coinLabel->setAnchorPoint(Vec2(0, 0.5));
	coinLabel->setPosition(Vec2(1060, 780));
	layer->addChild(coinLabel);
	coinLabel->setTag(6);
	layer->setVisible(false);
	isShow = false;
}

void Hero::changeInformation()
{
	//��ȡ��ֹͼ��
	auto layer = static_cast<Layer*>(scene->getChildByTag(-2));
	//�ı������
	static_cast<ProgressTimer*>(layer->getChildByTag(0))->setPercentage((((float)lifeNumberNow) / lifeNumberMax) * 100);
	static_cast<ProgressTimer*>(layer->getChildByTag(1))->setPercentage((((float)shieldNumberNow) / shieldNumberMax) * 100);
	static_cast<ProgressTimer*>(layer->getChildByTag(2))->setPercentage((((float)powerNumberNow) / powerNumberMax) * 100);
	//��ǩ����
	std::ostringstream msg[4];
	msg[0] << lifeNumberNow << "/" << lifeNumberMax;
	msg[1] << shieldNumberNow << "/" << shieldNumberMax;
	msg[2] << powerNumberNow << "/" << powerNumberMax;
	msg[3] << goldCoin;
	//�ı��ǩ
	static_cast<Label*>(layer->getChildByTag(3))->setString(msg[0].str());
	static_cast<Label*>(layer->getChildByTag(4))->setString(msg[1].str());
	static_cast<Label*>(layer->getChildByTag(5))->setString(msg[2].str());
	static_cast<Label*>(layer->getChildByTag(6))->setString(msg[3].str());
}

void Hero::commonDataInit()
{
	dx = 0, dy = 0;
	dx_ = 0, dy_ = 0;
	faceDirection = 1;
	isRun = 0;
	isAbility = true;
	count = 0;
	canMove = true;
	withScene = false;
}

void Hero::moveWithScene()
{
	withScene = true;
}

void Hero::moveWithoutScene()
{
	withScene = false;
}