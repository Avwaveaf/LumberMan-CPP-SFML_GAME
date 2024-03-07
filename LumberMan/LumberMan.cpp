#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

int main()
{
	float wWidth = 1920;
	float wHeight = 1080;

	int score = 0;
	

	// loading font
	sf::Font font;
	if (!font.loadFromFile("resources/fonts/ka1.ttf"))
	{
		std::cerr << "Failed to load the font" << std::endl;
		return -1;
	}
	sf::Font mainFont;
	if (!mainFont.loadFromFile("resources/fonts/main.ttf"))
	{
		std::cerr << "Failed to Load main font" << std::endl;
		return -1;
	}

	//adding text object
	sf::Text messageText;
	sf::Text scoreText;
	sf::Text gameName;
	sf::Text authorText;

	gameName.setString("Lumber MAN 2024");
	messageText.setString("Press Enter to Start!!!");
	scoreText.setString("Score = 0");

	authorText.setString("Avwaveaf@2024");

	gameName.setCharacterSize(100);
	messageText.setCharacterSize(30);
	scoreText.setCharacterSize(20);
	
	authorText.setCharacterSize(20);
	

	scoreText.setFillColor(sf::Color::Yellow);
	gameName.setFillColor(sf::Color::Yellow);
	messageText.setFillColor(sf::Color::White);
	
	authorText.setFillColor(sf::Color::White);

	messageText.setFont(font);
	scoreText.setFont(font);
	gameName.setFont(font);

	authorText.setFont(mainFont);

	// Managing text Position
	sf::FloatRect textRect = messageText.getLocalBounds();
	sf::FloatRect mainTextRect = gameName.getLocalBounds();
	
	// setting the origin from top left as center to the center of the text rectangle.
	messageText.setOrigin(textRect.left + (textRect.width / 2.0f),
		textRect.top + (textRect.height / 2.0f)
	);
	//					setting the center of x axis by left + total width /2
	gameName.setOrigin(mainTextRect.left + (mainTextRect.width / 2.0f),
		mainTextRect.top + (mainTextRect.height / 2.0f)
	);
	
	// getting the half of screen width and height in order to get it centered on the screen
	messageText.setPosition(wWidth / 2.0f, wHeight / 2.0f);
	gameName.setPosition(wWidth / 2.0f, (wHeight / 2) - (messageText.getLocalBounds().height + 80));
	scoreText.setPosition(20000,0 );

	authorText.setPosition(wWidth - (authorText.getLocalBounds().width + 20), wHeight - (authorText.getLocalBounds().height + 20));

	// Create Video Object for windows
	sf::VideoMode vm(wWidth, wHeight);

	// Create Open Window for the game
	sf::RenderWindow w(vm, "LumberMan!!! 2024", sf::Style::Fullscreen);

	/*
	===============================
	BACKGROUND SPRITE
	===============================
	*/

	//Create a texture to hold graphics on the GPU
	sf::Texture textureBackground;

	// Loading the graphics
	textureBackground.loadFromFile("resources/graphics/background.png");

	//Create a sprite
	sf::Sprite spriteBackground;
	//attach texture to the sprite
	spriteBackground.setTexture(textureBackground);

	//set the sprite background to cover the screen;
	spriteBackground.setPosition(0, 0);

	/*
	===============================
	TREE SPRITE
	===============================
	*/
	sf::Texture textureTree;
	if (!textureTree.loadFromFile("resources/graphics/tree.png"))
	{
		std::cerr << "Failure to load tree.png" << std::endl;
		return -1;

	}
	sf::Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(800, 10000);

	/*
	===============================
	BEE SPRITE
	===============================
	*/
	sf::Texture textureBee;
	if (!textureBee.loadFromFile("resources/graphics/bee.png"))
	{
		std::cerr << "Failure to load bee.png" << std::endl;
		return -1;

	}

	sf::Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(10000, 0);

	// Bee moving State
	bool beeActive = false;

	// Bee base velocity
	float beeSpeed = 0.0f;

	/*
	===============================
	CLOUDS SPRITE
	===============================
	*/

	sf::Texture textureClouds;
	textureClouds.loadFromFile("resources/graphics/cloud.png");

	// 3 clouds sprite with the same texture
	sf::Sprite spriteCloud1;
	sf::Sprite spriteCloud2;
	sf::Sprite spriteCloud3;
	sf::Sprite spriteCloud1i;

	spriteCloud1.setTexture(textureClouds);
	spriteCloud2.setTexture(textureClouds);
	spriteCloud3.setTexture(textureClouds);
	spriteCloud1i.setTexture(textureClouds);

	// set position for each clouds with different position
	spriteCloud1.setPosition(-2000, 0);
	spriteCloud2.setPosition(-2000, 250);
	spriteCloud3.setPosition(-2000, 500);
	spriteCloud1i.setPosition(wWidth+200, 250);


	// is clouds currently on the screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	bool cloud1iActive = false;

	// How fast each cloud
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;
	float cloud1iSpeed = 0.0f;


	// variable to control time 

	sf::Clock clock;

	sf::RectangleShape timeBar;
	float timeBarStartWidth = wWidth;
	float timeBarHeight = 10;
	timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(sf::Color::Red);
	timeBar.setPosition((wWidth / 2) - timeBarStartWidth/2, wHeight);

	sf::Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;


	// Tracking the game state
	bool paused = true;


	// main loop
	while (w.isOpen())
	{
		/*
		===============================
		HANDLE PLAYER INPUT
		===============================
		*/

		sf::Event event;
		while (w.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				w.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			w.close();

		}

		// starting the game on Enter key pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			paused = false;
			//reset time and score
			score = 0;
			timeRemaining = 6;
		}

		/*
		===============================
		HANDLE Update Scene
		===============================
		*/

		if (!paused)
		{
			sf::Time dt = clock.restart();
			// substract time remaining
			timeRemaining -= dt.asSeconds();
			// resize timebar
			timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));


			spriteTree.setPosition(800, 0);
			scoreText.setPosition(20, 20);
			timeBar.setPosition((wWidth / 2) - timeBarStartWidth / 2, 1060);

			// handle time's up
			if (timeRemaining <= 0.0f)
			{
				// pause the game
				paused = true;

				// Change the message
				gameName.setString("Time's Up!!");
				
				gameName.setFillColor(sf::Color::Red);
				
				sf::FloatRect mainTextRect = gameName.getLocalBounds();

				//					setting the center of x axis by left + total width /2
				gameName.setOrigin(mainTextRect.left + (mainTextRect.width / 2.0f),
					mainTextRect.top + (mainTextRect.height / 2.0f)
				);
				gameName.setPosition(wWidth / 2.0f, (wHeight / 2) - (messageText.getLocalBounds().height + 80));
					
			}

			//making bee come to live
			if (!beeActive)
			{
				//How fast is bee moving
				srand((int)time(0));
				beeSpeed = (rand() % 200) + 500;

				// How High is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				// move the bee

				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);

				// if bee reached to the end left of the screen,set bee to false active
				if (spriteBee.getPosition().x < -100)
				{
					// setup for teh next frame
					beeActive = false;

				}
			}

			// Managing CLoud 1
			if (!cloud1Active)
			{
				// how fast is the cloud
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				// how high is the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				// declaring base position x and random y axis before active
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x + cloud1Speed * dt.asSeconds()
					, spriteCloud1.getPosition().y);

				// is the cloud reached the end?
				if (spriteCloud1.getPosition().x > (wWidth + 20))
				{
					// set the cloud off
					cloud1Active = false;
				}
			}

			//managing cloud 2
			if (!cloud2Active)
			{
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds())
					, spriteCloud2.getPosition().y);

				// is the cloud reached the end?
				if (spriteCloud2.getPosition().x > (wWidth + 20))
				{
					// set the cloud off
					cloud2Active = false;
				}
			}

			//Managing cloud 3
			if (!cloud3Active)
			{
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + cloud3Speed * dt.asSeconds()
					, spriteCloud3.getPosition().y);
				// is the cloud reached the end?
				if (spriteCloud3.getPosition().x > (wWidth + 20))
				{
					// set the cloud off
					cloud3Active = false;
				}
			}


			// Managing cloud 1 inverted
			if (!cloud1iActive)
			{
				srand((int)time(0) * 40);
				cloud1iSpeed = (rand() % 200);

				srand((int)time(0) * 40);
				float height = (rand() % 150);
				spriteCloud1i.setPosition(wWidth - 200, height);
				cloud1iActive = true;
			}
			else
			{
				spriteCloud1i.setPosition(
					spriteCloud1i.getPosition().x - (cloud1iSpeed * dt.asSeconds()),
					spriteCloud1i.getPosition().y);

				if (spriteCloud1i.getPosition().x < -200) {

					cloud1iActive = true;
				}
			}

			// updating the score text
			score++;
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			
		} // end if not paused
		/*
		===============================
		HANDLE Draw Scene
		===============================
		*/
		//Clearing Everything from last frame
		w.clear();

		//	>>>>>	drawing game schene here

		w.draw(spriteBackground);

		// draw clouds
		w.draw(spriteCloud1);
		w.draw(spriteCloud2);
		w.draw(spriteCloud3);
		w.draw(spriteCloud1i);

		// draw tree
		w.draw(spriteTree);

		// draw insects
		w.draw(spriteBee);

		// draw score
		w.draw(scoreText);

		// draw timebar
		w.draw(timeBar);
		
		// draw continue message conditionally..
		if (paused)
		{
			w.draw(gameName);
			w.draw(messageText);
		}

		w.draw(authorText);



		//show everything we just draw
		w.display();


	}

	return 0;
}
