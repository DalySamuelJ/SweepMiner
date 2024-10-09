#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

/*
Project: SweepMiner
Created: 6 MAR 2024
Updated: 9 OCT 2024
Author: Samuel Daly

Description:
	This file contains the declarations for the Minefeild class and Tile Struct.
*/


struct Tile {
	bool isHidden = 1;
	bool isHover = 0;
	bool isClick = 0;
	bool isLock = 0;
	sf::Sprite sprite;
	sf::Sprite hidden;
	sf::Sprite lock;
	int value = 0;
	void Update(sf::Vector2i mousePos);
	void Poll(sf::Event& e, sf::Vector2i mousePos);
};

//####### MINE FIELD ##############

class Minefield
{
private:
	//Data
	sf::Vector2i pos = { 0,0 };
	std::vector<sf::Texture*> textures; //0 = mine, 1 = empty, 2-9 = numbers, 10 = hidden
	std::vector<std::vector<Tile*>*> tiles;
	int** field;

	int width = 10;
	int height = 10;
	int numMines = 2;
	sf::Vector2i tileSize = { 16,16 };

	//Util
	void initTextures();
	std::vector<sf::Vector2i> GenMineCoords(int seed, int rangeX, int rangeY);

public:
	//Constructor and Destructor
	Minefield(int width, int height, int mines, sf::Vector2i tileSize);
	~Minefield();

	//Primary Functions
	void Generate(int seed);
	void Update(sf::RenderWindow& win);
	void Poll(sf::Event& e, sf::RenderWindow& win);
	void Draw(sf::RenderWindow& win);
	void SetPos(sf::Vector2i newPos);
	void RevealClear(sf::Vector2i pos);
	void Print();
	int GetScore();
	int GetFoundBombs();
};

