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
	if (key == sf::Keyboard::W || key == sf::Keyboard::Up)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::S || key == sf::Keyboard::Down)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::A || key == sf::Keyboard::Left)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D || key == sf::Keyboard::Right)
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
		/*move the craft to the position, in lab 2 this was done simply by me doing :
		sf::Vector2f direction = (MousePos)-mPlayer.getPosition();
		mPlayer.move(direction * elapsedTime.asSeconds());*/

		movement.x = MousePos.x;
		movement.y = -MousePos.y;
		std::cout << MousePos.y << endl;
		//sf::Vector2f movement = (MousePos) + mWorld.getAircraft()->getPosition();
		//direction = direction / sqrt((std::pow(direction.x, 2.f)) + (std::pow(direction.y, 2.f)));
		mWorld.getAircraft()->move(movement * elapsed.asSeconds());
		//mWorld.getAircraft()->move(movement);
		//mWorld.getAircraft()->setVelocity(40.f, -350.f * direction.y);
		mWorld.getAircraft()->setVelocity(40.f, -350.f);
		//mWorld.getAircraft()->setVelocity(direction);
	} else {
		mWorld.getAircraft()->move(movement * elapsed.asSeconds());
	}
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
				sf::Vector2i pix_pos = sf::Mouse::getPosition(mWindow);
				MousePos = mWindow.mapPixelToCoords(pix_pos, mWindow.getView());
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
