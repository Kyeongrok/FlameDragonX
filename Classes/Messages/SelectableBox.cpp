//
//  SelectableBox.cpp
//  FlameDragonX
//
//  Created by SuiYi on 9/15/16.
//
//

#include "SelectableBox.hpp"

SelectableBox::SelectableBox(Creature * creature, MessageBoxOperatingType type, Ref* caller, SEL_CALLBACK1 method)
{
    _creature = creature;
    _operatingType = type;
    _caller = caller;
    _callerMethod = method;

    _baseSprite = ScaledSprite::create("Others/ContainerBase.png");

    _initialX = 5;
    _initialY = 5;
    _spacingX = 100;
    _spacingY = 25;

    _selectedIndex = -1;
}

ScaledSprite * SelectableBox::getSprite()
{
    return _baseSprite;
}

int SelectableBox::handleClick(Vec2 location)
{
    log("Clicked at SelectableBox (%f, %f)", location.x, location.y);

    return 0;
}

void SelectableBox::registerSelectable(Label * label, int tag)
{
    _selectableLabels.push_back(label);
    _selectableTags.push_back(tag);
    if (_selectedIndex < 0) {
        _selectedIndex = 0;
        applyHighlight();
    }
}

void SelectableBox::navigate(int delta)
{
    int n = (int)_selectableLabels.size();
    if (n == 0) return;
    if (_selectedIndex < 0) _selectedIndex = 0;
    _selectedIndex = (_selectedIndex + delta + n) % n;
    applyHighlight();
}

void SelectableBox::confirmSelected()
{
    if (_selectedIndex < 0 || _selectedIndex >= (int)_selectableTags.size()) return;
    if (_caller == nullptr || _callerMethod == nullptr) return;
    int tag = _selectableTags[_selectedIndex];
    (_caller->*_callerMethod)(tag);
}

void SelectableBox::applyHighlight()
{
    for (int i = 0; i < (int)_selectableLabels.size(); ++i) {
        if (i == _selectedIndex) {
            _selectableLabels[i]->setColor(Color3B::YELLOW);
        } else {
            _selectableLabels[i]->setColor(Color3B::WHITE);
        }
    }
}
