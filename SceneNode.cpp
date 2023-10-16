#include "SceneNode.hpp"

SceneNode::SceneNode()
{
    mChildren.clear();
    mParent = nullptr;
}

//Gắn nút con vào cha
void SceneNode::attachChild(Ptr child)
{
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

//Tách nút con ra khỏi cha
SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
    auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr& p) -> bool { return p.get() == &node;});
    Ptr result = std::move(*found);
    result->mParent = nullptr;
    mChildren.erase(found);
    return result;
}

//Update vị trí của nút hiện tại và các nút con thuộc nút hiện tại
void SceneNode::update(sf::Time deltaTime)
{
    updateCurrent(deltaTime);
    updateChildren(deltaTime);
}

//Lấy vị trí thế giới của nút hiện tại
sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

//Lấy ma trận biến đổi thế giới của nút hiện tại
sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;
    for (const SceneNode* node = this; node != nullptr; node = node->mParent)
        transform = node->getTransform() * transform;
    return transform;
}

//Update vị trí của nút hiện tại
void SceneNode::updateCurrent(sf::Time deltaTime)
{
    //Do nothing by default
}

//Update vị trí của các nút con
void SceneNode::updateChildren(sf::Time deltaTime)
{
    for (Ptr& child : mChildren)
        child->update(deltaTime);
}

//Vẽ nút hiện tại và các nút con
void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    drawCurrent(target, states);
    drawChildren(target, states);
}

//Vẽ nút hiện tại
void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    //Do nothing by default
}

//Vẽ các nút con
void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const Ptr& child : mChildren)
        child->draw(target, states);
}