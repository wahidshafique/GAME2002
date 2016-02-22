#include <Book/Game.hpp>
#include <Book/StringHelpers.hpp>
#include <iostream>
#include <Book/World.hpp>

#include <SFML/Window/Event.hpp>

const float PLAYERSPEED = 85.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: mWindow(sf::VideoMode(640, 480), "World", sf::Style::Close)
	, mWorld(mWindow)
	, mFont()
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
	, mIsMovingUp()
	, mIsMovingDown()
	, mIsMovingLeft()
	, mIsMovingRight()
	, mIsClicked()
{
	mFont.loadFromFile("Media/Sansation.ttf");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);

		}

		updateStatistics(elapsedTime);
		render();
	}
}


//A1 handle the keyboard presses and mouse
void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::W)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::S)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::A)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D)
		mIsMovingRight = isPressed;
}
using namespace std;
//A1 act upon the keyboard and mouse presses
void Game::doPlayerInput(sf::Time elapsed) {

	sf::Vector2f movement(0.f, 0.f);
	if (mIsMovingUp)
		movement.y -= PLAYERSPEED;
	if (mIsMovingDown)
		movement.y += PLAYERSPEED;
	if (mIsMovingLeft)
		movement.x -= PLAYERSPEED;
	if (mIsMovingRight)
		movement.x += PLAYERSPEED;
	if (mIsClicked) {
		sf::Vector2f direction = (MousePos) - mWorld.getAircraft()->getPosition();
		mWorld.getAircraft()->setPosition(direction);
		//mWorld.getAircraft()->move(direction * elapsed.asSeconds() / 20.f);
		//mIsClicked = false;
	}
	mWorld.getAircraft()->move(movement * elapsed.asSeconds());

}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left) {
				//Assignment1 
				MousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
				mIsClicked = true;
			}
			break;
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Game::update(sf::Time elapsedTime)
{
	mWorld.update(elapsedTime);
	doPlayerInput(elapsedTime);
}

void Game::render()
{
	mWindow.clear();
	mWorld.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);
	mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}
