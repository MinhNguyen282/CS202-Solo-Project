#include "CreditState.hpp"
#include "Utility.hpp"
#include "Button.hpp"
#include "ResourcesHolder.hpp"

#include <SFML/Graphics.hpp>

CreditState::CreditState(StateStack& stack, Context context)
: State(stack, context)
, mGUIContainer()
{
    mBackgroundSprite.setTexture(context.textures->get(Textures::Title));
    mPanel.setTexture(context.textures->get(Textures::bigPanel));
	mPanel.setPosition((1200 - mPanel.getGlobalBounds().width)/2, 100);

    auto backButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures, Textures::smallButtonNormal, Textures::smallButtonSelected, Textures::smallButtonPressed);
    backButton->setPosition(100, 100);
    backButton->setText("Back");
    backButton->setTextColor(sf::Color::Black);
    backButton->setTextSize(20);
    backButton->setTextPosition((backButton->getBoundingRect().width - backButton->getTextBounding().width)/2,(backButton->getBoundingRect().height - backButton->getTextBounding().height)/4);
    backButton->setCallback([this] ()
    {
        requestStackPop();
    });

    mGUIContainer.pack(backButton);
}

bool CreditState::handleEvent(const sf::Event& event)
{
    mGUIContainer.handleEvent(event);
    return false;
}

bool CreditState::update(sf::Time deltaTime)
{
    return false;
}

void CreditState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackgroundSprite);
    window.draw(mPanel);
    window.draw(mText);
    window.draw(mGUIContainer);
}
