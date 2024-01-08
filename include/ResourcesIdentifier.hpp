#ifndef BOOK_RESOURCEIDENTIFIERS_HPP
#define BOOK_RESOURCEIDENTIFIERS_HPP


// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
	class Shader;
	class SoundBuffer;
}

namespace Textures
{
	enum ID
	{
		BlueWitch,
		FlyingEye,
		FlyingEyeIcon,
		Goblin,
		GoblinIcon,
		Mushroom,
		MushroomIcon,
		Skeleton,
		SkeletonIcon,
		DarkWizzard,
		Desert,
		Title,
		Panel,
		bigPanel,
		CreditPanel,
		ButtonNormal,
		ButtonSelected,
		ButtonPressed,
		smallButtonNormal,
		smallButtonSelected,
		smallButtonPressed,
		scrollBar,
		scrollBarButtonNormal,
		scrollBarButtonSelected,
		musicIcon,
		musicIconMuted,
		soundIcon,
		AlliedBullet,
		AlliedSkillE,
		AlliedSkillQ,
		AlliedUltimate,
		MechaBossRangedAttack,
		DarkAttack,
		ExplosionAttack,
		ThunderStrike,
		FlyingEyeBullet,
		GoblinBullet,
		MushroomBullet,
		SkeletonBullet,
		ExpBar,
		ExpBarFrame,
		HealthBar,
		HealthBarFrame,
		SkillEIcon,
		SkillQIcon,
		UltimateIcon,
		SkillBorder,
		IconBlur,
		Particle,
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
		Title
	};
}

namespace Shaders
{
	enum ID
	{
		BrightnessPass,
		DownSamplePass,
		GaussianBlurPass,
		AddPass,
	};
}

namespace SoundEffect
{
	enum ID
	{
		AlliedFiring,
		AlliedAbility,
		AlliedDebuff,
		AlliedUltimate,
		FlyingEyeFiring,
		GoblinFiring,
		MushroomFiring,
		SkeletonFiring,
		DarkAttack,
		ExplosionAttack,
		ThunderStrike,
		ThunderCasting,
		ExplosionCasting,
		DarkAttackCasting,
		Chasing,
		LevelUp,
	};
}

namespace Music
{
	enum ID
	{
		MenuTheme,
		Before5MinsTheme,
		After5MinsTheme,
		After10MinsTheme,
		BossTheme,
	};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::Shader, Shaders::ID> ShaderHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID> SoundBufferHolder;

#endif // BOOK_RESOURCEIDENTIFIERS_HPP
