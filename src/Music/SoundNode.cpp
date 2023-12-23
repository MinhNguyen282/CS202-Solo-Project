#include "include/SoundNode.hpp"
#include "include/SoundPlayer.hpp"

SoundNode::SoundNode(SoundPlayer& player)
: SceneNode()
, mSounds(player)
{
}

void SoundNode::playSound(SoundEffect::ID sound, sf::Vector2f position)
{
    mSounds.play(sound, position);
}

void SoundNode::setVolume(float volume)
{
}

unsigned int SoundNode::getCategory() const
{
    return Category::SoundEffect;
}