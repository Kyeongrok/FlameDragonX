//
//  SelectableBox.hpp
//  FlameDragonX
//
//  Created by SuiYi on 9/15/16.
//
//

#ifndef SelectableBox_hpp
#define SelectableBox_hpp

typedef enum MessageBoxOperatingType
{
    MessageBoxOperatingType_Select,
    MessageBoxOperatingType_ShowOnly,
    MessageBoxOperatingType_Equip,
    MessageBoxOperatingType_Use
    
} MessageBoxOperatingType;

#include "cocos2d.h"
#include "Creature.hpp"
#include "ScaledSprite.hpp"
#include "CallbackMethod.hpp"
#include <vector>

USING_NS_CC;

class SelectableBox : public cocos2d::Ref
{
protected:

    Creature * _creature;

    MessageBoxOperatingType _operatingType;

    ScaledSprite * _baseSprite;

    Ref * _caller;
    SEL_CALLBACK1 _callerMethod;

    float _initialX;
    float _initialY;
    float _spacingX;
    float _spacingY;

    int _objectCount;

    // Keyboard navigation: indices in _selectableLabels are the navigable items.
    std::vector<Label *> _selectableLabels;
    std::vector<int> _selectableTags;     // tag (= original item index) for each label
    int _selectedIndex;                   // index into _selectableLabels, -1 if none

public:

    SelectableBox(Creature * creature, MessageBoxOperatingType type, Ref* caller, SEL_CALLBACK1 method);

    ScaledSprite * getSprite();

    // virtual void drawObjects();
    int handleClick(Vec2 location);

    void registerSelectable(Label * label, int tag);
    void navigate(int delta);
    void confirmSelected();
    void applyHighlight();

};

#endif /* SelectableBox_hpp */
