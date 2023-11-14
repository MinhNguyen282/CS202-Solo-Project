#include "include/Component.hpp"    

namespace GUI
{
    Component::Component()
    : mIsSelected(false)
    , mIsActive(false)
    {

    }

    Component::~Component()
    {

    }

    void Component::select()
    {
        mIsSelected = true;
    }

    void Component::deselect()
    {
        mIsSelected = false;
    }

    bool Component::isSelected() const
    {
        return mIsSelected;
    }

    void Component::activate()
    {
        mIsActive = true;
    }

    void Component::deactivate()
    {
        mIsActive = false;
    }

    bool Component::isActive() const
    {
        return mIsActive;
    }
}