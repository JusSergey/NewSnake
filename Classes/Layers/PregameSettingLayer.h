#ifndef __PREGAMESETTINGLAYER_H__
#define __PREGAMESETTINGLAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>


// class set colors players
class SwitchColorSnake : public cocos2d::Sprite {

    friend class PregameSettingLayer;

private:
    virtual bool init();
    CREATE_FUNC(SwitchColorSnake);
    void setCallbackSelectColor(const std::function<void(const cocos2d::Color3B &color, int tag)> &callback);

private:
    std::function<void(const cocos2d::Color3B &color, int tag)> callbackSelectColor;

}; // END CLASS SwitchColorSnake


// class for scroll levels.
class SwitchLevelGame : public cocos2d::ui::Button {

    friend class PregameSettingLayer;
private:

    virtual bool init();
    CREATE_FUNC(SwitchLevelGame);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);

}; // END CLASS SwitchLevelGame


// main class for setting players
class PregameSettingLayer : public cocos2d::Layer
{
public:
    virtual bool init();

    CREATE_FUNC(PregameSettingLayer);

    // getPosition is not valid
    virtual void addChild(cocos2d::Node *child, int level);

public:
    void setCallbackNext(const std::function<void (Ref*)> &value);
    void setCallbackBackToMenu(const std::function<void (Ref*)> &value);

private:
    enum {
        PlayerTag = 0xF,
        OpponentTag
    };

private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

private:
    cocos2d::MenuItemLabel *itemStart;
    cocos2d::MenuItemLabel *itemBackToMenu;
    cocos2d::DrawNode *dnode;
    cocos2d::ui::TextField *fieldPlayer;
    cocos2d::ui::TextField *fieldOpponent;

private:
    typedef std::pair<cocos2d::Sprite*, bool> SwitchColorObj_t;
    typedef std::vector<SwitchColorObj_t> SwitchColorContainer_t;

    SwitchColorContainer_t swColorsPlayer1;
    SwitchColorContainer_t swColorsPlayer2;

private:
    void setSwitchColor(SwitchColorContainer_t &sw, const cocos2d::Color3B &color);

private:
    std::function<void(const cocos2d::Color3B &color, int tag)> getCallbackSelectColor();

private:
    void initItemNext();
    void initItemBackToMenu();
    void initMenu();
    void initDrawNode();

    void initLabelSetting(const std::string &title, float procentY, const std::function<float(float)> &positioning);
    void initColors(SwitchColorContainer_t &sw, const std::function<float(float)> &positioning, int tag);
    void initTextFields(const std::function<float(float)> &positioning, cocos2d::ui::TextField* &field, const std::string &name);

    void initScrollViewLevels();

}; // END CLASS PregameSettingLayer

#endif // __PREGAMESETTINGLAYER_H__