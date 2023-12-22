#ifndef UPGRADESTATE_HPP
#define UPGRADESTATE_HPP

#include "State.hpp"

#include <SFML/Graphics.hpp>
#include <fstream>
#include <array>

class UpgradeState : public State
{
    public:
        UpgradeState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time deltaTime);
        virtual bool handleEvent(const sf::Event& event);
    private:
        void initializeUpgrade();
        void initializeGUI();
        void initializeGUI2();
        void initializeUpgradeCost();
        void saveData();
    private:
        int mUpgradePoints;

        sf::Sprite mBackgroundSprite;
        sf::RectangleShape mBlur;
        sf::RectangleShape mExit;
        sf::Text mExitText;

        sf::Text mUpgradePointsText;

        sf::Text hpText;
        sf::Text hpAddUpText;
        sf::RectangleShape hpUp;
        sf::RectangleShape hpMax;
        sf::RectangleShape hpAddButton;
        sf::Text hpUpCostText;

        sf::Text speedText;
        sf::Text speedAddUpText;
        sf::RectangleShape speedUp;
        sf::RectangleShape speedMax;
        sf::RectangleShape speedAddButton;
        sf::Text speedUpCostText;

        sf::Text cooldownText;
        sf::Text cooldownAddUpText;
        sf::RectangleShape cooldownUp;
        sf::RectangleShape cooldownMax;
        sf::RectangleShape cooldownAddButton;
        sf::Text cooldownUpCostText;

        sf::Text fireDamageText;
        sf::Text fireDamageAddUpText;
        sf::RectangleShape fireDamageUp;
        sf::RectangleShape fireDamageMax;
        sf::RectangleShape fireDamageAddButton;
        sf::Text fireDamageUpCostText;

        sf::Text abilityDamageText;
        sf::Text abilityDamageAddUpText;
        sf::RectangleShape abilityDamageUp;
        sf::RectangleShape abilityDamageMax;
        sf::RectangleShape abilityDamageAddButton;
        sf::Text abilityDamageUpCostText;

        sf::Text debuffDamageText;
        sf::Text debuffDamageAddUpText;
        sf::RectangleShape debuffDamageUp;
        sf::RectangleShape debuffDamageMax;
        sf::RectangleShape debuffDamageAddButton;
        sf::Text debuffDamageUpCostText;

        sf::Text ultimateDamageText;
        sf::Text ultimateDamageAddUpText;
        sf::RectangleShape ultimateDamageUp;
        sf::RectangleShape ultimateDamageMax;
        sf::RectangleShape ultimateDamageAddButton;
        sf::Text ultimateDamageUpCostText;

        std::vector<sf::RectangleShape> mGUI;
        std::vector<sf::Text> mGUIText;
        std::vector<sf::RectangleShape> mGUIButtons;

        int maxHitpointUp;
        int mHitpointUp;
        int curHitpointUp;
        std::array<int, 10> mHitpointUpCost;

        int maxSpeedUp;
        int mSpeedUp;
        int curSpeedUp;
        std::array<int, 10> mSpeedUpCost;
        
        int maxCooldownUp;
        int mCooldownUp;
        int curCooldownUp;
        std::array<int, 10> mCooldownUpCost;
        
        int maxFireDamageUp;
        int mFireDamageUp;
        int curFireDamageUp;
        std::array<int, 10> mFireDamageUpCost;

        int maxAbilityDamageUp;
        int mAbilityDamageUp;
        int curAbilityDamageUp;
        std::array<int, 10> mAbilityDamageUpCost;

        int maxDebuffDamageUp;
        int mDebuffDamageUp;
        int curDebuffDamageUp;
        std::array<int, 10> mDebuffDamageUpCost;

        int maxUltimateDamageUp;
        int mUltimateDamageUp;
        int curUltimateDamageUp;
        std::array<int, 10> mUltimateDamageUpCost;
};

#endif // UPGRADESTATE_HPP