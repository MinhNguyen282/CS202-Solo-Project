#include "include/ConfigState.hpp"

ConfigState::ConfigState(StateStack& stack, Context context)
: State(stack, context)
{

    mBackgroundBlur.setSize(sf::Vector2f(1200.f, 720.f));
    mBackgroundBlur.setFillColor(sf::Color(0, 0, 0, 230));
    mBackgroundBlur.setPosition(0.f, 0.f);

    mText.setFont(context.fonts->get(Fonts::Main));
    mText.setString("Stats of Entity");
    mText.setCharacterSize(40);
    mText.setFillColor(sf::Color::White);
    mText.setPosition((1200 - mText.getGlobalBounds().width)/2.f, 50.f);

    Button backButton(*context.textures, *context.fonts, sf::Vector2f(100.f, 50.f), "Back", [this](){
		requestStackPop();
	});

    mEnemyData = initializeEnemyData();

    initStat(Textures::FlyingEyeIcon, 100.f, 150.f, Enemy::FlyingEye);
    initStat(Textures::GoblinIcon, 350.f, 150.f, Enemy::Goblin);
    initStat(Textures::MushroomIcon, 600.f, 150.f, Enemy::Mushroom);
    initStat(Textures::SkeletonIcon, 850.f, 150.f, Enemy::Skeleton);

    mButtons.push_back(std::move(backButton));
}

void ConfigState::initStat(Textures::ID id, float x, float y, Enemy::Type type)
{
    sf::Font& font = getContext().fonts->get(Fonts::Main);
    sf::Vector2f viewSize = getContext().window->getView().getSize();

    sf::Sprite sprite;
    sprite.setTexture(getContext().textures->get(id));
    sprite.setPosition(x, y);
    mSprites.push_back(std::move(sprite));

    Button hpButton(*getContext().textures, *getContext().fonts, sf::Vector2f(x, y+150.f), "HP: " + std::to_string(mEnemyData[type].hitpoints), [this](){
        
    });
    Button speedButton(*getContext().textures, *getContext().fonts, sf::Vector2f(x, y + 225.f), "Speed: " + std::to_string(mEnemyData[type].speed), [this](){

    });
    Button bodyDamageButton(*getContext().textures, *getContext().fonts, sf::Vector2f(x, y + 300.f), "Body Damage: " + std::to_string(mEnemyData[type].bodyDamage), [this](){

    });
    Button bulletDamageButton(*getContext().textures, *getContext().fonts, sf::Vector2f(x, y + 375.f), "Projectile Damage: " + std::to_string(mEnemyData[type].fireDamage), [this](){

    });
    mButtons.push_back(std::move(hpButton));
    mButtons.push_back(std::move(speedButton));
    mButtons.push_back(std::move(bodyDamageButton));
    mButtons.push_back(std::move(bulletDamageButton));
}

void ConfigState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(mBackgroundBlur);
    window.draw(mText);
    for(auto& sprite : mSprites)
        window.draw(sprite);
    for(auto& button : mButtons)
        window.draw(button);
}

bool ConfigState::update(sf::Time deltaTime)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
    for(int i=0; i<mButtons.size(); i++){
        mButtons[i].update(deltaTime, mousePos);
    }
    return false;
}

bool ConfigState::handleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed){
        if(event.key.code == sf::Keyboard::Escape){
            requestStackPop();
        }
    }
    sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
        for(int i=0; i<mButtons.size(); i++){
        mButtons[i].handleEvent(event, mousePos);
    }
    return false;
}