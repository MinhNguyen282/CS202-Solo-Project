#include "include/MusicPlayer.hpp"

MusicPlayer::MusicPlayer()
: mMusic()
, mFilenames()
, mVolume(100)
{
    mFilenames[Music::MenuTheme] = "Media/Music/MenuTheme.ogg";
    mFilenames[Music::Before5MinsTheme] = "Media/Music/Before5MinsTheme.ogg";
    mFilenames[Music::After5MinsTheme] = "Media/Music/After5MinsTheme.ogg";
    mFilenames[Music::After10MinsTheme] = "Media/Music/After10MinsTheme.ogg";
    mFilenames[Music::BossTheme] = "Media/Music/BossTheme.ogg";
}

void MusicPlayer::play(Music::ID theme)
{
	std::string filename = mFilenames[theme];

	if (!mMusic.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");

	mMusic.setVolume(mVolume);
	mMusic.setLoop(true);
	mMusic.play();
}

void MusicPlayer::stop()
{
	mMusic.stop();
}

void MusicPlayer::setVolume(float volume)
{
	mVolume = volume;
}

void MusicPlayer::setPaused(bool paused)
{
	if (paused)
		mMusic.pause();
	else
		mMusic.play();
}