#include "include/SoundPlayer.hpp"

#include <SFML/Audio/Listener.hpp>

#include <cmath>    

namespace
{
	// Sound coordinate system, point of view of a player in front of the screen:
	// X = left; Y = up; Z = back (out of the screen)
	const float ListenerZ = 300.f;
	const float Attenuation = 8.f;
	const float MinDistance2D = 200.f;
	const float MinDistance3D = std::sqrt(MinDistance2D*MinDistance2D + ListenerZ*ListenerZ);
}

SoundPlayer::SoundPlayer()
: mSoundBuffers()
, mSounds()
{
    // Fill the buffers with sounds
    mSoundBuffers.load(SoundEffect::ExplosionAttack, "Media/Sound/DarkWizzard/ExplosionAttack/Explosion.ogg");
    mSoundBuffers.load(SoundEffect::ExplosionCasting, "Media/Sound/DarkWizzard/ExplosionAttack/ExplosionCasting.ogg");

    mSoundBuffers.load(SoundEffect::ThunderStrike, "Media/Sound/DarkWizzard/ThunderStrike/ThunderStrike.ogg");
    mSoundBuffers.load(SoundEffect::ThunderCasting, "Media/Sound/DarkWizzard/ThunderStrike/ThunderCasting.ogg");

    mSoundBuffers.load(SoundEffect::DarkAttack, "Media/Sound/DarkWizzard/DarkAttack/DarkAttack.ogg");
    mSoundBuffers.load(SoundEffect::DarkAttackCasting, "Media/Sound/DarkWizzard/DarkAttack/DarkAttackCasting.ogg");

    mSoundBuffers.load(SoundEffect::Chasing, "Media/Sound/DarkWizzard/Chasing/Chasing.ogg");

    mSoundBuffers.load(SoundEffect::AlliedFiring, "Media/Sound/BlueWitch/AlliedFiring.ogg");
    mSoundBuffers.load(SoundEffect::AlliedAbility, "Media/Sound/BlueWitch/AlliedAbility.ogg");
    mSoundBuffers.load(SoundEffect::AlliedDebuff, "Media/Sound/BlueWitch/AlliedDebuff.ogg");
    mSoundBuffers.load(SoundEffect::AlliedUltimate, "Media/Sound/BlueWitch/AlliedUltimate.ogg");

    mSoundBuffers.load(SoundEffect::FlyingEyeFiring, "Media/Sound/FlyingEye/FlyingEyeFiring.ogg");
    mSoundBuffers.load(SoundEffect::GoblinFiring, "Media/Sound/Goblin/GoblinFiring.ogg");
    mSoundBuffers.load(SoundEffect::MushroomFiring, "Media/Sound/Mushroom/MushroomFiring.ogg");
    mSoundBuffers.load(SoundEffect::SkeletonFiring, "Media/Sound/Skeleton/SkeletonFiring.ogg");

	mSoundBuffers.load(SoundEffect::LevelUp, "Media/Sound/Event/LevelUp.wav");

	// Listener points towards the screen (default in SFML)
	sf::Listener::setDirection(0.f, 0.f, -1.f);
}

void SoundPlayer::play(SoundEffect::ID effect)
{
	play(effect, getListenerPosition());
}

void SoundPlayer::setVolume(float volume)
{
    for(auto& sound : mSounds)
    {
        sound.setVolume(volume);
    }
}

void SoundPlayer::play(SoundEffect::ID effect, sf::Vector2f position)
{
	mSounds.push_back(sf::Sound());
	sf::Sound& sound = mSounds.back();

	sound.setBuffer(mSoundBuffers.get(effect));
	sound.setPosition(position.x, -position.y, 0.f);
	sound.setAttenuation(Attenuation);
	sound.setMinDistance(MinDistance3D);
    sound.setVolume(25.f);

	sound.play();
}

void SoundPlayer::removeStoppedSounds()
{
	mSounds.remove_if([] (const sf::Sound& s)
	{
		return s.getStatus() == sf::Sound::Stopped;
	});
}

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
	sf::Listener::setPosition(position.x, -position.y, ListenerZ);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
	sf::Vector3f position = sf::Listener::getPosition();
	return sf::Vector2f(position.x, -position.y);
}