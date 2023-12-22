#include "include/UpgradeState.hpp"
#include "include/Utility.hpp"
#include "include/ResourcesHolder.hpp"

UpgradeState::UpgradeState(StateStack& stack, Context context)
: State(stack, context)
{
    sf::Texture& texture = context.textures->get(Textures::Title);
    sf::Font& font = context.fonts->get(Fonts::Main);

    mBackgroundSprite.setTexture(texture);

    mBlur.setSize(sf::Vector2f(1200.f, 720.f));
    mBlur.setFillColor(sf::Color(0, 0, 0, 200));
    mBlur.setPosition(0.f, 0.f);

    mExit.setSize(sf::Vector2f(100.f, 50.f));
    mExit.setFillColor(sf::Color::Black);
    mExit.setOutlineThickness(2.f);
    mExit.setOutlineColor(sf::Color::White);
    mExit.setPosition(50.f, 90.f);

    mExitText.setFont(font);
    mExitText.setString("Exit");
    mExitText.setCharacterSize(20);
    mExitText.setFillColor(sf::Color::White);
    mExitText.setPosition(55.f, 95.f);

    initializeUpgrade();
    initializeUpgradeCost();
    initializeGUI();
    initializeGUI2();
}

void UpgradeState::initializeUpgradeCost()
{
    std::ifstream ifs("Data/Upgrade/BlueWitch/UpgradeCost.dat");
    for(int i=0; i<10; i++) ifs >> mHitpointUpCost[i];
    for(int i=0; i<10; i++) ifs >> mSpeedUpCost[i];
    for(int i=0; i<10; i++) ifs >> mCooldownUpCost[i];
    for(int i=0; i<10; i++) ifs >> mFireDamageUpCost[i];
    for(int i=0; i<10; i++) ifs >> mAbilityDamageUpCost[i];
    for(int i=0; i<10; i++) ifs >> mDebuffDamageUpCost[i];
    for(int i=0; i<10; i++) ifs >> mUltimateDamageUpCost[i];
    ifs.close();
}

void UpgradeState::initializeUpgrade()
{
    std::ifstream ifs("Data/Upgrade/mUpgradePoints.dat");
    ifs >> mUpgradePoints;
    ifs.close();
    mUpgradePointsText.setFont(getContext().fonts->get(Fonts::Main));
    mUpgradePointsText.setString("Upgrade Points: " + toString(mUpgradePoints));
    mUpgradePointsText.setCharacterSize(40);
    mUpgradePointsText.setFillColor(sf::Color::White);
    mUpgradePointsText.setPosition((1200 - mUpgradePointsText.getGlobalBounds().getSize().x)/2, 50.f);

    ifs.open("Data/Upgrade/BlueWitch/UpgradePower.dat");
    ifs >> curHitpointUp >> maxHitpointUp >> mHitpointUp;
    ifs >> curSpeedUp >> maxSpeedUp >> mSpeedUp;
    ifs >> curCooldownUp >> maxCooldownUp >> mCooldownUp;
    ifs.close();

    ifs.open("Data/Upgrade/BlueWitch/ProjectilePower.dat");
    ifs >> curFireDamageUp >> maxFireDamageUp >> mFireDamageUp;
    ifs >> curAbilityDamageUp >> maxAbilityDamageUp >> mAbilityDamageUp;
    ifs >> curDebuffDamageUp >> maxDebuffDamageUp >> mDebuffDamageUp;
    ifs >> curUltimateDamageUp >> maxUltimateDamageUp >> mUltimateDamageUp;
    ifs.close();
}

void UpgradeState::initializeGUI()
{
    sf::Font& font = getContext().fonts->get(Fonts::Main);

    hpText.setFont(font);
    hpText.setString("Hitpoint");
    hpText.setCharacterSize(30);
    hpText.setFillColor(sf::Color::White);
    hpText.setPosition(50.f, 200.f);

    hpUp.setSize(sf::Vector2f(300.f, 50.f));
    float percent = static_cast<float>(curHitpointUp)/maxHitpointUp;
    hpUp.setScale(percent, 1.f);
    hpUp.setFillColor(sf::Color::Red);
    hpUp.setPosition(50.f, 250.f);

    hpMax.setSize(sf::Vector2f(300.f, 50.f));
    hpMax.setFillColor(sf::Color::Black);
    hpMax.setPosition(50.f, 250.f);
    hpMax.setOutlineColor(sf::Color::Red);
    hpMax.setOutlineThickness(2.f);

    hpAddUpText.setFont(font);
    hpAddUpText.setString("+" + toString(curHitpointUp));
    hpAddUpText.setCharacterSize(30);
    hpAddUpText.setFillColor(sf::Color::White);
    hpAddUpText.setPosition(350.f + (150.f - hpAddUpText.getGlobalBounds().getSize().x)/2, 250.f);

    hpAddButton.setSize(sf::Vector2f(50.f, 50.f));
    hpAddButton.setFillColor(sf::Color::Black);
    hpAddButton.setOutlineThickness(2.f);
    hpAddButton.setOutlineColor(sf::Color::White);
    hpAddButton.setPosition(500.f, 250.f);

    hpUpCostText.setFont(font);
    hpUpCostText.setString(toString(mHitpointUpCost[curHitpointUp/mHitpointUp]));
    hpUpCostText.setCharacterSize(15);
    hpUpCostText.setFillColor(sf::Color::White);
    hpUpCostText.setPosition(500.f + (50.f - hpUpCostText.getGlobalBounds().getSize().x)/2, 258.75);

    speedText.setFont(font);
    speedText.setString("Speed");
    speedText.setCharacterSize(30);
    speedText.setFillColor(sf::Color::White);
    speedText.setPosition(50.f, 325.f);

    speedUp.setSize(sf::Vector2f(300.f, 50.f));
    percent = static_cast<float>(curSpeedUp)/maxSpeedUp;
    speedUp.setScale(percent, 1.f);
    speedUp.setFillColor(sf::Color::Red);
    speedUp.setPosition(50.f, 375.f);

    speedMax.setSize(sf::Vector2f(300.f, 50.f));
    speedMax.setFillColor(sf::Color::Black);
    speedMax.setPosition(50.f, 375.f);
    speedMax.setOutlineColor(sf::Color::Red);
    speedMax.setOutlineThickness(2.f);

    speedAddUpText.setFont(font);
    speedAddUpText.setString("+" + toString(curSpeedUp));
    speedAddUpText.setCharacterSize(30);
    speedAddUpText.setFillColor(sf::Color::White);
    speedAddUpText.setPosition(350.f + (150.f - speedAddUpText.getGlobalBounds().getSize().x)/2, 375.f);

    speedAddButton.setSize(sf::Vector2f(50.f, 50.f));
    speedAddButton.setFillColor(sf::Color::Black);
    speedAddButton.setOutlineThickness(2.f);
    speedAddButton.setOutlineColor(sf::Color::White);
    speedAddButton.setPosition(500.f, 375.f);

    speedUpCostText.setFont(font);
    speedUpCostText.setString(toString(mSpeedUpCost[curSpeedUp/mSpeedUp]));
    speedUpCostText.setCharacterSize(15);
    speedUpCostText.setFillColor(sf::Color::White);
    speedUpCostText.setPosition(500.f + (50.f - speedUpCostText.getGlobalBounds().getSize().x)/2, 383.75);

    cooldownText.setFont(font);
    cooldownText.setString("Cooldown");
    cooldownText.setCharacterSize(30);
    cooldownText.setFillColor(sf::Color::White);
    cooldownText.setPosition(50.f, 450.f);

    cooldownUp.setSize(sf::Vector2f(300.f, 50.f));
    percent = static_cast<float>(curCooldownUp)/maxCooldownUp;
    cooldownUp.setScale(percent, 1.f);
    cooldownUp.setFillColor(sf::Color::Red);
    cooldownUp.setPosition(50.f, 500.f);
    
    cooldownMax.setSize(sf::Vector2f(300.f, 50.f));
    cooldownMax.setFillColor(sf::Color::Black);
    cooldownMax.setPosition(50.f, 500.f);
    cooldownMax.setOutlineColor(sf::Color::Red);
    cooldownMax.setOutlineThickness(2.f);

    cooldownAddUpText.setFont(font);
    cooldownAddUpText.setString("+" + toString(curCooldownUp) + "%");
    cooldownAddUpText.setCharacterSize(30);
    cooldownAddUpText.setFillColor(sf::Color::White);
    cooldownAddUpText.setPosition(350.f + (150.f - cooldownAddUpText.getGlobalBounds().getSize().x)/2, 500.f);

    cooldownAddButton.setSize(sf::Vector2f(50.f, 50.f));
    cooldownAddButton.setFillColor(sf::Color::Black);
    cooldownAddButton.setOutlineThickness(2.f);
    cooldownAddButton.setOutlineColor(sf::Color::White);
    cooldownAddButton.setPosition(500.f, 500.f);

    cooldownUpCostText.setFont(font);
    cooldownUpCostText.setString(toString(mCooldownUpCost[curCooldownUp/mCooldownUp]));
    cooldownUpCostText.setCharacterSize(15);
    cooldownUpCostText.setFillColor(sf::Color::White);
    cooldownUpCostText.setPosition(500.f + (50.f - cooldownUpCostText.getGlobalBounds().getSize().x)/2, 508.75);

    fireDamageText.setFont(font);
    fireDamageText.setString("Fire Damage");
    fireDamageText.setCharacterSize(30);
    fireDamageText.setFillColor(sf::Color::White);
    fireDamageText.setPosition(50.f, 575.f);

    fireDamageUp.setSize(sf::Vector2f(300.f, 50.f));
    percent = static_cast<float>(curFireDamageUp)/maxFireDamageUp;
    fireDamageUp.setScale(percent, 1.f);
    fireDamageUp.setFillColor(sf::Color::Red);
    fireDamageUp.setPosition(50.f, 625.f);

    fireDamageMax.setSize(sf::Vector2f(300.f, 50.f));
    fireDamageMax.setFillColor(sf::Color::Black);
    fireDamageMax.setPosition(50.f, 625.f);
    fireDamageMax.setOutlineColor(sf::Color::Red);
    fireDamageMax.setOutlineThickness(2.f);

    fireDamageAddUpText.setFont(font);
    fireDamageAddUpText.setString("+" + toString(curFireDamageUp));
    fireDamageAddUpText.setCharacterSize(30);
    fireDamageAddUpText.setFillColor(sf::Color::White);
    fireDamageAddUpText.setPosition(350.f + (150.f - fireDamageAddUpText.getGlobalBounds().getSize().x)/2, 625.f);

    fireDamageAddButton.setSize(sf::Vector2f(50.f, 50.f));
    fireDamageAddButton.setFillColor(sf::Color::Black);
    fireDamageAddButton.setOutlineThickness(2.f);
    fireDamageAddButton.setOutlineColor(sf::Color::White);
    fireDamageAddButton.setPosition(500.f, 625.f);

    fireDamageUpCostText.setFont(font);
    fireDamageUpCostText.setString(toString(mFireDamageUpCost[curFireDamageUp/mFireDamageUp]));
    fireDamageUpCostText.setCharacterSize(15);
    fireDamageUpCostText.setFillColor(sf::Color::White);
    fireDamageUpCostText.setPosition(500.f + (50.f - fireDamageUpCostText.getGlobalBounds().getSize().x)/2, 633.75);
}

void UpgradeState::initializeGUI2()
{
    sf::Font& font = getContext().fonts->get(Fonts::Main);

    abilityDamageText.setFont(font);
    abilityDamageText.setString("Ability Damage");
    abilityDamageText.setCharacterSize(30);
    abilityDamageText.setFillColor(sf::Color::White);
    abilityDamageText.setPosition(650.f, 200.f);

    abilityDamageUp.setSize(sf::Vector2f(300.f, 50.f));
    float percent = static_cast<float>(curAbilityDamageUp)/maxAbilityDamageUp;
    abilityDamageUp.setScale(percent, 1.f);
    abilityDamageUp.setFillColor(sf::Color::Red);
    abilityDamageUp.setPosition(650.f, 250.f);

    abilityDamageMax.setSize(sf::Vector2f(300.f, 50.f));
    abilityDamageMax.setFillColor(sf::Color::Black);
    abilityDamageMax.setPosition(650.f, 250.f);
    abilityDamageMax.setOutlineColor(sf::Color::Red);
    abilityDamageMax.setOutlineThickness(2.f);

    abilityDamageAddUpText.setFont(font);
    abilityDamageAddUpText.setString("+" + toString(curAbilityDamageUp));
    abilityDamageAddUpText.setCharacterSize(30);
    abilityDamageAddUpText.setFillColor(sf::Color::White);
    abilityDamageAddUpText.setPosition(950.f + (150.f - abilityDamageAddUpText.getGlobalBounds().getSize().x)/2, 250.f);

    abilityDamageAddButton.setSize(sf::Vector2f(50.f, 50.f));
    abilityDamageAddButton.setFillColor(sf::Color::Black);
    abilityDamageAddButton.setOutlineThickness(2.f);
    abilityDamageAddButton.setOutlineColor(sf::Color::White);
    abilityDamageAddButton.setPosition(1100.f, 250.f);

    abilityDamageUpCostText.setFont(font);
    abilityDamageUpCostText.setString(toString(mAbilityDamageUpCost[curAbilityDamageUp/mAbilityDamageUp]));
    abilityDamageUpCostText.setCharacterSize(15);
    abilityDamageUpCostText.setFillColor(sf::Color::White);
    abilityDamageUpCostText.setPosition(1100.f + (50.f - abilityDamageUpCostText.getGlobalBounds().getSize().x)/2, 258.75);

    debuffDamageText.setFont(font);
    debuffDamageText.setString("Debuff Damage");
    debuffDamageText.setCharacterSize(30);
    debuffDamageText.setFillColor(sf::Color::White);
    debuffDamageText.setPosition(650.f, 325.f);

    debuffDamageUp.setSize(sf::Vector2f(300.f, 50.f));
    percent = static_cast<float>(curDebuffDamageUp)/maxDebuffDamageUp;
    debuffDamageUp.setScale(percent, 1.f);
    debuffDamageUp.setFillColor(sf::Color::Red);
    debuffDamageUp.setPosition(650.f, 375.f);

    debuffDamageMax.setSize(sf::Vector2f(300.f, 50.f));
    debuffDamageMax.setFillColor(sf::Color::Black);
    debuffDamageMax.setPosition(650.f, 375.f);
    debuffDamageMax.setOutlineColor(sf::Color::Red);
    debuffDamageMax.setOutlineThickness(2.f);

    debuffDamageAddUpText.setFont(font);
    debuffDamageAddUpText.setString("+" + toString(curDebuffDamageUp));
    debuffDamageAddUpText.setCharacterSize(30);
    debuffDamageAddUpText.setFillColor(sf::Color::White);
    debuffDamageAddUpText.setPosition(950.f + (150.f - debuffDamageAddUpText.getGlobalBounds().getSize().x)/2, 375.f);

    debuffDamageAddButton.setSize(sf::Vector2f(50.f, 50.f));
    debuffDamageAddButton.setFillColor(sf::Color::Black);
    debuffDamageAddButton.setOutlineThickness(2.f);
    debuffDamageAddButton.setOutlineColor(sf::Color::White);
    debuffDamageAddButton.setPosition(1100.f, 375.f);

    debuffDamageUpCostText.setFont(font);
    debuffDamageUpCostText.setString(toString(mDebuffDamageUpCost[curDebuffDamageUp/mDebuffDamageUp]));
    debuffDamageUpCostText.setCharacterSize(15);
    debuffDamageUpCostText.setFillColor(sf::Color::White);
    debuffDamageUpCostText.setPosition(1100.f + (50.f - debuffDamageUpCostText.getGlobalBounds().getSize().x)/2, 383.75);

    ultimateDamageText.setFont(font);
    ultimateDamageText.setString("Ultimate Damage");
    ultimateDamageText.setCharacterSize(30);
    ultimateDamageText.setFillColor(sf::Color::White);
    ultimateDamageText.setPosition(650.f, 450.f);

    ultimateDamageUp.setSize(sf::Vector2f(300.f, 50.f));
    percent = static_cast<float>(curUltimateDamageUp)/maxUltimateDamageUp;
    ultimateDamageUp.setScale(percent, 1.f);
    ultimateDamageUp.setFillColor(sf::Color::Red);
    ultimateDamageUp.setPosition(650.f, 500.f);

    ultimateDamageMax.setSize(sf::Vector2f(300.f, 50.f));
    ultimateDamageMax.setFillColor(sf::Color::Black);
    ultimateDamageMax.setPosition(650.f, 500.f);
    ultimateDamageMax.setOutlineColor(sf::Color::Red);
    ultimateDamageMax.setOutlineThickness(2.f);

    ultimateDamageAddUpText.setFont(font);
    ultimateDamageAddUpText.setString("+" + toString(curUltimateDamageUp));
    ultimateDamageAddUpText.setCharacterSize(30);
    ultimateDamageAddUpText.setFillColor(sf::Color::White);
    ultimateDamageAddUpText.setPosition(950.f + (150.f - ultimateDamageAddUpText.getGlobalBounds().getSize().x)/2, 500.f);

    ultimateDamageAddButton.setSize(sf::Vector2f(50.f, 50.f));
    ultimateDamageAddButton.setFillColor(sf::Color::Black);
    ultimateDamageAddButton.setOutlineThickness(2.f);
    ultimateDamageAddButton.setOutlineColor(sf::Color::White);
    ultimateDamageAddButton.setPosition(1100.f, 500.f);

    ultimateDamageUpCostText.setFont(font);
    ultimateDamageUpCostText.setString(toString(mUltimateDamageUpCost[curUltimateDamageUp/mUltimateDamageUp]));
    ultimateDamageUpCostText.setCharacterSize(15);
    ultimateDamageUpCostText.setFillColor(sf::Color::White);
    ultimateDamageUpCostText.setPosition(1100.f + (50.f - ultimateDamageUpCostText.getGlobalBounds().getSize().x)/2, 508.75);
}

void UpgradeState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(mBackgroundSprite);
    window.draw(mBlur);
    window.draw(mExit);
    window.draw(mExitText);
    window.draw(mUpgradePointsText);

    window.draw(hpText);
    window.draw(hpAddUpText);
    window.draw(hpMax);
    window.draw(hpUp);
    window.draw(hpAddButton);
    window.draw(hpUpCostText);

    window.draw(speedText);
    window.draw(speedAddUpText);
    window.draw(speedMax);
    window.draw(speedUp);
    window.draw(speedAddButton);
    window.draw(speedUpCostText);

    window.draw(cooldownText);
    window.draw(cooldownAddUpText);
    window.draw(cooldownMax);
    window.draw(cooldownUp);
    window.draw(cooldownAddButton);
    window.draw(cooldownUpCostText);

    window.draw(fireDamageText);
    window.draw(fireDamageAddUpText);
    window.draw(fireDamageMax);
    window.draw(fireDamageUp);
    window.draw(fireDamageAddButton);
    window.draw(fireDamageUpCostText);

    window.draw(abilityDamageText);
    window.draw(abilityDamageAddUpText);
    window.draw(abilityDamageMax);
    window.draw(abilityDamageUp);
    window.draw(abilityDamageAddButton);
    window.draw(abilityDamageUpCostText);

    window.draw(debuffDamageText);
    window.draw(debuffDamageAddUpText);
    window.draw(debuffDamageMax);
    window.draw(debuffDamageUp);
    window.draw(debuffDamageAddButton);
    window.draw(debuffDamageUpCostText);

    window.draw(ultimateDamageText);
    window.draw(ultimateDamageAddUpText);
    window.draw(ultimateDamageMax);
    window.draw(ultimateDamageUp);
    window.draw(ultimateDamageAddButton);
    window.draw(ultimateDamageUpCostText);
}

bool UpgradeState::update(sf::Time)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    if(mExit.getGlobalBounds().contains(mousePosF))
    {
        mExit.setFillColor(sf::Color::White);
        mExitText.setFillColor(sf::Color::Black);
    }
    else
    {
        mExit.setFillColor(sf::Color::Black);
        mExitText.setFillColor(sf::Color::White);
    }

    if (hpAddButton.getGlobalBounds().contains(mousePosF))
    {
        hpAddButton.setFillColor(sf::Color::White);
        hpAddButton.setOutlineColor(sf::Color::Black);
        hpUpCostText.setFillColor(sf::Color::Black);
    }
    else
    {
        hpAddButton.setFillColor(sf::Color::Black);
        hpAddButton.setOutlineColor(sf::Color::White);
        hpUpCostText.setFillColor(sf::Color::White);
    }

    if (speedAddButton.getGlobalBounds().contains(mousePosF))
    {
        speedAddButton.setFillColor(sf::Color::White);
        speedAddButton.setOutlineColor(sf::Color::Black);
        speedUpCostText.setFillColor(sf::Color::Black);
    }
    else
    {
        speedAddButton.setFillColor(sf::Color::Black);
        speedAddButton.setOutlineColor(sf::Color::White);
        speedUpCostText.setFillColor(sf::Color::White);
    }

    if (cooldownAddButton.getGlobalBounds().contains(mousePosF))
    {
        cooldownAddButton.setFillColor(sf::Color::White);
        cooldownAddButton.setOutlineColor(sf::Color::Black);
        cooldownUpCostText.setFillColor(sf::Color::Black);
    }
    else
    {
        cooldownAddButton.setFillColor(sf::Color::Black);
        cooldownAddButton.setOutlineColor(sf::Color::White);
        cooldownUpCostText.setFillColor(sf::Color::White);
    }

    if (fireDamageAddButton.getGlobalBounds().contains(mousePosF))
    {
        fireDamageAddButton.setFillColor(sf::Color::White);
        fireDamageAddButton.setOutlineColor(sf::Color::Black);
        fireDamageUpCostText.setFillColor(sf::Color::Black);
    }
    else
    {
        fireDamageAddButton.setFillColor(sf::Color::Black);
        fireDamageAddButton.setOutlineColor(sf::Color::White);
        fireDamageUpCostText.setFillColor(sf::Color::White);
    }

    if (abilityDamageAddButton.getGlobalBounds().contains(mousePosF))
    {
        abilityDamageAddButton.setFillColor(sf::Color::White);
        abilityDamageAddButton.setOutlineColor(sf::Color::Black);
        abilityDamageUpCostText.setFillColor(sf::Color::Black);
    }
    else
    {
        abilityDamageAddButton.setFillColor(sf::Color::Black);
        abilityDamageAddButton.setOutlineColor(sf::Color::White);
        abilityDamageUpCostText.setFillColor(sf::Color::White);
    }

    if (debuffDamageAddButton.getGlobalBounds().contains(mousePosF))
    {
        debuffDamageAddButton.setFillColor(sf::Color::White);
        debuffDamageAddButton.setOutlineColor(sf::Color::Black);
        debuffDamageUpCostText.setFillColor(sf::Color::Black);
    }
    else
    {
        debuffDamageAddButton.setFillColor(sf::Color::Black);
        debuffDamageAddButton.setOutlineColor(sf::Color::White);
        debuffDamageUpCostText.setFillColor(sf::Color::White);
    }

    if (ultimateDamageAddButton.getGlobalBounds().contains(mousePosF))
    {
        ultimateDamageAddButton.setFillColor(sf::Color::White);
        ultimateDamageAddButton.setOutlineColor(sf::Color::Black);
        ultimateDamageUpCostText.setFillColor(sf::Color::Black);
    }
    else
    {
        ultimateDamageAddButton.setFillColor(sf::Color::Black);
        ultimateDamageAddButton.setOutlineColor(sf::Color::White);
        ultimateDamageUpCostText.setFillColor(sf::Color::White);
    }

    return false;
}

void UpgradeState::saveData()
{
    std::ofstream ofs("Data/Upgrade/mUpgradePoints.dat");
    ofs << mUpgradePoints;
    ofs.close();

    std::ofstream ofs2("Data/Upgrade/BlueWitch/UpgradePower.dat");
    ofs2 << curHitpointUp << " " << maxHitpointUp << " " << mHitpointUp << std::endl;
    ofs2 << curSpeedUp << " " << maxSpeedUp << " " << mSpeedUp << std::endl;
    ofs2 << curCooldownUp << " " << maxCooldownUp << " " << mCooldownUp << std::endl;
    ofs2.close();

    std::ofstream ofs3("Data/Upgrade/BlueWitch/ProjectilePower.dat");
    ofs3 << curFireDamageUp << " " << maxFireDamageUp << " " << mFireDamageUp << std::endl;
    ofs3 << curAbilityDamageUp << " " << maxAbilityDamageUp << " " << mAbilityDamageUp << std::endl;
    ofs3 << curDebuffDamageUp << " " << maxDebuffDamageUp << " " << mDebuffDamageUp << std::endl;
    ofs3 << curUltimateDamageUp << " " << maxUltimateDamageUp << " " << mUltimateDamageUp << std::endl;
    ofs3.close();

    std::ofstream ofs4("Data/Upgrade/BlueWitch/UpgradeCost.dat");
    for(int i=0; i<10; i++) ofs4 << mHitpointUpCost[i] << " ";
    ofs4 << std::endl;
    for(int i=0; i<10; i++) ofs4 << mSpeedUpCost[i] << " ";
    ofs4 << std::endl;
    for(int i=0; i<10; i++) ofs4 << mCooldownUpCost[i] << " ";
    ofs4 << std::endl;
    for(int i=0; i<10; i++) ofs4 << mFireDamageUpCost[i] << " ";
    ofs4 << std::endl;
    for(int i=0; i<10; i++) ofs4 << mAbilityDamageUpCost[i] << " ";
    ofs4 << std::endl;
    for(int i=0; i<10; i++) ofs4 << mDebuffDamageUpCost[i] << " ";
    ofs4 << std::endl;
    for(int i=0; i<10; i++) ofs4 << mUltimateDamageUpCost[i] << " ";
    ofs4 << std::endl;
    ofs4.close();
}

bool UpgradeState::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        if (mExit.getGlobalBounds().contains(mousePosF))
        {
            requestStackPop();
        }
        else if (hpAddButton.getGlobalBounds().contains(mousePosF))
        {
            int curLevel = curHitpointUp / mHitpointUp;
            if(mUpgradePoints >= mHitpointUpCost[curLevel] && curHitpointUp < maxHitpointUp)
            {
                mUpgradePoints -= mHitpointUpCost[curLevel];
                curHitpointUp += mHitpointUp;
                mUpgradePointsText.setString("Upgrade Points: " + toString(mUpgradePoints));
                hpAddUpText.setString("+" + toString(curHitpointUp));
                float percent = static_cast<float>(curHitpointUp)/maxHitpointUp;
                hpUp.setScale(percent, 1.f);
                hpUpCostText.setString(toString(mHitpointUpCost[curHitpointUp/mHitpointUp]));
                hpUpCostText.setPosition(500.f + (50.f - hpUpCostText.getGlobalBounds().getSize().x)/2, 258.75);
                saveData();
            }
        }
        else if (speedAddButton.getGlobalBounds().contains(mousePosF))
        {
            int curLevel = curSpeedUp / mSpeedUp;
            if(mUpgradePoints >= mSpeedUpCost[curLevel] && curSpeedUp < maxSpeedUp)
            {
                mUpgradePoints -= mSpeedUpCost[curLevel];
                curSpeedUp += mSpeedUp;
                mUpgradePointsText.setString("Upgrade Points: " + toString(mUpgradePoints));
                speedAddUpText.setString("+" + toString(curSpeedUp));
                float percent = static_cast<float>(curSpeedUp)/maxSpeedUp;
                speedUp.setScale(percent, 1.f);
                speedUpCostText.setString(toString(mSpeedUpCost[curSpeedUp/mSpeedUp]));
                speedUpCostText.setPosition(500.f + (50.f - speedUpCostText.getGlobalBounds().getSize().x)/2, 383.75);
                saveData();
            }
        }
        else if (cooldownAddButton.getGlobalBounds().contains(mousePosF))
        {
            int curLevel = curCooldownUp / mCooldownUp;
            if(mUpgradePoints >= mCooldownUpCost[curLevel] && curCooldownUp < maxCooldownUp)
            {
                mUpgradePoints -= mCooldownUpCost[curLevel];
                curCooldownUp += mCooldownUp;
                mUpgradePointsText.setString("Upgrade Points: " + toString(mUpgradePoints));
                cooldownAddUpText.setString("+" + toString(curCooldownUp) + "%");
                float percent = static_cast<float>(curCooldownUp)/maxCooldownUp;
                cooldownUp.setScale(percent, 1.f);
                cooldownUpCostText.setString(toString(mCooldownUpCost[curCooldownUp/mCooldownUp]));
                cooldownUpCostText.setPosition(500.f + (50.f - cooldownUpCostText.getGlobalBounds().getSize().x)/2, 508.75);
                saveData();
            }
        }
        else if (fireDamageAddButton.getGlobalBounds().contains(mousePosF))
        {
            int curLevel = curFireDamageUp / mFireDamageUp;
            if(mUpgradePoints >= mFireDamageUpCost[curLevel] && curFireDamageUp < maxFireDamageUp)
            {
                mUpgradePoints -= mFireDamageUpCost[curLevel];
                curFireDamageUp += mFireDamageUp;
                mUpgradePointsText.setString("Upgrade Points: " + toString(mUpgradePoints));
                fireDamageAddUpText.setString("+" + toString(curFireDamageUp));
                float percent = static_cast<float>(curFireDamageUp)/maxFireDamageUp;
                fireDamageUp.setScale(percent, 1.f);
                fireDamageUpCostText.setString(toString(mFireDamageUpCost[curFireDamageUp/mFireDamageUp]));
                fireDamageUpCostText.setPosition(500.f + (50.f - fireDamageUpCostText.getGlobalBounds().getSize().x)/2, 633.75);
                saveData();
            }
        }
        else if (abilityDamageAddButton.getGlobalBounds().contains(mousePosF))
        {
            int curLevel = curAbilityDamageUp / mAbilityDamageUp;
            if(mUpgradePoints >= mAbilityDamageUpCost[curLevel] && curAbilityDamageUp < maxAbilityDamageUp)
            {
                mUpgradePoints -= mAbilityDamageUpCost[curLevel];
                curAbilityDamageUp += mAbilityDamageUp;
                mUpgradePointsText.setString("Upgrade Points: " + toString(mUpgradePoints));
                abilityDamageAddUpText.setString("+" + toString(curAbilityDamageUp));
                float percent = static_cast<float>(curAbilityDamageUp)/maxAbilityDamageUp;
                abilityDamageUp.setScale(percent, 1.f);
                abilityDamageUpCostText.setString(toString(mAbilityDamageUpCost[curAbilityDamageUp/mAbilityDamageUp]));
                abilityDamageUpCostText.setPosition(1100.f + (50.f - abilityDamageUpCostText.getGlobalBounds().getSize().x)/2, 258.75);
                saveData();
            }
        }
        else if (debuffDamageAddButton.getGlobalBounds().contains(mousePosF))
        {
            int curLevel = curDebuffDamageUp / mDebuffDamageUp;
            if(mUpgradePoints >= mDebuffDamageUpCost[curLevel] && curDebuffDamageUp < maxDebuffDamageUp)
            {
                mUpgradePoints -= mDebuffDamageUpCost[curLevel];
                curDebuffDamageUp += mDebuffDamageUp;
                mUpgradePointsText.setString("Upgrade Points: " + toString(mUpgradePoints));
                debuffDamageAddUpText.setString("+" + toString(curDebuffDamageUp));
                float percent = static_cast<float>(curDebuffDamageUp)/maxDebuffDamageUp;
                debuffDamageUp.setScale(percent, 1.f);
                debuffDamageUpCostText.setString(toString(mDebuffDamageUpCost[curDebuffDamageUp/mDebuffDamageUp]));
                debuffDamageUpCostText.setPosition(1100.f + (50.f - debuffDamageUpCostText.getGlobalBounds().getSize().x)/2, 383.75);
                saveData();
            }
        }
        else if (ultimateDamageAddButton.getGlobalBounds().contains(mousePosF))
        {
            int curLevel = curUltimateDamageUp / mUltimateDamageUp;
            if(mUpgradePoints >= mUltimateDamageUpCost[curLevel] && curUltimateDamageUp < maxUltimateDamageUp)
            {
                mUpgradePoints -= mUltimateDamageUpCost[curLevel];
                curUltimateDamageUp += mUltimateDamageUp;
                mUpgradePointsText.setString("Upgrade Points: " + toString(mUpgradePoints));
                ultimateDamageAddUpText.setString("+" + toString(curUltimateDamageUp));
                float percent = static_cast<float>(curUltimateDamageUp)/maxUltimateDamageUp;
                ultimateDamageUp.setScale(percent, 1.f);
                ultimateDamageUpCostText.setString(toString(mUltimateDamageUpCost[curUltimateDamageUp/mUltimateDamageUp]));
                ultimateDamageUpCostText.setPosition(1100.f + (50.f - ultimateDamageUpCostText.getGlobalBounds().getSize().x)/2, 508.75);
                saveData();
            }
        }
    }
    return false;
}