#include "MyTransitionScene.h"

MyTransitionScene * MyTransitionScene::createMyTransitionScene(Hero * _hero)
{
	MyTransitionScene* scene =new MyTransitionScene();
	if (scene&&scene->init())
	{
		scene->autorelease();//自动清理内存
		scene->initMyTransitionScene(_hero);
		return scene;
	}
	CC_SAFE_DELETE(scene);
	return nullptr;
}



void MyTransitionScene::initMyTransitionScene(Hero * _hero)
{
	//获取窗口大小
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//添加图层
	addLayer();
	addHero(_hero);

	auto transfer = Sprite::create("map/Transfer.png");
	transfer->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 100);
	transfer->setScale(1.5);
	this->addChild(transfer);

	auto* m_frameCache = SpriteFrameCache::getInstance();
	m_frameCache->addSpriteFramesWithFile("map/TransferDoor.plist", "map/TransferDoor.png");//使用图集
	Vector<SpriteFrame*> frameArray;
	for (int i = 1; i <= 4; i++)
	{
		auto* frame = m_frameCache->getSpriteFrameByName(String::createWithFormat("Transfer%d.png", i)->getCString());
		frameArray.pushBack(frame);
	}
	auto animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(-1);
	animation->setDelayPerUnit(0.08f);
	auto animate = Animate::create(animation);
	transfer->runAction(animate);

	p[0] = Point(visibleSize.width / 2,225);
	p[1] = Point(visibleSize.width / 2 - 200, 225);
	p[2] = Point(visibleSize.width / 2 + 200, 225);

	addHero(_hero);

/*	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch * touch, Event * event) { return true; };
	listener->onTouchMoved = [](Touch * touch, Event * event) {return true; };
	listener->onTouchEnded = [](Touch * touch, Event * event) {
		Point pt = touch->getLocation();
		log("%f %f ", pt.x, pt.y);
		return true; };
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);*/   //查找坐标
	
	
	//加上按钮
	srand((unsigned)time(0));
	int a[3] = { 0 };
	for (int i = 0; i < 3; )
	{
		bool judge = true;
		int tmp = rand() % 5 + 1;
		for (int j = 0; j < 3; j++)
		{
			if (a[j] == tmp)
				judge = false;
		}
		if (!judge)
			continue;
		if (tmp == 1)
		{
			auto button1 = ui::Button::create("talent/button1.png");
			button1->setScale(0.8);
			button1->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
				SimpleAudioEngine::sharedEngine()->playEffect("music/talent.mp3");
				if (hero->goldCoin > 6)
				{
					hero->goldCoin -= 6;
					hero->lifeNumberMax += 3;
				}
				button1->setTouchEnabled(false);  //防止多次点击
				button1->setVisible(false);
				//切换场景完善
				auto nextScene = FightScene1::createFightScene(hero);
				Director::getInstance()->replaceScene(nextScene);
			});
			button1->setPosition(p[i]);
			this->addChild(button1);
		}
		if (tmp == 2)
		{
			auto button2 = ui::Button::create("talent/button2.png");
			button2->setScale(0.8);
			button2->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
				SimpleAudioEngine::sharedEngine()->playEffect("music/talent.mp3");
				if (hero->goldCoin > 7)
				{
					hero->goldCoin -= 7;
					hero->powerNumberMax += 100;
				}
				button2->setTouchEnabled(false);  //防止多次点击
				button2->setVisible(false);
				//切换场景完善
				auto nextScene = FightScene1::createFightScene(hero);
				Director::getInstance()->replaceScene(nextScene);
			});
			button2->setPosition(p[i]);
			this->addChild(button2);
		}
		if (tmp == 3)
		{
			auto button3 = ui::Button::create("talent/button3.png");
			button3->setScale(0.8);
			button3->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
				SimpleAudioEngine::sharedEngine()->playEffect("music/talent.mp3");
				if (hero->goldCoin > 6)
				{
					hero->goldCoin -= 6;
					hero->k = _hero->k*0.8;
				}
				button3->setTouchEnabled(false);  //防止多次点击
				button3->setVisible(false);
				//切换场景完善
				auto nextScene = FightScene1::createFightScene(hero);
				Director::getInstance()->replaceScene(nextScene);
			});
			button3->setPosition(p[i]);
			this->addChild(button3);
		}
		if (tmp == 4)
		{
			auto button4 = ui::Button::create("talent/button4.png");
			button4->setScale(0.8);
			button4->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
				SimpleAudioEngine::sharedEngine()->playEffect("music/talent.mp3"); 
				if (hero->goldCoin > 5)
				{
					hero->goldCoin -= 5;
					hero->weaponNumberMax += 1;
				}
				button4->setTouchEnabled(false);  //防止多次点击
				button4->setVisible(false);
				//切换场景完善
				auto nextScene = FightScene1::createFightScene(hero);
				Director::getInstance()->replaceScene(nextScene);
			});
			button4->setPosition(p[i]);
			this->addChild(button4);
		}
		if (tmp == 5)
		{
			auto button5 = ui::Button::create("talent/button5.png");
			button5->setScale(0.8);
			button5->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
				SimpleAudioEngine::sharedEngine()->playEffect("music/talent.mp3"); 
				if (hero->goldCoin > 4)
				{
					hero->goldCoin -= 4;
					hero->velocity += 2;
				}
				button5->setTouchEnabled(false);  //防止多次点击
				button5->setVisible(false);
				//切换场景完善
				auto nextScene = FightScene1::createFightScene(hero);
				Director::getInstance()->replaceScene(nextScene);
			});
			button5->setPosition(p[i]);
			this->addChild(button5);
		}
		if (tmp == 6)
		{
			auto button6 = ui::Button::create("talent/button6.png");
			button6->setScale(0.8);
			button6->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
				SimpleAudioEngine::sharedEngine()->playEffect("music/talent.mp3"); 
				if (hero->goldCoin > 8)
				{
					hero->goldCoin -= 8;
					hero->shieldNumberMax += 2;
				}
				button6->setTouchEnabled(false);  //防止多次点击
				button6->setVisible(false);
				//切换场景完善
				auto nextScene = FightScene1::createFightScene(hero);
				Director::getInstance()->replaceScene(nextScene);
			});
			button6->setPosition(p[i]);
			this->addChild(button6);
		}
		i++;
		a[i] = tmp;
	}
	//addButtom();
}
/*
void MyTransitionScene::addHero(Hero * _hero)
{
	next = FightScene1::createFightScene(_hero);
	hero = next->hero;
}
*/
