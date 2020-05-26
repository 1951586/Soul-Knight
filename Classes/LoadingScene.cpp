#include "HomeScene.h"
#include "LoadingScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;
Scene* LoadingScene::createScene()
{
    return LoadingScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in LoadingScene.cpp\n");
}

// on "init" you need to initialize your instance
bool LoadingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //���ñ���ͼƬ 
    auto backMap = Sprite::create("Map/StartScene.png");//�ǵ�Ҫдһ��·��
    if (backMap == nullptr)
    {
        problemLoading("'StartScene.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        backMap->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(backMap, 0);
    }
    //���ñ�������
    auto audio = SimpleAudioEngine::getInstance();
    audio->playBackgroundMusic("Music/Forest.mp3", true); // set the background music and continuously play it.
    //���ò˵�
    //�˵���
    auto startItem = MenuItemImage::create("Map/start_game.png", "Map/start_game.png",
        [&](Ref* sender) {
            auto nextScene = HomeScene::createScene();
            Director::getInstance()->pushScene(nextScene);//�л���һ������
        });
    if (startItem == nullptr ||
        startItem->getContentSize().width <= 0 ||
        startItem->getContentSize().height <= 0)
    {
        problemLoading("'start_game.png' and 'start_game.png'");
    }
    else
    {
        float x = origin.x + 40;
        float y = origin.y + 400;
        startItem->setAnchorPoint(Vec2(0, 0));
        startItem->setPosition(Vec2(x, y));
    }
    auto endItem = MenuItemImage::create("Map/end_game.png", "Map/end_game.png",
        [&](Ref* sender) {
            Director::getInstance()->end();//��������
        });
    if (endItem == nullptr ||
        endItem->getContentSize().width <= 0 ||
        endItem->getContentSize().height <= 0)
    {
        problemLoading("'end_game.png' and 'end_game.png'");
    }
    else
    {
        float x = origin.x + 40;
        float y = origin.y + 300;
        endItem->setAnchorPoint(Vec2(0, 0));
        endItem->setPosition(Vec2(x, y));
    }
    //�˵�
    auto menu = Menu::create(startItem, endItem, nullptr);
    menu->setPosition(Vec2::ZERO);//���ã�
    this->addChild(menu, 1);
   

    return true;
}

void LoadingScene::callBack(cocos2d::Ref* pSender)
{

}