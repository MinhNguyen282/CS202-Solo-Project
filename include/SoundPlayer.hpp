#ifndef SOUNDPLAYER_HPP
#define SOUNDPLAYER_HPP

#include "include/ResourcesIdentifier.hpp"
#include "include/ResourcesHolder.hpp"

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include <list>

class SoundPlayer : private sf::NonCopyable
{
	public:
									SoundPlayer();

		void						play(SoundEffect::ID effect);
		void						play(SoundEffect::ID effect, sf::Vector2f position);

		void						removeStoppedSounds();
		void						setListenerPosition(sf::Vector2f position);
		sf::Vector2f				getListenerPosition() const;


	private:
		SoundBufferHolder			mSoundBuffers;
		std::list<sf::Sound>		mSounds;
};


#endif // SOUNDPLAYER_HPP