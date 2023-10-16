#ifndef SCENENODE_HPP
#define SCENENODE_HPP
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>  

#include "ResourcesIdentifier.hpp"
#include <vector>
#include <memory>

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
    public:
        typedef std::unique_ptr<SceneNode> Ptr;
    
    public:
        SceneNode();
        void attachChild(Ptr child);
        Ptr detachChild(const SceneNode& node);

        void update(sf::Time deltaTime);

        sf::Vector2f getWorldPosition() const;
        sf::Transform getWorldTransform() const;
    
    private:
        virtual void updateCurrent(sf::Time deltaTime);
        void updateChildren(sf::Time deltaTime);

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        std::vector<Ptr> mChildren;
        SceneNode* mParent;
};

#endif // SCENENODE_HPP