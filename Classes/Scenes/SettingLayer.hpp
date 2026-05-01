#ifndef SettingLayer_hpp
#define SettingLayer_hpp

#include "cocos2d.h"

class SettingLayer : public cocos2d::Layer
{
public:
    static const char* KEY_STORY_ENABLED;
    static const char* KEY_DIALOG_FONT_SIZE;
    static const char* KEY_WINDOW_SCALE;
    static const int DEFAULT_WINDOW_SCALE = 3;

    virtual bool init();
    CREATE_FUNC(SettingLayer);

private:
    cocos2d::Label * _storyValueLabel;
    cocos2d::Label * _fontValueLabel;
    cocos2d::Label * _resolutionValueLabel;
    cocos2d::Label * _resolutionHintLabel;
    int _selectedIndex;

    void updateStoryLabel();
    void updateFontLabel();
    void updateResolutionLabel();
    void updateSelection();
    void toggleStory();
    void cycleFontSize();
    void cycleResolution();
    void backToTitle();
};

#endif
