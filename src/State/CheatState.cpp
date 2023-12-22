#include "include/CheatState.hpp"
#include "include/Utility.hpp"

CheatState::CheatState(StateStack& stack, Context context)
: State(stack, context)
, mBackground()
, mInvicible()
, mPlayedTime()
, mPlayer(*context.player)
{
    sf::Font& font = context.fonts->get(Fonts::Main);
    sf::Vector2f windowSize(context.window->getSize());

    
    mBackground.setSize(windowSize);
    mBackground.setFillColor(sf::Color(0, 0, 0, 150));
    
    mInvicible.setFont(font);
    std::string isInvicible = (getContext().player->isInvicible() ? "ON" : "OFF");
    mInvicible.setString("Invicible :" + isInvicible);
    mInvicible.setCharacterSize(30);
    mInvicible.setPosition(100,100);
    
    mInvicibleButton.setSize(sf::Vector2f(100, 50));
    mInvicibleButton.setFillColor(sf::Color::Red);
    mInvicibleButton.setPosition(100 + mInvicible.getGlobalBounds().width + 100, 100);

    mInvicibleButtonText.setFont(font);
    mInvicibleButtonText.setString("ON/OFF");
    mInvicibleButtonText.setCharacterSize(30);
    mInvicibleButtonText.setPosition(100 + mInvicible.getGlobalBounds().width + 105, 105);
    
    mPlayedTime.setFont(font);
    std::string mMinute = std::to_string((int)(getContext().player->getPlayedTime().asSeconds() / 60));
    std::string mSecond = std::to_string((int)(getContext().player->getPlayedTime().asSeconds()) % 60);
    mPlayedTime.setString("Played Time" + mMinute + ":" + mSecond);
    mPlayedTime.setCharacterSize(30);
    mPlayedTime.setPosition(100,200);

    mPlayedTimedMinus30.setSize(sf::Vector2f(50, 50));
    mPlayedTimedMinus30.setFillColor(sf::Color::Red);
    mPlayedTimedMinus30.setPosition(100 + mPlayedTime.getGlobalBounds().width + 50, 200);

    mPlayedTimedMinus30Text.setFont(font);
    mPlayedTimedMinus30Text.setString("-30s");
    mPlayedTimedMinus30Text.setCharacterSize(20);
    mPlayedTimedMinus30Text.setPosition(100 + mPlayedTime.getGlobalBounds().width + 55, 205);

    mPlayedTimedMinus15.setSize(sf::Vector2f(50, 50));
    mPlayedTimedMinus15.setFillColor(sf::Color::Red);
    mPlayedTimedMinus15.setPosition(100 + mPlayedTime.getGlobalBounds().width + 150, 200);

    mPlayedTimedMinus15Text.setFont(font);
    mPlayedTimedMinus15Text.setString("-15s");
    mPlayedTimedMinus15Text.setCharacterSize(20);
    mPlayedTimedMinus15Text.setPosition(100 + mPlayedTime.getGlobalBounds().width + 155, 205);

    mPlayedTimedMinus1.setSize(sf::Vector2f(50, 50));
    mPlayedTimedMinus1.setFillColor(sf::Color::Red);
    mPlayedTimedMinus1.setPosition(100 + mPlayedTime.getGlobalBounds().width + 250, 200);

    mPlayedTimedMinus1Text.setFont(font);
    mPlayedTimedMinus1Text.setString("-1s");
    mPlayedTimedMinus1Text.setCharacterSize(20);
    mPlayedTimedMinus1Text.setPosition(100 + mPlayedTime.getGlobalBounds().width + 255, 205);
    
    mPlayedTimedPlus1.setSize(sf::Vector2f(50, 50));
    mPlayedTimedPlus1.setFillColor(sf::Color::Red);
    mPlayedTimedPlus1.setPosition(100 + mPlayedTime.getGlobalBounds().width + 350, 200);

    mPlayedTimedPlus1Text.setFont(font);
    mPlayedTimedPlus1Text.setString("+1s");
    mPlayedTimedPlus1Text.setCharacterSize(20);
    mPlayedTimedPlus1Text.setPosition(100 + mPlayedTime.getGlobalBounds().width + 355, 205);

    mPlayedTimedPlus15.setSize(sf::Vector2f(50, 50));
    mPlayedTimedPlus15.setFillColor(sf::Color::Red);
    mPlayedTimedPlus15.setPosition(100 + mPlayedTime.getGlobalBounds().width + 450, 200);

    mPlayedTimedPlus15Text.setFont(font);
    mPlayedTimedPlus15Text.setString("+15s");
    mPlayedTimedPlus15Text.setCharacterSize(20);
    mPlayedTimedPlus15Text.setPosition(100 + mPlayedTime.getGlobalBounds().width + 455, 205);

    mPlayedTimedPlus30.setSize(sf::Vector2f(50, 50));
    mPlayedTimedPlus30.setFillColor(sf::Color::Red);
    mPlayedTimedPlus30.setPosition(100 + mPlayedTime.getGlobalBounds().width + 550, 200);

    mPlayedTimedPlus30Text.setFont(font);
    mPlayedTimedPlus30Text.setString("+30s");
    mPlayedTimedPlus30Text.setCharacterSize(20);
    mPlayedTimedPlus30Text.setPosition(100 + mPlayedTime.getGlobalBounds().width + 555, 205);
}

bool CheatState::handleEvent(const sf::Event& event)
{
    sf::Vector2f mousePosition = getContext().window->mapPixelToCoords(sf::Mouse::getPosition(*getContext().window));

    if (event.key.code == sf::Keyboard::Escape)
    {
        requestStackPop();
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        if (mInvicibleButton.getGlobalBounds().contains(mousePosition))
        {
            getContext().player->setInvicible(!getContext().player->isInvicible());
            std::string isInvicible = (getContext().player->isInvicible() ? "ON" : "OFF");
            mInvicible.setString("Invicible :" + isInvicible);
        }
        if (mPlayedTimedMinus30.getGlobalBounds().contains(mousePosition))
        {
            mPlayer.subtractPlayedTime(sf::seconds(30));
            std::string mMinute = std::to_string((int)(mPlayer.getPlayedTime().asSeconds() / 60));
            std::string mSecond = std::to_string((int)(mPlayer.getPlayedTime().asSeconds()) % 60);
            mPlayedTime.setString("Played Time" + mMinute + ":" + mSecond);
        }
        if (mPlayedTimedPlus30.getGlobalBounds().contains(mousePosition))
        {
            mPlayer.addPlayedTime(sf::seconds(30));
            std::string mMinute = std::to_string((int)(mPlayer.getPlayedTime().asSeconds() / 60));
            std::string mSecond = std::to_string((int)(mPlayer.getPlayedTime().asSeconds()) % 60);
            mPlayedTime.setString("Played Time" + mMinute + ":" + mSecond);
        }
        if (mPlayedTimedMinus15.getGlobalBounds().contains(mousePosition))
        {
            mPlayer.subtractPlayedTime(sf::seconds(15));
            std::string mMinute = std::to_string((int)(mPlayer.getPlayedTime().asSeconds() / 60));
            std::string mSecond = std::to_string((int)(mPlayer.getPlayedTime().asSeconds()) % 60);
            mPlayedTime.setString("Played Time" + mMinute + ":" + mSecond);
        }
        if (mPlayedTimedPlus15.getGlobalBounds().contains(mousePosition))
        {
            mPlayer.addPlayedTime(sf::seconds(15));
            std::string mMinute = std::to_string((int)(mPlayer.getPlayedTime().asSeconds() / 60));
            std::string mSecond = std::to_string((int)(mPlayer.getPlayedTime().asSeconds()) % 60);
            mPlayedTime.setString("Played Time" + mMinute + ":" + mSecond);
        }
        if (mPlayedTimedMinus1.getGlobalBounds().contains(mousePosition))
        {
            mPlayer.subtractPlayedTime(sf::seconds(1));
            std::string mMinute = std::to_string((int)(mPlayer.getPlayedTime().asSeconds() / 60));
            std::string mSecond = std::to_string((int)(mPlayer.getPlayedTime().asSeconds()) % 60);
            mPlayedTime.setString("Played Time" + mMinute + ":" + mSecond);
        }
        if (mPlayedTimedPlus1.getGlobalBounds().contains(mousePosition))
        {
            mPlayer.addPlayedTime(sf::seconds(1));
            std::string mMinute = std::to_string((int)(mPlayer.getPlayedTime().asSeconds() / 60));
            std::string mSecond = std::to_string((int)(mPlayer.getPlayedTime().asSeconds()) % 60);
            mPlayedTime.setString("Played Time" + mMinute + ":" + mSecond);
        }
    }
    return false;
}

bool CheatState::update(sf::Time)
{
    sf::Vector2f mousePosition = getContext().window->mapPixelToCoords(sf::Mouse::getPosition(*getContext().window));

    if (mInvicibleButton.getGlobalBounds().contains(mousePosition))
    {
        mInvicibleButton.setFillColor(sf::Color::Green);
    }
    else
    {
        mInvicibleButton.setFillColor(sf::Color::Red);
    }

    if (mPlayedTimedMinus30.getGlobalBounds().contains(mousePosition))
    {
        mPlayedTimedMinus30.setFillColor(sf::Color::Green);
    }
    else
    {
        mPlayedTimedMinus30.setFillColor(sf::Color::Red);
    }

    if (mPlayedTimedPlus30.getGlobalBounds().contains(mousePosition))
    {
        mPlayedTimedPlus30.setFillColor(sf::Color::Green);
    }
    else
    {
        mPlayedTimedPlus30.setFillColor(sf::Color::Red);
    }

    if (mPlayedTimedMinus15.getGlobalBounds().contains(mousePosition))
    {
        mPlayedTimedMinus15.setFillColor(sf::Color::Green);
    }
    else
    {
        mPlayedTimedMinus15.setFillColor(sf::Color::Red);
    }

    if (mPlayedTimedPlus15.getGlobalBounds().contains(mousePosition))
    {
        mPlayedTimedPlus15.setFillColor(sf::Color::Green);
    }
    else
    {
        mPlayedTimedPlus15.setFillColor(sf::Color::Red);
    }

    if (mPlayedTimedMinus1.getGlobalBounds().contains(mousePosition))
    {
        mPlayedTimedMinus1.setFillColor(sf::Color::Green);
    }
    else
    {
        mPlayedTimedMinus1.setFillColor(sf::Color::Red);
    }

    if (mPlayedTimedPlus1.getGlobalBounds().contains(mousePosition))
    {
        mPlayedTimedPlus1.setFillColor(sf::Color::Green);
    }
    else
    {
        mPlayedTimedPlus1.setFillColor(sf::Color::Red);
    }

    return false;
}

void CheatState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    
    window.draw(mBackground);
    window.draw(mInvicible);
    window.draw(mInvicibleButton);
    window.draw(mPlayedTime);
    window.draw(mPlayedTimedMinus30);
    window.draw(mPlayedTimedMinus15);
    window.draw(mPlayedTimedMinus1);
    window.draw(mPlayedTimedPlus1);
    window.draw(mPlayedTimedPlus15);
    window.draw(mPlayedTimedPlus30);
    window.draw(mInvicibleButtonText);
    window.draw(mPlayedTimedMinus30Text);
    window.draw(mPlayedTimedMinus15Text);
    window.draw(mPlayedTimedMinus1Text);
    window.draw(mPlayedTimedPlus1Text);
    window.draw(mPlayedTimedPlus15Text);
    window.draw(mPlayedTimedPlus30Text);
}