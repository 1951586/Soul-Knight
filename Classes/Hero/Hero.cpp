#include<cmath>
#include<algorithm>
#include<sstream>
#include<string>
#include "Hero.h"
#include"HRocker.h"
USING_NS_CC;

bool Hero::init()
{
	if (!Layer::init())//判断是否初始化成功
	{
		return false;
	}
	return true;
}

void Hero::moveBy(double dx, double dy)//移动到某位置
{
	//获取运动图层
	auto layer = static_cast<Layer*>(scene->getChildByTag(-3));
	if (dx == 0 && dy == 0)
	{
		if (withScene)
		{
			//判断是否位于窗口中心
			float x = position.x + layer->getPositionX();//获取相对窗口坐标
			float y = position.y + layer->getPositionY();
			log("%f %f", layer->getPositionX(), layer->getPositionY());
			if (fabs(x - 600) > 8 || fabs(y - 400) > 8)//浮点数相等的范围判断：避免抖动
			{
				float r = sqrt(pow(x - 600, 2) + pow(y - 400, 2));
				float _dx = x - 600;
				float _dy = y - 400;
				//场景相对屏幕动
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
			//人物相对场景动
			float x = this->getPositionX();
			float y = this->getPositionY();
			this->setPosition(x + dx, y + dy);
			if (withScene)
			{
				//场景相对屏幕动
				double x_ = layer->getPositionX();
				double y_ = layer->getPositionY();
				layer->setPosition(x_ - dx, y_ - dy);
			}
		}
		else
		{
			if (withScene)
			{
				float x = position.x + layer->getPositionX();//获取相对窗口坐标
				float y = position.y + layer->getPositionY();
				int dis = sqrt(pow(fabs(x - 600), 2) + pow(fabs(y - 400), 2));//获取到中心点的距离
				if(dis < 100)
				{
					//场景还是相对屏幕动，只是慢一些
					double x_ = layer->getPositionX();
					double y_ = layer->getPositionY();
					layer->setPosition(x_ - dx / 4, y_ - dy / 4);
				}
			}
		}
	}
	//修改坐标
	position.x = sprite->getPositionX() * scale + this->getPositionX();
	position.y = sprite->getPositionY() * scale + this->getPositionY();
}

void Hero::setAction(int status)
{
	if (status == 1 || status == 3)
	{
		//抖动代码：
		sprite->stopActionByTag(99);
		sprite->setAnchorPoint(Vec2(0.5, 0));
		auto scaleTo1 = ScaleTo::create(0.3, 0.99, 1.03);
		auto scaleTo2 = ScaleTo::create(0.3, 1.01, 0.98);
		auto action = RepeatForever::create(Sequence::create(scaleTo1, scaleTo2, NULL)->clone());//抖动
		action->setTag(99);
		sprite->runAction(action);
		//走动动画：
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
	//判断代码：
	//判断代码：
	/* 加载meta层 */
	//meta = map->layerNamed("meta");
	//auto meta = map->getLayer("meta");
	//meta->setVisible(true);
	/* -----------------判断是否不可通行---------------- */
	/* 获得当前主角在地图中的格子位置 */
	//CCPoint tiledPos = tileCoordForPosition(ccp(dx, dy));
	//Point tiledPos = Point(dx, dy);

	/* 获取地图格子的唯一标识 */
	//int tiledGid = meta->tileGIDAt(tiledPos);

	/* 不为0，代表存在这个格子 */
	//if (tiledGid != 0) {
		/*
			获取该地图格子的所有属性，目前我们只有一个Collidable属性,
			格子是属于meta层的，但同时也是属于整个地图的，所以在获取格子的所有属性时，
			通过格子唯一标识在地图中取得。
		*/
		//auto propertiesDict = map->propertiesForGID(tiledGid);
		//log("value:%s", propertiesDict.getDescription().c_str());

		//以字符串形式获取propertiesDict中的内容
		//auto propertiesStr = propertiesDict.getDescription();

		//if (tiledGid)
		//{
			//判断propertiesDict是否为空
		//	if (!propertiesDict.isNull())
			//{
				//遇到障碍物，则不让精灵移动
			//	if (propertiesStr.find("Collidable") != -1)
				//{
					//取得特定字符串
			//		auto CollidableValue = propertiesDict.asValueMap().at("Collidable").asString().c_str();
				//	auto strTrue = "True";
				//	log("%s", CollidableValue);

					//判断取出的特定字符串是否与预期相等
			//		if (CollidableValue && strcmp(CollidableValue, strTrue) == 0)//阻碍了
			//		{
			//			return false;
			//		}
			//	}

			//}
		//}

		//auto propertiesDict=CC_PROPERTY(CCDictionary*, tiledGid, Properties);
		/* 取得格子的Collidable属性值 */
		//const CCString* prop = propertiesDict.valueForKey("Collidable");

		/* 判断Collidable属性是否为true,是的话，不让玩家移动 */
		/*if (prop) {
			return false;
		}*/
	//}
	//获取运动图层
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

void Hero::myUpdate(float dt)//定时器
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
	dy = dy*velocity;//控制速度
	if (dx != 0 || dy != 0)
	{
		dx_ = dx, dy_ = dy;
	}
	//产生走动动画
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
	if (isCanMove(dx, dy))//判断能否移动
	{
		canMove = true;
	}
	else
	{
		canMove = false;
	}
	moveBy(dx, dy);//移动
	//实现技能间隔
	if (!isAbility)
	{
		count++;
		if (count == abilityInterval)
		{
			isAbility = true;
			count = 0;
		}
	}
	//实现信息更新
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
//显示人物信息
void Hero::showInformation()
{
	//获取静止图层
	auto layer = static_cast<Layer*>(scene->getChildByTag(-2));
	if (!layer->isVisible())
	{
		layer->setVisible(true);
		isShow = true;
	}
}

void Hero::hideInformation()
{
	//获取静止图层
	auto layer = static_cast<Layer*>(scene->getChildByTag(-2));
	if (layer->isVisible())
	{
		layer->setVisible(false);
		isShow = false;
	}
}

void Hero::initInformation()
{
	//获取静止图层
	auto layer = static_cast<Layer*>(scene->getChildByTag(-2));
	//显示板
	auto board = Sprite::create("Map/Information.png");
	board->setAnchorPoint(Vec2(0, 1));
	board->setPosition(Vec2(0, 800));
	layer->addChild(board);
	Sprite* background[3];
	//滑动条背景
	for (int i = 0; i < 3; i++)
	{
		int distance = i * 33;
		background[i] = Sprite::create("Map/BG.png");
		background[i]->setAnchorPoint(Vec2(0, 0.5));
		background[i]->setPosition(Vec2(44, 780 - distance));
		layer->addChild(background[i]);
	}
	//设置血量条等
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
		progress[i]->setMidpoint(Vec2(0, 0.5));//从右到左减少
		progress[i]->setBarChangeRate(Vec2(1, 0));//只改变水平方向
		progress[i]->setAnchorPoint(Vec2(0, 0.5));
		progress[i]->setPosition(Vec2(45, 780 - distance));
		layer->addChild(progress[i]);
		progress[i]->setTag(i);
		progress[i]->setPercentage(100);
	}
	//设置标签
	std::ostringstream msg[3];//标签内容
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
	//设置金币袋
	auto pocket = Sprite::create("Map/pocket.png");
	pocket->setAnchorPoint(Vec2(0.5, 0.5));
	pocket->setPosition(Vec2(1040, 780));
	pocket->setScale(0.7);
	layer->addChild(pocket);
	std::ostringstream goldMsg;//标签内容
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
	//获取静止图层
	auto layer = static_cast<Layer*>(scene->getChildByTag(-2));
	//改变进度条
	static_cast<ProgressTimer*>(layer->getChildByTag(0))->setPercentage((((float)lifeNumberNow) / lifeNumberMax) * 100);
	static_cast<ProgressTimer*>(layer->getChildByTag(1))->setPercentage((((float)shieldNumberNow) / shieldNumberMax) * 100);
	static_cast<ProgressTimer*>(layer->getChildByTag(2))->setPercentage((((float)powerNumberNow) / powerNumberMax) * 100);
	//标签内容
	std::ostringstream msg[4];
	msg[0] << lifeNumberNow << "/" << lifeNumberMax;
	msg[1] << shieldNumberNow << "/" << shieldNumberMax;
	msg[2] << powerNumberNow << "/" << powerNumberMax;
	msg[3] << goldCoin;
	//改变标签
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