#include <iostream>
#include <SFML/Graphics.hpp>

#include "Minefield.h"

/*
Project: SweepMiner
Created: 6 MAR 2024
Updated: 9 OCT 2024
Author: Samuel Daly

Description:
	This file contains the code for the game state (ie playing, and win) and drives a single instance of the minefield class.
*/


int main() {
	//Vars
	sf::RenderWindow mainWindow(sf::VideoMode(452, 500), "Unnamed");
	sf::Event event;

	sf::Font elephant;

	bool isWon = 0;
	int numMines = 15;
	sf::Vector2i tileSize = { 28,28 };
	int width = 10;
	int height = 10;
	int seed = 12345 + time(0);
	std::cout << "Seed: " << seed << std::endl;

	sf::RectangleShape bar;
	bar.setSize({ (float)mainWindow.getSize().x, 48 });
	bar.setFillColor(sf::Color(150, 150, 150));
	bar.setOutlineThickness(1);
	bar.setOutlineColor(sf::Color::Black);

	tileSize.x = mainWindow.getSize().x / width;
	tileSize.y = (mainWindow.getSize().y - 48) / height;

	sf::RectangleShape winRect;
	float sizeX = mainWindow.getSize().x / 2;
	float sizeY = sizeX / 2;
	winRect.setSize({ sizeX,sizeY });
	winRect.setFillColor(sf::Color(150, 150, 150));
	float px = (mainWindow.getSize().x / 2) - (winRect.getSize().x / 2);
	float py = (mainWindow.getSize().x / 2) - (winRect.getSize().x / 2);
	winRect.setPosition({ px, py });
	winRect.setOutlineThickness(1);
	winRect.setOutlineColor(sf::Color::Black);

	Minefield m(width, height, numMines, tileSize);
	m.Generate(seed);
	m.Print();
	m.SetPos({ 0,48 });

	//Text setup
	elephant.loadFromFile("fonts/ELEPHNT.TTF");
	sf::Text winText;
	winText.setFont(elephant);
	winText.setString("You Won!");
	winText.setCharacterSize(32);
	winText.setFillColor(sf::Color::White);
	winText.setPosition(winRect.getPosition());
	winText.move({ (winRect.getSize().x / 2) - (winText.getLocalBounds().width / 2), (winRect.getSize().y / 2) - (winText.getLocalBounds().height / 2) });
	int correctX = (int)winText.getPosition().x;
	int correctY = (int)winText.getPosition().y;
	winText.setPosition({ (float)correctX, (float)correctY });


	sf::Text scoreText;
	scoreText.setFont(elephant);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setCharacterSize(28);
	//scoreText.setPosition();
	scoreText.setString("0");

	sf::Text bombText;
	bombText.setFont(elephant);
	bombText.setFillColor(sf::Color::White);
	bombText.setCharacterSize(28);
	bombText.setPosition({ 200,0 });
	bombText.setString("Bombs Left: 0");

	//Window Loop
	while (mainWindow.isOpen()) {
		//Update Delta time

		//Event Loop
		while (mainWindow.pollEvent(event)) {

			//Window Closure
			if (event.type == sf::Event::Closed) {
				mainWindow.close();
			}

			//Window Resize
			if (event.type == sf::Event::Resized) {
				sf::FloatRect newViewport(0,0,event.size.width,event.size.height);
				mainWindow.setView(sf::View(newViewport));
			}

			//Custom
			m.Poll(event, mainWindow);

			//Test
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Middle) {
				std::cout << "Score: " << m.GetScore() << std::endl;
			}
		}

		//Update

		if (!isWon) {
			//Update
			m.Update(mainWindow);
			scoreText.setString("Score: " + std::to_string(m.GetScore()));
			int left = numMines - m.GetFoundBombs();
			bombText.setString("Bombs Left: " + std::to_string(left));

			//Check for win
			if (m.GetFoundBombs() >= numMines) {
				//Then you win
				isWon = 1;

			}
		}



		//Render
		mainWindow.clear();

		mainWindow.draw(bar);
		mainWindow.draw(scoreText);
		mainWindow.draw(bombText);
		m.Draw(mainWindow);

		if (isWon) {
			mainWindow.draw(winRect);
			mainWindow.draw(winText);
		}

		mainWindow.display();

	}

	return 0;
}