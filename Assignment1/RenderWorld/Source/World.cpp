#include <Book/World.hpp>
#include <iostream>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

const int TEXDISTANCE = 2000;


World::World(sf::RenderWindow& window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, TEXDISTANCE)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
	, mScrollSpeed(-350.f)
	, mPlayerAircraft(nullptr)
{
	loadTextures();
	buildScene();

	// Prepare the view
	mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{
	// Scroll the world
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());

	// Move the player sidewards (plane scouts follow the main aircraft)
	sf::Vector2f position = mPlayerAircraft->getPosition();
	sf::Vector2f velocity = mPlayerAircraft->getVelocity();

	//own bound checker
	//std::cout << "POSX: " << position.x << std::endl;
	//std::cout << "POSY: " << position.y << std::endl;
	//sf::Vector2f Relposition = mWorldView;

	if (position.x <= 120.f) {
		velocity.x = std::abs(velocity.x);
		velocity.x = -velocity.x;
		mPlayerAircraft->setVelocity(velocity);
	}
	if (position.x >= 520.f) {
		velocity.x = std::abs(velocity.x);
		mPlayerAircraft->setVelocity(velocity);
	}
	// If player touches borders, flip its X velocity ..not sure if this worked wrote own helper on top

	if (position.x <= mWorldBounds.left + 150.f
		|| position.x >= mWorldBounds.left + mWorldBounds.width - TEXDISTANCE)
	{
		velocity.x = -velocity.x;
		mPlayerAircraft->setVelocity(velocity);
	}
	// Apply movements
	mSceneGraph.update(dt);

	//implement the looping background A1
	static int iterator = 2;
	static bool firstPass = true;
	if (position.y <= -(textureRectArr[0].height * iterator - mSpawnPosition.y)) {
		static int childIndex = 0;
		std::cout << "threshold passed" << std::endl;
		mSceneLayers[Background]->getChild(0)->setPosition(mWorldBounds.left, mWorldBounds.top - textureRectArr[0].height * (iterator + 1));
		mSceneLayers[Background]->getChild(1)->setPosition(mWorldBounds.left, mWorldBounds.top - textureRectArr[0].height * (iterator + 2));
		iterator += 3;
		firstPass = false;
	}
	if ((position.y <= -(textureRectArr[0].height * (iterator - 1) - mSpawnPosition.y) && (!firstPass))) {
		std::cout << "Green threshold passed" << std::endl;
		mSceneLayers[Background]->getChild(2)->setPosition(mWorldBounds.left, mWorldBounds.top - textureRectArr[0].height * (iterator));

	}
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

void World::loadTextures()
{
	mTextures.load(Textures::Eagle, "Media/Textures/Eagle.png");
	mTextures.load(Textures::Raptor, "Media/Textures/Raptor.png");
	mTextures.load(Textures::Desert, "Media/Textures/Desert.png");
	mTextures.load(Textures::Water, "Media/Textures/Water.jpg");
	mTextures.load(Textures::Grass, "Media/Textures/Grass.jpg");
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	// Prepare the tiled desert background
	textureArr[0] = mTextures.get(Textures::Desert);
	textureRectArr[0] = sf::IntRect(mWorldBounds);
	textureArr[0].setRepeated(true);


	// Prepare the tiled water background
	textureArr[1] = mTextures.get(Textures::Water);
	textureRectArr[1] = sf::IntRect(mWorldBounds);
	textureArr[1].setRepeated(true);

	// Prepare the tiled grass background
	textureArr[2] = mTextures.get(Textures::Grass);
	textureRectArr[2] = sf::IntRect(mWorldBounds);
	textureArr[2].setRepeated(true);

	//std::unique_ptr<SpriteNode> backgroundSprite3(new SpriteNode(textureArr[2], textureRectArr[2]));
	//backgroundSprite3->setPosition(mWorldBounds.left, textureRectArr[1].top);
	//mSceneLayers[Background]->attachChild(std::move(backgroundSprite3));
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(textureArr[0], textureRectArr[0]));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);

	std::unique_ptr<SpriteNode> backgroundSprite2(new SpriteNode(textureArr[1], textureRectArr[1]));
	backgroundSprite2->setPosition(mWorldBounds.left, mWorldBounds.top - textureRectArr[0].height);

	std::unique_ptr<SpriteNode> backgroundSprite3(new SpriteNode(textureArr[2], textureRectArr[2]));
	backgroundSprite3->setPosition(mWorldBounds.left, mWorldBounds.top - textureRectArr[0].height * 2);

	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite2));
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite3));

	// Add player's aircraft
	std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Eagle, mTextures));
	mPlayerAircraft = leader.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	mPlayerAircraft->setVelocity(40.f, mScrollSpeed);
	mSceneLayers[Air]->attachChild(std::move(leader));

	// Add two escorting aircrafts, placed relatively to the main plane
	std::unique_ptr<Aircraft> leftEscort(new Aircraft(Aircraft::Raptor, mTextures));
	leftEscort->setPosition(-80.f, 50.f);
	mPlayerAircraft->attachChild(std::move(leftEscort));

	std::unique_ptr<Aircraft> rightEscort(new Aircraft(Aircraft::Raptor, mTextures));
	rightEscort->setPosition(80.f, 50.f);
	mPlayerAircraft->attachChild(std::move(rightEscort));

}

Aircraft* World::getAircraft() {
	return mPlayerAircraft;
}
