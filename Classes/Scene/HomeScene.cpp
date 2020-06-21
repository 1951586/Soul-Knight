#include "Scene/HomeScene.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Hero/Knight.h"
#include "Hero/Ranger.h"
#include "Hero/Sorcerer.h"
#include "Monster/SnowMan.h"
#include "Weapon/weapon.h"
#include "Hero/Hero.h"
#include "Scene/FightScene1.h"
#include "Scene/FightScene2.h"
#include "Scene/FightScene3.h"
#include "Scene/LoadingScene.h"
#include "Scene/FightScene4.h"
#include "Scene/FightScene5.h"
#include "Scene/MyTransitionScene.h"


using namespace CocosDenshion;

USING_NS_CC;

FightScene* HomeScene::createScene()
{
    return HomeScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HomeScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }
    //添加图层
    addLayer();
    //获取窗口大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //设置背景图片 
    //auto homeMap = Sprite::create("Map/Home.png");//记得要改一下路径
    auto homeMap = TMXTiledMap::create("Tmx/homescene.tmx");
    if (homeMap == nullptr)
    {
        problemLoading("'Map/Home.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        //homeMap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        homeMap->setPosition(Vec2(0, 0));
        // add the sprite as a child to this layer
        dynamicLayer->addChild(homeMap, 0);
    }
    //设置背景音乐
    auto audio = SimpleAudioEngine::getInstance();
    audio->playBackgroundMusic("Music/Home.mp3", true); // set the background music and continuously play it.
    //设置回退按钮
    auto turnBack = ui::Button::create("Map/turn_back.png", "Map/turn_back.png", "Map/turn_back.png");
    turnBack->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        auto audio = SimpleAudioEngine::getInstance();//定义变量须放在switch外面
        auto prevScene = LoadingScene::createScene();
        switch (type)
        {
        case ui::Widget::TouchEventType::BEGAN:
            audio->playBackgroundMusic("Music/Forest.mp3");
            Director::getInstance()->replaceScene(prevScene);//返回LoadingScene
			hadChosen = false;
            break;
        default:
            break;
        }
        });
    turnBack->setPosition(Vec2(970, 10));
    turnBack->setAnchorPoint(Vec2(0, 0));
    dynamicLayer->addChild(turnBack,1);
    //设置传送门
    Transfer = Sprite::create("Map/door.png");
	Transfer->setPosition(Vec2(605, 710));
    dynamicLayer->addChild(Transfer, 2);
    auto moveRepeat = MoveBy::create(1.0, Vec2(0, 6));//设置动作
	Transfer->runAction(RepeatForever::create(Sequence::create(moveRepeat, moveRepeat->reverse(), NULL)));//永远执行，来回抖动
    Transfer->setTag(6);
    //设置菜单隐藏项
    auto transParentPicture = Sprite::create("Map/transparent_picture.png");
    transParentPicture->setPosition(Vec2(100, 580));
    transParentPicture ->setAnchorPoint(Vec2(0, 1));
    transParentPicture->setVisible(false);
    //隐藏
    auto hide = CCHide::create();
    transParentPicture->runAction(hide);
    dynamicLayer->addChild(transParentPicture, 3);
    //定义隐藏显现监听器
    auto listener1 = EventListenerCustom::create("hideAndShowEvent", [&](EventCustom* event_)mutable {
        Sprite* sprite = static_cast<Sprite*>(event_->getCurrentTarget());
        log("myself listener");
        auto hide = CCHide::create();
        auto show = CCShow::create();
        if (sprite->isVisible())
            sprite->runAction(hide);
        else
            sprite->runAction(show);
        });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, transParentPicture);
    //控制菜单
    auto intercalateItem= MenuItemImage::create("Map/intercalate.png", "Map/catercalate.png", CC_CALLBACK_1(HomeScene::callBack1, this));
    intercalateItem->setPosition(Vec2(50,550));
    auto menu1 = Menu::create(intercalateItem, nullptr);
    menu1->setPosition(Vec2::ZERO);//
    dynamicLayer->addChild(menu1, 4);
    //音乐菜单
    auto closeMusic = Sprite::create("Map/music_control2.png");
    auto closeMusicItem = MenuItemImage::create("Map/music_control1.png", "Map/music_control1.png",
        [&](cocos2d::Ref* pSender) {
            EventCustom event("closeAndResumeEvent");
            _eventDispatcher->dispatchEvent(&event);
        }
        );
    closeMusicItem->setPosition(Vec2(1150,540));
    closeMusic->setPosition(Vec2(1150, 540));
    auto menu2 = Menu::create(closeMusicItem, nullptr);
    menu2->setPosition(Vec2::ZERO);//
    dynamicLayer->addChild(menu2, 4);
    dynamicLayer->addChild(closeMusic, 5);
    closeMusic->runAction(hide->clone());
    //定义音乐暂停与恢复监听器
    auto listener2 = EventListenerCustom::create("closeAndResumeEvent", [&](EventCustom* event_)mutable {
        Sprite* sprite = static_cast<Sprite*>(event_->getCurrentTarget());
        auto hide = CCHide::create();
        auto show = CCShow::create();
        auto audio = SimpleAudioEngine::getInstance();
        if (audio->isBackgroundMusicPlaying())
        {
            sprite->runAction(show);
            audio->pauseBackgroundMusic();
        }
        else
        {
            sprite->runAction(hide);
            audio->resumeBackgroundMusic();
        }
        });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, closeMusic);
	hadChosen = false;

    //设置精灵

 //   auto Mage = Sprite::create("Map/Mage.png");//法师
	//Mage->setPosition(Vec2(770, 440));
	//Mage->setScale(0.7);
 //   this->addChild(Mage,5);
      //auto Elves = Sprite::create("Map/Elves.png");//精灵
	  //Elves->setPosition(Vec2(318, 620));
	 // Elves->setScale(0.7);
	 // Elves->setAnchorPoint(Vec2(0, 0));
     // this->addChild(Elves,2);
 //   auto Knight = Sprite::create("Map/Knight.png");//骑士
	//Knight->setPosition(Vec2(770, 620));
	//Knight->setScale(0.7);
	//Knight->setAnchorPoint(Vec2(0, 0));
 //   this->addChild(Knight,5);
 //   auto Ranger = Sprite::create("Map/Ranger.png");//游侠
	//Ranger->setPosition(Vec2(1030, 200));
	//Ranger->setScale(0.7);
	//Ranger->setAnchorPoint(Vec2(0, 0));
 //   this->addChild(Ranger, 5);
 //   //模拟抖动动作
 //   auto scaleTo1 = ScaleTo::create(0.3, 0.67, 0.77);
 //   auto scaleTo2 = ScaleTo::create(0.3, 0.72, 0.68);
 //   auto scaleTo3 = ScaleTo::create(0.3, 0.72, 0.73);
	//Mage->runAction(RepeatForever::create(Sequence::create(scaleTo1, scaleTo2, NULL)));//永远执行，来回抖动
	//Elves->runAction(RepeatForever::create(Sequence::create(scaleTo3, scaleTo2, NULL)->clone()));
 //   Knight->runAction(RepeatForever::create(Sequence::create(scaleTo3, scaleTo2, NULL)->clone()));
	//Ranger->runAction(RepeatForever::create(Sequence::create(scaleTo3, scaleTo2, NULL)->clone()));


	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch * touch, Event * event) { return true; };
	listener->onTouchMoved = [](Touch * touch, Event * event) {return true; };
	listener->onTouchEnded = [](Touch * touch, Event * event) {
		Point pt = touch->getLocation();
		log("%f %f ", pt.x, pt.y);
		return true; };
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this );   //查找坐标



	creatHero();
	scheduleUpdate();

    return true;
}

void HomeScene::callBack1(cocos2d::Ref* pSender)
{
    EventCustom event("hideAndShowEvent");
    _eventDispatcher->dispatchEvent(&event);
}

void HomeScene::update(float delta)
{
	startGame();
}

void HomeScene::startGame()
{
	if (hero != nullptr)
	{
        //传送门设置
        srand((unsigned)time(0));
        if (Transfer->boundingBox().containsPoint(hero->position))
		{
			log("True");
            SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
            int num = rand() % 5;
            FightScene* nextScene;
            if (num == 0)
            {
                nextScene = FightScene1::createFightScene(hero);
            }
            else if (num == 1)
            {
                nextScene = FightScene2::createFightScene(hero);
            }
            else if (num == 2)
            {
                nextScene = FightScene3::createFightScene(hero);
            }
            else if (num == 3)
            {
                nextScene = FightScene4::createFightScene(hero);
            }
            else if (num == 4)
            {
                nextScene = FightScene5::createFightScene(hero);
            }
           
            //auto nextScene = FightScene5::createFightScene(hero);
            this->removeAllChildren();//要记得清除上一场景内容
            Director::getInstance()->replaceScene(nextScene);//切换下一个场景
      
		}
        
	}
}

void HomeScene::creatHero()   //选择英雄函数，在此添加
{
	//骑士
	auto knight = ui::Button::create("character/Knight.png");
	knight->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
		if (!hadChosen)
		{
			hero = Knight::createHeroSprite(Point(790, 620),this);//参数是一个Ponit类，表示位置
			hero->startKeyRocker();
            hero->moveWithScene();
           // hero->shieldNumberNow = 2;
            dynamicLayer->addChild(hero,10);
			//hero->startTouchRocker();
            dynamicLayer->removeChild(knight);
			hadChosen = true;
            //显示板
            hero->showInformation();
            //怪物
            auto snowman = Snowman::createMonsterSprite(Point(200, 200),this);
            dynamicLayer->addChild(snowman, 5);
            //snowman->comeOnStage();
            Rect rect = snowman->boundingBox();
            monster.push_back(snowman);
            snowman->getHeroInstance(hero);//“knight”为场景之前加入的Hero的名称
            auto snowman2 = Snowman::createMonsterSprite(Point(300, 300), this);
            dynamicLayer->addChild(snowman2, 5);
            snowman2->comeOnStage();
            monster.push_back(snowman2);
            snowman2->getHeroInstance(hero);//“knight”为场景之前加入的Hero的名称
            auto snowman3 = Snowman::createMonsterSprite(Point(300, 300), this);
            dynamicLayer->addChild(snowman3, 5);
            snowman3->comeOnStage();
            monster.push_back(snowman3);
            snowman3->getHeroInstance(hero);//“knight”为场景之前加入的Hero的名称
            //加武器
            auto weapon2 = Weapon::createWeapon(this, hero, "broadsword");
            hero->changeWeapon(weapon2);
            auto weapon3 = Weapon::createWeapon(this, hero, "fireGun");
            hero->changeWeapon(weapon3);
            
		}
	});
	knight->setScale(0.8);
	knight->setAnchorPoint(Vec2(0.5, 0));
	knight->setPosition(Point(790, 620));
    dynamicLayer->addChild(knight);

	//法师
	auto sorcerer = ui::Button::create("character/sorcerer.png");
	sorcerer->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
		if (!hadChosen)
		{		
			hero = Sorcerer::createHeroSprite(Point(770, 440),this);//参数是一个Ponit类，表示位置
            dynamicLayer->addChild(hero,10);
            hero->showInformation();
			//hero->startTouchRocker();
            hero->startKeyRocker();
            hero->moveWithScene();
            dynamicLayer->removeChild(sorcerer);
			hadChosen = true;
           // hero->lifeNumberNow = 1;
           // hero->powerNumberNow = 100;
           // hero->shieldNumberNow = 1;
            //怪物
            auto snowman = Snowman::createMonsterSprite(Point(200, 200),this);
            dynamicLayer->addChild(snowman, 5);
            snowman->comeOnStage();
            monster.push_back(snowman);
            snowman->getHeroInstance(hero);//“knight”为场景之前加入的Hero的名称
            auto snowman2 = Snowman::createMonsterSprite(Point(300, 300), this);
            dynamicLayer->addChild(snowman2, 5);
            snowman2->comeOnStage();
            monster.push_back(snowman2);
            snowman2->getHeroInstance(hero);//“knight”为场景之前加入的Hero的名称
            auto snowman3 = Snowman::createMonsterSprite(Point(300, 300), this);
            dynamicLayer->addChild(snowman3, 5);
            snowman3->comeOnStage();
            monster.push_back(snowman3);
            snowman3->getHeroInstance(hero);//“knight”为场景之前加入的Hero的名称
             //加武器
            auto weapon1 = Weapon::createWeapon(this, hero, "iceGun");//true表示远程
            hero->changeWeapon(weapon1);
            auto weapon2 = Weapon::createWeapon(this, hero, "machetes");
            hero->changeWeapon(weapon2);
           
		}
	});
	sorcerer->setScale(0.8);
	sorcerer->setAnchorPoint(Vec2(0.5, 0));
	sorcerer->setPosition(Point(770, 440));
    dynamicLayer->addChild(sorcerer);

	//游侠
	auto ranger = ui::Button::create("character/Ranger.png");
	ranger->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
		if (!hadChosen)
		{
			hero = Ranger::createHeroSprite(Point(1030, 200),this);//参数是一个Ponit类，表示位置
            dynamicLayer->addChild(hero,10);
			hero->startKeyRocker();
            hero->showInformation();
            hero->moveWithScene();
            //hero->goldCoin = 100;
           // hero->powerNumberNow = 100;
            //hero->shieldNumberNow = 0;
			//hero->startTouchRocker();
            dynamicLayer->removeChild(ranger);
			hadChosen = true;
            /*
            //怪物
            //auto snowman = Snowman::createMonsterSprite(Point(200, 200),this);
            //dynamicLayer->addChild(snowman, 5);
           // snowman->comeOnStage();
            //monster.push_back(snowman);
            //snowman->getHeroInstance(hero);//“knight”为场景之前加入的Hero的名称
            //auto snowman2 = Snowman::createMonsterSprite(Point(300, 300), this);
            //dynamicLayer->addChild(snowman2, 5);
            //snowman2->comeOnStage();
            //monster.push_back(snowman2);
            //snowman2->getHeroInstance(hero);//“knight”为场景之前加入的Hero的名称
            auto snowman3 = Snowman::createMonsterSprite(Point(300, 300), this);
            dynamicLayer->addChild(snowman3, 5);
            snowman3->comeOnStage();
            monster.push_back(snowman3);
            snowman3->getHeroInstance(hero);//“knight”为场景之前加入的Hero的名称
            */
             //加武器
            auto weapon1 = Weapon::createWeapon(this, hero, "lightsaberBlue");//true表示远程
            hero->changeWeapon(weapon1);
            auto weapon3 = Weapon::createWeapon(this, hero, "rifle");
            hero->changeWeapon(weapon3);
           
		}
	});
	ranger->setScale(0.7);
	ranger->setAnchorPoint(Vec2(0.5, 0));
	ranger->setPosition(Point(1030, 200));
    dynamicLayer->addChild(ranger);

}

