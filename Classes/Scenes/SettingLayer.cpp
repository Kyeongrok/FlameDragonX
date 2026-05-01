#include "SettingLayer.hpp"
#include "SceneCreator.hpp"
#include "Constants.hpp"

USING_NS_CC;

const char* SettingLayer::KEY_STORY_ENABLED   = "storyEventsEnabled";
const char* SettingLayer::KEY_DIALOG_FONT_SIZE = "dialogFontSize";
const char* SettingLayer::KEY_WINDOW_SCALE    = "windowScale";

static const char* FONT = "fonts/arial.ttf";
static const int FONT_SIZE_OPTIONS[] = { 12, 14, 18, 24 };
static const int FONT_SIZE_COUNT = 4;
static const int WINDOW_SCALE_OPTIONS[] = { 1, 2, 3, 4 };
static const int WINDOW_SCALE_COUNT = 4;
static const int MENU_COUNT = 4;

namespace {
    Label * _menuLabels[MENU_COUNT] = { nullptr, nullptr, nullptr, nullptr };
}

bool SettingLayer::init()
{
    if (!Layer::init()) return false;

    Size screen = Constants::getScreenSize();

    auto title = Label::createWithTTF("Settings", FONT, 28);
    title->setPosition(screen.width / 2, screen.height - 40);
    title->setColor(Color3B::WHITE);
    this->addChild(title);

    float lineY = screen.height / 2 + 50;
    float lineGap = 40;

    auto storyLabel = Label::createWithTTF("Story Events", FONT, 18);
    storyLabel->setAnchorPoint(Vec2(0, 0.5f));
    storyLabel->setPosition(screen.width / 2 - 130, lineY);
    this->addChild(storyLabel);

    _storyValueLabel = Label::createWithTTF("OFF", FONT, 18);
    _storyValueLabel->setAnchorPoint(Vec2(1, 0.5f));
    _storyValueLabel->setPosition(screen.width / 2 + 130, lineY);
    this->addChild(_storyValueLabel);
    updateStoryLabel();

    auto fontLabel = Label::createWithTTF("Dialog Font Size", FONT, 18);
    fontLabel->setAnchorPoint(Vec2(0, 0.5f));
    fontLabel->setPosition(screen.width / 2 - 130, lineY - lineGap);
    this->addChild(fontLabel);

    _fontValueLabel = Label::createWithTTF("14", FONT, 18);
    _fontValueLabel->setAnchorPoint(Vec2(1, 0.5f));
    _fontValueLabel->setPosition(screen.width / 2 + 130, lineY - lineGap);
    this->addChild(_fontValueLabel);
    updateFontLabel();

    auto resolutionLabel = Label::createWithTTF("Resolution", FONT, 18);
    resolutionLabel->setAnchorPoint(Vec2(0, 0.5f));
    resolutionLabel->setPosition(screen.width / 2 - 130, lineY - lineGap * 2);
    this->addChild(resolutionLabel);

    _resolutionValueLabel = Label::createWithTTF("1440x960", FONT, 18);
    _resolutionValueLabel->setAnchorPoint(Vec2(1, 0.5f));
    _resolutionValueLabel->setPosition(screen.width / 2 + 130, lineY - lineGap * 2);
    this->addChild(_resolutionValueLabel);
    updateResolutionLabel();

    _resolutionHintLabel = Label::createWithTTF("(restart required)", FONT, 12);
    _resolutionHintLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
    _resolutionHintLabel->setPosition(screen.width / 2, lineY - lineGap * 2 - 20);
    _resolutionHintLabel->setColor(Color3B(180, 180, 180));
    this->addChild(_resolutionHintLabel);

    auto backLabel = Label::createWithTTF("Back", FONT, 18);
    backLabel->setPosition(screen.width / 2, lineY - lineGap * 3 - 20);
    this->addChild(backLabel);

    _menuLabels[0] = _storyValueLabel;
    _menuLabels[1] = _fontValueLabel;
    _menuLabels[2] = _resolutionValueLabel;
    _menuLabels[3] = backLabel;
    _selectedIndex = 0;
    updateSelection();

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [this](Touch* touch, Event*) {
        Vec2 p = touch->getLocation();
        for (int i = 0; i < MENU_COUNT; ++i) {
            Size bb = _menuLabels[i]->getContentSize();
            Vec2 anchor = _menuLabels[i]->getAnchorPoint();
            Vec2 pos = _menuLabels[i]->getPosition();
            Rect world(pos.x - bb.width * anchor.x - 10,
                       pos.y - bb.height * anchor.y - 10,
                       bb.width + 20, bb.height + 20);
            if (world.containsPoint(p)) {
                _selectedIndex = i;
                updateSelection();
                if (i == 0) toggleStory();
                else if (i == 1) cycleFontSize();
                else if (i == 2) cycleResolution();
                else backToTitle();
                return true;
            }
        }
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    auto kb = EventListenerKeyboard::create();
    kb->onKeyPressed = [this](EventKeyboard::KeyCode code, Event*) {
        switch (code) {
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
                _selectedIndex = (_selectedIndex - 1 + MENU_COUNT) % MENU_COUNT;
                updateSelection(); break;
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                _selectedIndex = (_selectedIndex + 1) % MENU_COUNT;
                updateSelection(); break;
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                if (_selectedIndex == 0) toggleStory();
                else if (_selectedIndex == 1) cycleFontSize();
                else if (_selectedIndex == 2) cycleResolution();
                break;
            case EventKeyboard::KeyCode::KEY_ENTER:
            case EventKeyboard::KeyCode::KEY_KP_ENTER:
            case EventKeyboard::KeyCode::KEY_SPACE:
                if (_selectedIndex == 0) toggleStory();
                else if (_selectedIndex == 1) cycleFontSize();
                else if (_selectedIndex == 2) cycleResolution();
                else backToTitle();
                break;
            case EventKeyboard::KeyCode::KEY_ESCAPE:
                backToTitle(); break;
            default: break;
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(kb, this);

    return true;
}

void SettingLayer::updateStoryLabel()
{
    bool enabled = UserDefault::getInstance()->getBoolForKey(KEY_STORY_ENABLED, false);
    _storyValueLabel->setString(enabled ? "ON" : "OFF");
}

void SettingLayer::updateFontLabel()
{
    int size = UserDefault::getInstance()->getIntegerForKey(KEY_DIALOG_FONT_SIZE, 14);
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", size);
    _fontValueLabel->setString(buf);
}

void SettingLayer::updateSelection()
{
    for (int i = 0; i < MENU_COUNT; ++i) {
        if (_menuLabels[i] == nullptr) continue;
        if (i == _selectedIndex) {
            _menuLabels[i]->setColor(Color3B::YELLOW);
            _menuLabels[i]->setScale(1.2f);
        } else {
            _menuLabels[i]->setColor(Color3B::WHITE);
            _menuLabels[i]->setScale(1.0f);
        }
    }
}

void SettingLayer::toggleStory()
{
    bool current = UserDefault::getInstance()->getBoolForKey(KEY_STORY_ENABLED, false);
    UserDefault::getInstance()->setBoolForKey(KEY_STORY_ENABLED, !current);
    UserDefault::getInstance()->flush();
    updateStoryLabel();
}

void SettingLayer::cycleFontSize()
{
    int current = UserDefault::getInstance()->getIntegerForKey(KEY_DIALOG_FONT_SIZE, 14);
    int idx = 1;
    for (int i = 0; i < FONT_SIZE_COUNT; ++i) {
        if (FONT_SIZE_OPTIONS[i] == current) { idx = i; break; }
    }
    int next = FONT_SIZE_OPTIONS[(idx + 1) % FONT_SIZE_COUNT];
    UserDefault::getInstance()->setIntegerForKey(KEY_DIALOG_FONT_SIZE, next);
    UserDefault::getInstance()->flush();
    updateFontLabel();
}

void SettingLayer::updateResolutionLabel()
{
    int scale = UserDefault::getInstance()->getIntegerForKey(KEY_WINDOW_SCALE, DEFAULT_WINDOW_SCALE);
    char buf[32];
    snprintf(buf, sizeof(buf), "%dx%d",
             Constants::ORIGIN_SCREEN_WIDTH * scale,
             Constants::ORIGIN_SCREEN_HEIGHT * scale);
    _resolutionValueLabel->setString(buf);
}

void SettingLayer::cycleResolution()
{
    int current = UserDefault::getInstance()->getIntegerForKey(KEY_WINDOW_SCALE, DEFAULT_WINDOW_SCALE);
    int idx = 0;
    for (int i = 0; i < WINDOW_SCALE_COUNT; ++i) {
        if (WINDOW_SCALE_OPTIONS[i] == current) { idx = i; break; }
    }
    int next = WINDOW_SCALE_OPTIONS[(idx + 1) % WINDOW_SCALE_COUNT];
    UserDefault::getInstance()->setIntegerForKey(KEY_WINDOW_SCALE, next);
    UserDefault::getInstance()->flush();
    updateResolutionLabel();
}

void SettingLayer::backToTitle()
{
    Director::getInstance()->replaceScene(SceneCreator::createTitleScene());
}
