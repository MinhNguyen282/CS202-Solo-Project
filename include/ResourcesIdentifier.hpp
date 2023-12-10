#ifndef BOOK_RESOURCEIDENTIFIERS_HPP
#define BOOK_RESOURCEIDENTIFIERS_HPP


// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
	class Shader;
}

namespace Textures
{
	enum ID
	{
		BlueWitch,
		FlyingEye,
		Goblin,
		Mushroom,
		Skeleton,
		MechaBoss,
		Desert,
		Title,
		Panel,
		bigPanel,
		ButtonNormal,
		ButtonSelected,
		ButtonPressed,
		smallButtonNormal,
		smallButtonSelected,
		smallButtonPressed,
		AlliedBullet,
		AlliedSkillE,
		AlliedSkillQ,
		MechaBossRangedAttack,
		FlyingEyeBullet,
		GoblinBullet,
		MushroomBullet,
		SkeletonBullet,
		ExpBar,
		ExpBarFrame,
		SkillEIcon,
		SkillQIcon,
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

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::Shader, Shaders::ID> ShaderHolder;

#endif // BOOK_RESOURCEIDENTIFIERS_HPP
