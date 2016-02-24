#ifndef BOOK_WORLD_HPP
#define BOOK_WORLD_HPP

#include <Book/ResourceHolder.hpp>
#include <Book/ResourceIdentifiers.hpp>
#include <Book/SceneNode.hpp>
#include <Book/SpriteNode.hpp>
#include <Book/Aircraft.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics.hpp>

#include <array>


// Forward declaration
namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
	public:
		explicit							World(sf::RenderWindow& window);
		void								update(sf::Time dt);
		void								draw();
		Aircraft*							getAircraft();
	private:
		void								loadTextures();
		void								buildScene();

	private:
		enum Layer
		{
			Background,
			Air,
			LayerCount
		};


	private:
		sf::RenderWindow&					mWindow;
		sf::View							mWorldView;
		TextureHolder						mTextures;

		SceneNode							mSceneGraph;
		std::array<SceneNode*, LayerCount>	mSceneLayers;

		Aircraft*							mPlayerAircraft;
		sf::FloatRect						mWorldBounds;
		sf::Vector2f						mSpawnPosition;
		sf::Vector2f						mMouseLocation;
		float								mScrollSpeed;
		sf::Texture textureArr[3];
		sf::IntRect textureRectArr[3];
};

#endif // BOOK_WORLD_HPP
