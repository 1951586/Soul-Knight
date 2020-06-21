#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"

class  LoadingScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void callBack(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(LoadingScene);
};
#endif //__LOADING_SCENE_H__