#ifndef SOUNDPLAYER_HPP
#define SOUNDPLAYER_HPP

#include "include/ResourcesIdentifier.hpp"
#include "include/ResourcesHolder.hpp"

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include <list>
#include <fstream>

class SoundPlayer : private sf::NonCopyable
{
	public:
									SoundPlayer();

		void						play(SoundEffect::ID effect);
		void						play(SoundEffect::ID effect, sf::Vector2f position);

		void						removeStoppedSounds();
		void						setListenerPosition(sf::Vector2f position);
		sf::Vector2f				getListenerPosition() const;
        void                        setVolume(float volume); 
		float                       getVolume() const;


	private:
		SoundBufferHolder			mSoundBuffers;
		std::list<sf::Sound>		mSounds;
		float 					 	mVolume;
};


#endif // SOUNDPLAYER_HPP