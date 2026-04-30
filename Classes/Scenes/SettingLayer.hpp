#ifndef SettingLayer_hpp
#define SettingLayer_hpp

#include "cocos2d.h"

class SettingLayer : public cocos2d::Layer
{
public:
    static const char* KEY_STORY_ENABLED;
    static const char* KEY_DIALOG_FONT_SIZE;

    virtual bool init();
    CREATE_FUNC(SettingLayer);

private:
    cocos2d::Label * _storyValueLabel;
    cocos2d::Label * _fontValueLabel;
    int _selectedIndex;

    void updateStoryLabel();
    void updateFontLabel();
    void updateSelection();
    void toggleStory();
    void cycleFontSize();
    void backToTitle();
};

#endif
