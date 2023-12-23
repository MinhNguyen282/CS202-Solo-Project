#include "include/SceneNode.hpp"

SceneNode::SceneNode(Category::Type category)
: mChildren()
, mParent(nullptr)
, mDefaultCategory(category)
, isDrawBoundingRect(false)
{
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
void SceneNode::update(sf::Time deltaTime, CommandQueue& commands)
{
    updateCurrent(deltaTime, commands);
    updateChildren(deltaTime, commands);
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

//Gửi lệnh đến nút hiện tại và các nút con
void SceneNode::onCommand(const Command& command, sf::Time deltaTime)
{
    if (command.category & getCategory())
    {
        command.action(*this, deltaTime);
    }
    for (Ptr& child : mChildren)
        child->onCommand(command, deltaTime);
}

//Lấy danh mục của nút hiện tại
unsigned int SceneNode::getCategory() const
{
    //return Category::Scene;
    return mDefaultCategory;
}

//Update vị trí của nút hiện tại
void SceneNode::updateCurrent(sf::Time deltaTime, CommandQueue& commands)
{
    //Do nothing by default
}

//Update vị trí của các nút con
void SceneNode::updateChildren(sf::Time deltaTime, CommandQueue& commands)
{
    for (Ptr& child : mChildren)
        child->update(deltaTime, commands);
}

void SceneNode::setDrawBoundingRect(bool drawBoundingRect)
{
    isDrawBoundingRect = drawBoundingRect;
}

//Vẽ nút hiện tại và các nút con
void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    drawCurrent(target, states);
    drawChildren(target, states);

    //draw Bounding of object

    if (isDrawBoundingRect) drawBoundingRect(target, states);
}

int SceneNode::getSize() const
{
    return mChildren.size();
}

//Vẽ nút hiện tại
void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{

}

//Vẽ các nút con
void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const Ptr& child : mChildren)
        child->draw(target, states);
}

//Lấy khung bao của nút hiện tại
sf::FloatRect SceneNode::getBoundingRect() const
{
    return sf::FloatRect();
}

//Vẽ khung bao của nút hiện tại
void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::FloatRect rect = getBoundingRect();
    sf::RectangleShape shape;
    shape.setPosition(sf::Vector2f(rect.left, rect.top));
    shape.setSize(sf::Vector2f(rect.width, rect.height));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(1.f);
    target.draw(shape);
}

//Xóa các nút con bị hủy
void SceneNode::removeWrecks()
{
    auto wreckfieldBegin = std::remove_if(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
    mChildren.erase(wreckfieldBegin, mChildren.end());
    std::for_each(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}

bool SceneNode::isMarkedForRemoval() const
{
    return isDestroyed();
}

bool SceneNode::isDestroyed() const
{
    return false;
}

void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs)
{
    checkNodeCollision(sceneGraph, collisionPairs);
    for (Ptr& child : sceneGraph.mChildren)
        checkSceneCollision(*child, collisionPairs);
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
    if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
        collisionPairs.insert(std::minmax(this, &node));
    for (Ptr& child : mChildren)
        child->checkNodeCollision(node, collisionPairs);
}

//Kiểm tra va chạm giữa 2 nút
bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
    return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

//Tính khoảng cách giữa 2 nút
float distance(const SceneNode& lhs, const SceneNode& rhs)
{
    return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}