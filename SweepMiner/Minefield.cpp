#include "Minefield.h"

void Minefield::initTextures()
{
	//Mine
	textures.push_back(new sf::Texture);
	textures.at(0)->loadFromFile("textures/mine.png");

	//Empty
	textures.push_back(new sf::Texture);
	textures.at(1)->loadFromFile("textures/empty.png");

	//Numbers
	textures.push_back(new sf::Texture);
	textures.at(2)->loadFromFile("textures/1.png");

	textures.push_back(new sf::Texture);
	textures.at(3)->loadFromFile("textures/2.png");

	textures.push_back(new sf::Texture);
	textures.at(4)->loadFromFile("textures/3.png");

	textures.push_back(new sf::Texture);
	textures.at(5)->loadFromFile("textures/4.png");

	textures.push_back(new sf::Texture);
	textures.at(6)->loadFromFile("textures/5.png");

	textures.push_back(new sf::Texture);
	textures.at(7)->loadFromFile("textures/6.png");

	textures.push_back(new sf::Texture);
	textures.at(8)->loadFromFile("textures/7.png");

	textures.push_back(new sf::Texture);
	textures.at(9)->loadFromFile("textures/8.png");

	//Lock
	textures.push_back(new sf::Texture);
	textures.at(10)->loadFromFile("textures/lock.png");

	//Hidden
	textures.push_back(new sf::Texture);
	textures.at(11)->loadFromFile("textures/hidden.png");

}

std::vector<sf::Vector2i> Minefield::GenMineCoords(int seed, int rangeX, int rangeY)
{
	//Vars
	std::vector<sf::Vector2i> output;
	int currX = 0;
	int currY = 0;
	int counter = 1;
	bool found = 1;

	//Gen
	srand(seed);
	currX = rand() % rangeX;
	currY = rand() % rangeY;

	output.push_back({ currX, currY });

	while (counter < numMines) {
		currX = rand() % rangeX;
		currY = rand() % rangeY;


		found = 0;
		for (int i = 0; i < output.size(); i++) {
			if ((sf::Vector2i{ currX, currY } == output.at(i))) {
				//If not found
				found = 1;
			}
		}

		if (!found) {
			output.push_back({ currX, currY });
			counter++;
		}
	}

	return output;
}

Minefield::Minefield(int width, int height, int mines, sf::Vector2i tileSize)
{
	this->width = width;
	this->height = height;
	numMines = mines;

	this->tileSize = tileSize;

	//Ensure valid size
	if (this->width < 3) {
		this->width = 3;
	}

	if (this->height < 3) {
		this->height = 3;
	}

	//Clip number of mines to ensure at least one free space
	if (numMines > (width * height) - 1) {
		numMines = (width * height) - 1;
	}

	initTextures();
}

Minefield::~Minefield()
{
	//## Delete field
	//Free interior
	for (int i = 0; i < height; i++) {
		free(field[i]);
	}
	//Free exterior
	free(field);

	//## Delete tiles
	for (int i = 0; i < height; i++) {

		for (int j = 0; j < width; j++) {
			delete tiles.at(i)->at(j);
		}

		delete tiles.at(i);
	}

}


void Minefield::Generate(int seed)
{
	//Vars
	int randX, randY;

	int rangeX = width;
	int rangeY = height;


	//Init field to 0
	field = (int**)malloc(height * sizeof(int*));

	//# Allocate and initialize interior to -1
	for (int i = 0; i < height; i++) {
		field[i] = (int*)malloc(width * sizeof(int));
		memset(field[i], 0, width * sizeof(int));
	}

	//Generate mines
	std::vector<sf::Vector2i> mines = GenMineCoords(seed, rangeX, rangeY);

	for (int i = 0; i < (int)mines.size(); i++) {
		field[mines[i].y][mines[i].x] = -1;
	}



	//Compute adjacency markers
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (field[i][j] == -1) {
				//Then its a mine increment the adjacent 8 tiles
				sf::Vector2i adj[8];

				adj[0] = { j - 1,i - 1 };
				adj[1] = { j,i - 1 };
				adj[2] = { j + 1,i - 1 };
				adj[3] = { j - 1,i };
				adj[4] = { j + 1,i };
				adj[5] = { j - 1,i + 1 };
				adj[6] = { j,i + 1 };
				adj[7] = { j + 1,i + 1 };

				//Increment valid adjacent tiles (ie within bounds and not a mine)
				for (int k = 0; k < 8; k++) {
					if ((adj[k].x >= 0 && adj[k].y >= 0) && (adj[k].x < width && adj[k].y < height) && field[adj[k].y][adj[k].x] != -1) {
						field[adj[k].y][adj[k].x] += 1;
					}
				}

			}
		}
	}

	//Set up tiles
	for (int i = 0; i < height; i++) {
		tiles.push_back(new std::vector<Tile*>);
		for (int j = 0; j < width; j++) {
			tiles.at(i)->push_back(new Tile);
			tiles.at(i)->at(j)->value = field[i][j];

			tiles.at(i)->at(j)->hidden.setTexture(*textures.back());
			tiles.at(i)->at(j)->hidden.setScale({ (float)tileSize.x / (float)tiles.at(i)->at(j)->hidden.getTexture()->getSize().x, (float)tileSize.y / (float)tiles.at(i)->at(j)->hidden.getTexture()->getSize().y });
			tiles.at(i)->at(j)->hidden.setPosition({ (float)j * tileSize.x,(float)i * tileSize.y });

			tiles.at(i)->at(j)->sprite.setTexture(*textures.at(tiles.at(i)->at(j)->value + 1));;
			tiles.at(i)->at(j)->sprite.setScale({ (float)tileSize.x / (float)tiles.at(i)->at(j)->sprite.getTexture()->getSize().x, (float)tileSize.y / (float)tiles.at(i)->at(j)->sprite.getTexture()->getSize().y });
			tiles.at(i)->at(j)->sprite.setPosition({ (float)j * tileSize.x,(float)i * tileSize.y });

			tiles.at(i)->at(j)->lock.setTexture(*textures.at(10));
			tiles.at(i)->at(j)->lock.setScale({ (float)tileSize.x / (float)tiles.at(i)->at(j)->lock.getTexture()->getSize().x, (float)tileSize.y / (float)tiles.at(i)->at(j)->lock.getTexture()->getSize().y });
			tiles.at(i)->at(j)->lock.setPosition({ (float)j * tileSize.x,(float)i * tileSize.y });
		}
	}

}

void Minefield::Update(sf::RenderWindow& win)
{
	//Vars
	sf::Vector2i mousePos = sf::Mouse::getPosition(win);

	//Update tiles
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tiles.at(i)->at(j)->Update(mousePos);
		}
	}
}

void Minefield::Poll(sf::Event& e, sf::RenderWindow& win)
{
	//Vars
	sf::Vector2i mousePos = sf::Mouse::getPosition(win);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tiles.at(i)->at(j)->Poll(e, mousePos);
		}
	}
}

void Minefield::Draw(sf::RenderWindow& win)
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (tiles.at(i)->at(j)->isLock) {
				win.draw(tiles.at(i)->at(j)->lock);
			}
			else if (tiles.at(i)->at(j)->isHidden) {
				win.draw(tiles.at(i)->at(j)->hidden);
			}
			else {
				win.draw(tiles.at(i)->at(j)->sprite);
			}
		}
	}
}

void Minefield::SetPos(sf::Vector2i newPos)
{
	pos = newPos;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tiles.at(i)->at(j)->hidden.setPosition({ (float)j * tileSize.x + pos.x,(float)i * tileSize.y + pos.y });
			tiles.at(i)->at(j)->lock.setPosition({ (float)j * tileSize.x + pos.x,(float)i * tileSize.y + pos.y });
			tiles.at(i)->at(j)->sprite.setPosition({ (float)j * tileSize.x + pos.x,(float)i * tileSize.y + pos.y });
		}
	}
}

void Minefield::RevealClear(sf::Vector2i pos)
{
	if (field[pos.y][pos.x] != 1) {
		return;
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (field[i][j] == -1) {
				//Then its a mine increment the adjacent 8 tiles
				sf::Vector2i adj[8];

				adj[0] = { j - 1,i - 1 };
				adj[1] = { j,i - 1 };
				adj[2] = { j + 1,i - 1 };
				adj[3] = { j - 1,i };
				adj[4] = { j + 1,i };
				adj[5] = { j - 1,i + 1 };
				adj[6] = { j,i + 1 };
				adj[7] = { j + 1,i + 1 };

				//Reveal valid adjacent tiles (ie within bounds and not a mine)
				for (int k = 0; k < 8; k++) {
					if ((adj[k].x >= 0 && adj[k].y >= 0) && (adj[k].x < width && adj[k].y < height) && field[adj[k].y][adj[k].x] != -1) {
						if (field[adj[k].y][adj[k].x] == 1) {
							RevealClear({adj[k].x, adj[k].y});
						}
					}
				}

			}
		}
	}

	tiles.at(pos.y)->at(pos.x)->isHidden = 0;

}

void Minefield::Print()
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			std::cout << field[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

int Minefield::GetScore()
{
	//Vars
	int score = 0;

	//Count revealed non bomb tiles
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (!tiles.at(i)->at(j)->isHidden && tiles.at(i)->at(j)->value != -1) {
				score++;
			}
		}
	}

	return score;
}

int Minefield::GetFoundBombs()
{
	//Vars
	int numFound = 0;

	//Count revealed bomb tiles
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (!tiles.at(i)->at(j)->isHidden && tiles.at(i)->at(j)->value == -1) {
				numFound++;
			}
		}
	}

	return numFound;
}

void Tile::Update(sf::Vector2i mousePos)
{
	if (sprite.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
		isHover = true;
		hidden.setColor(sf::Color(200, 200, 200));
		lock.setColor(sf::Color(200, 200, 200));
	}
	else {
		isHover = false;
		hidden.setColor(sf::Color(255, 255, 255));
		lock.setColor(sf::Color(255, 255, 255));
	}
}

void Tile::Poll(sf::Event& e, sf::Vector2i mousePos)
{
	if (!isHover) {
		return;
	}

	if (e.type == sf::Event::MouseButtonReleased) {
		if (e.mouseButton.button == sf::Mouse::Left) {
			//Then reveal tile
			if (!isLock) {
				isHidden = false;
			}
		}
		if (e.mouseButton.button == sf::Mouse::Right) {
			if (isHidden) {
				//Then toggle lock
				isLock = !isLock;
			}
		}
	}
}
