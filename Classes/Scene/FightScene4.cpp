#include "Scene/FightScene4.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Hero/Hero.h"
#include "Hero/Knight.h"
#include "Hero/Ranger.h"
#include "Hero/Sorcerer.h"
#include "Weapon/weapon.h"
#include "Monster/SnowMan.h"
#include "Monster/MonsShooter.h"
#include "Monster/Wildboar.h"
#include "Monster/wizard.h"
#include <cstdlib>
#include <ctime>


using namespace CocosDenshion;

FightScene4* FightScene4::createFightScene(Hero* _hero)
{
    FightScene4* scene = new FightScene4();
    if (scene && scene->init())
    {
        scene->autorelease();//�Զ������ڴ�
        scene->initScene(_hero);
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return NULL;
}

bool FightScene4::initScene(Hero* _hero)
{
    num = 4;
    //���ͼ��
    addLayer();
    //�����Ƭ��ͼ
    auto map = TMXTiledMap::create("Tmx/fightingscene3.tmx");
    scale = 1;
    map->setScale(scale);
    map->setTag(3);
    map->setAnchorPoint(Vec2(0, 0));
    height = 116;////
    pos = Point(-88,-144);//
    gid = Point(282, 269);/////
    map->setPosition(pos);
    dynamicLayer->addChild(map, 0);
    //���عرյ���
    auto doorClose = map->getLayer("doorClose");
    doorClose->setVisible(false);
    //���ñ�������
    auto audio = SimpleAudioEngine::getInstance();
    audio->playBackgroundMusic("Music/Glacier.mp3", true);
    //���Ӣ��
    addHero(_hero);
    //��ʼ���ó����Ĺ�����Ϣ
    initMonsterInformation();
    //��ʼ��������Ϣ
    initAreaInformation();
    //��ʼ��ս����������Ϣ
    initBoxPosition();
    //���ӱ���
    addBox(Vec2(24 * 32 * scale + pos.x, (height-90) * 32 * scale + pos.y));
    addBox(Vec2(96 * 32 * scale + pos.x, (height - 6) * 32 * scale + pos.y));
    //��Ӽ��鲢���³��������Ķ�ʱ��
    schedule(schedule_selector(FightScene4::update), 0.01);
    //��Ӹ����������ݵĶ�ʱ��
    schedule(schedule_selector(FightScene4::updateArea), 0.2);
    return true;
}
//��ʼ���ó����Ĺ�����Ϣ
void FightScene4::initMonsterInformation()
{
    srand((unsigned)time(0));
    auto map = static_cast<TMXTiledMap*>(dynamicLayer->getChildByTag(3));
    //�ó����Ĺ����ʼλ����Ϣ
    Point posMonster[3][2][8] = //��ս������ÿ��������ÿ�����8��
    {
        {{ {12 ,47}, {27 ,46 }, {26,64}, {17,56} ,{7,67}, {-1,-1},{-1,-1},{-1,-1} } ,
         { {12 ,47}, {27 ,46 }, {26,64}, {17,56} ,{7,67}, {-1,-1},{-1,-1},{-1,-1}} },
        {{ {48 ,68}, {60 ,68 }, {69,69}, {71,58} ,{60,56}, {50,55},{51,47},{63,45} } ,
         { {48 ,68}, {60 ,68 }, {69,69}, {71,58} ,{60,56}, {50,55},{51,47},{63,45} } },
        {{ {93 ,67 }, {101,68}, {113,68}, {111,57} ,{111,50}, {104,47}, {104,47},{93,47} } ,
         { {93 ,67 }, {101,68}, {113,68}, {111,57} ,{111,50}, {104,47}, {104,47},{93,47} } }//����С�ֵľ��ã�-1��-1����ʶ
    };
    //��С�ּ���������
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            int num = rand() % 4;//ʵ��С���������
            for (int k = 0; k < 8; k++)
            {
                if (posMonster[i][j][k].x != -1 && posMonster[i][j][k].y != -1)
                {
                    //�ٴε���һ������
                    posMonster[i][j][k].x = 32 * posMonster[i][j][k].x * scale + pos.x;
                    posMonster[i][j][k].y = (height - posMonster[i][j][k].y)*32* scale + pos.y;
                    if (num % 4 == 0)
                    {
                        auto mons = Snowman::createMonsterSprite(posMonster[i][j][k], this);
                        mons->getHeroInstance(hero);
                        allMonster[i][j].push_back(mons);
                        dynamicLayer->addChild(mons);
                    }
                    else if (num % 4 == 1)
                    {
                        auto mons = Wildboar::createMonsterSprite(posMonster[i][j][k], this);
                        mons->getHeroInstance(hero);
                        allMonster[i][j].push_back(mons);
                        dynamicLayer->addChild(mons);
                    }
                    else if (num % 4 == 2)
                    {
                        auto mons = MonsShooter::createMonsterSprite(posMonster[i][j][k], this);
                        mons->getHeroInstance(hero);
                        allMonster[i][j].push_back(mons);
                        dynamicLayer->addChild(mons);
                    }
                    else if (num % 4 == 3)
                    {
                        auto mons = Wizard::createMonsterSprite(posMonster[i][j][k], this);
                        mons->getHeroInstance(hero);
                        allMonster[i][j].push_back(mons);
                        dynamicLayer->addChild(mons);
                    }
                    num++;
                }
            }
        }
    }

}
//��ʼ��������Ϣ
void FightScene4::initAreaInformation()
{
    Area _area[5] =
    {
        {  Rect(4,112,26,25),false,{-1,1,-1,-1},false,0 ,0,0 ,Point(950,500)},
        {  Rect(4,68,24,24),false,{-1,-1,2,0 },true,0,0,1 ,Point(950,580)  },
        {  Rect(47,70,25,28),false,{1,-1,3,-1 },true,1,0,1,Point(1030,580)   },
        {  Rect(91,69,24,24),false,{2,4,-1,-1},true,2,0,1 ,Point(1110,580) },
        {  Rect(90,28,25,24),false,{-1,-1,-1,3},false,0,0,0 ,Point(1110,660) }
    };
    auto map = static_cast<TMXTiledMap*>(dynamicLayer->getChildByTag(3));
    for (int i = 0; i < 5; i++)
    {
        int x = _area[i].rect.origin.x * 32 * scale + pos.x;
        int y = (height - _area[i].rect.origin.y) * 32 * scale + pos.y;
        int dx = _area[i].rect.size.width * 32 * scale;
        int dy = _area[i].rect.size.height * 32 * scale;
        _area[i].rect.setRect(x, y, dx, dy);
        area.push_back(_area[i]);
    }
}
//�л�С��
void FightScene4::changeMonster(int count, int num)
{
    monster = allMonster[count][num];
    addMonster(monster);
}
//��ʼ������λ����Ϣ
void FightScene4::initBoxPosition()
{
    posBox.push_back(Point(6 * 32 * scale + pos.x, (height - 46) * 32 * scale + pos.y));
    posBox.push_back(Point(70 * 32 * scale + pos.x, (height - 67) * 32 * scale + pos.y));
    posBox.push_back(Point(111 * 32 * scale + pos.x, (height - 49) * 32 * scale + pos.y));
}