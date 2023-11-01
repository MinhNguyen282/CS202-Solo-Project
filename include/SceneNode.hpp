#ifndef SCENENODE_HPP
#define SCENENODE_HPP
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>  

#include "ResourcesIdentifier.hpp"
#include "Utility.hpp"
#include "Category.hpp"
#include "Command.hpp"
#include "CommandQueue.hpp"
#include <vector>
#include <memory>
#include <set>
#include <iostream>

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
    public:
        typedef std::unique_ptr<SceneNode> Ptr;
        typedef std::pair<SceneNode*, SceneNode*> Pair;
    
    public:
        explicit SceneNode(Category::Type category = Category::None);
        void attachChild(Ptr child);
        Ptr detachChild(const SceneNode& node);

        void update(sf::Time deltaTime, CommandQueue& commands);
        void removeWrecks();
        virtual bool isMarkedForRemoval() const;
        virtual bool isDestroyed() const;

        sf::Vector2f getWorldPosition() const;
        sf::Transform getWorldTransform() const;

        void onCommand(const Command& command, sf::Time deltaTime);
        virtual unsigned int getCategory() const;

        void checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);
        void checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs);

        virtual sf::FloatRect getBoundingRect() const;
    
    private:
        virtual void updateCurrent(sf::Time deltaTime, CommandQueue& commands);
        void updateChildren(sf::Time deltaTime, CommandQueue& commands);

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
        void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        std::vector<Ptr> mChildren;
        SceneNode* mParent;
        Category::Type mDefaultCategory;
};

bool collision(const SceneNode& lhs, const SceneNode& rhs);
float distance(const SceneNode& lhs, const SceneNode& rhs);

#endif // SCENENODE_HPP