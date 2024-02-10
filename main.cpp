#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

class Tile
{
	public:
		Tile()
		{
			(*this).value = 0;
			(*this).valExp = 0;
		}

		int getValue() const
		{
			return (*this).value;
		}

		void setValue(int newVal)
		{
		    (*this).value = newVal;
		    if (newVal == 0)
                valExp = 0;
            else
                (*this).valExp = round(log(newVal) / log(2));
		}

		int getExp() const
		{
		    return (*this).valExp;
		}

		void setSprite(sf::Texture tileTexture[], int pos)
		{
		    (*this).sprite.setTexture(tileTexture[(*this).getExp()]);
		    (*this).sprite.setPosition((pos%4)*80,(pos-pos%4)*20);
		}

		void drawTile(sf::RenderWindow & window)
		{
		    window.draw((*this).sprite);
		}

		bool combineWith(Tile & other)
		{
			if ((*this).getValue() != 0 && (*this).getValue() == other.getValue())
			{
				other.setValue((*this).getValue() * 2);
				(*this).setValue(0);
				return 1;
			}
			return 0;
		}

		bool throughZero1(Tile & other)
		{
			if ((*this).getValue() != 0 && other.getValue() == 0)
			{
				other.setValue((*this).getValue());
				(*this).setValue(0);
				return 1;
			}
			return 0;
		}

		bool throughZero2(Tile & other1, Tile & other2)
		{
			if ((*this).getValue() != 0 && other1.getValue() == 0)
			{
				if (other2.getValue() == 0)
				{
					other2.setValue((*this).getValue());
					(*this).setValue(0);
				}
				else
				{
					other1.setValue((*this).getValue());
					(*this).setValue(0);
				}
				return 1;
			}
			return 0;
		}

		bool throughZero3(Tile & other1, Tile & other2, Tile & other3)
		{
			if ((*this).getValue() != 0 && other1.getValue() == 0)
			{
				if (other2.getValue() == 0)
				{
					if (other3.getValue() == 0)
					{
						other3.setValue((*this).getValue());
						(*this).setValue(0);
					}
					else
					{
						other2.setValue((*this).getValue());
						(*this).setValue(0);
					}
				}
				else
				{
					other1.setValue((*this).getValue());
					(*this).setValue(0);
				}
				return 1;
			}
			return 0;
		}

		void spawnTile()
		{
			if (rand() % 10 == 0)
				(*this).setValue(4);
			else
				(*this).setValue(2);
		}

	private:
		int value, valExp;
		sf::Sprite sprite;
};

void tileGenerator(Tile tile[])
{
	int size = 0, spawn = 0, counter = 0;
	bool spaceFound = false;
	for (int i = 0; i < 16; i++)
		if (tile[i].getValue() == 0)
			size++;
	spawn = rand() % size;
	for (int i = 0; i < 16 && !spaceFound; i++)
		if (tile[i].getValue() == 0)
		{
			if (counter == spawn)
				tile[i].spawnTile();
			counter++;
		}
}

void tileMove(int direction, Tile tile[])
{
	int successCount = 0, c = 0;
	if (direction == 1)
	{
		c = 4;
		while (c < 8)
		{
			successCount += tile[c].throughZero1(tile[c-4]);
			c++;
		}
		while (c < 12)
		{
			successCount += tile[c].throughZero2(tile[c-4], tile[c-8]);
			c++;
		}
		while (c < 16)
		{
			successCount += tile[c].throughZero3(tile[c-4], tile[c-8], tile[c-12]);
			c++;
		}
		for (int i = 4; i < 16; i++)
			successCount += tile[i].combineWith(tile[i-4]);
		c = 4;
		while (c < 8)
		{
			successCount += tile[c].throughZero1(tile[c-4]);
			c++;
		}
		while (c < 12)
		{
			successCount += tile[c].throughZero2(tile[c-4], tile[c-8]);
			c++;
		}
		while (c < 16)
		{
			successCount += tile[c].throughZero3(tile[c-4], tile[c-8], tile[c-12]);
			c++;
		}
	}
	else if (direction == 3)
	{
		c = 11;
		while (c > 7)
		{
			successCount += tile[c].throughZero1(tile[c+4]);
			c--;
		}
		while (c > 3)
		{
			successCount += tile[c].throughZero2(tile[c+4], tile[c+8]);
			c--;
		}
		while (c > -1)
		{
			successCount += tile[c].throughZero3(tile[c+4], tile[c+8], tile[c+12]);
			c--;
		}
		for (int i = 11; i > -1; i--)
			successCount += tile[i].combineWith(tile[i+4]);
		c = 11;
		while (c > 7)
		{
			successCount += tile[c].throughZero1(tile[c+4]);
			c--;
		}
		while (c > 3)
		{
			successCount += tile[c].throughZero2(tile[c+4], tile[c+8]);
			c--;
		}
		while (c > -1)
		{
			successCount += tile[c].throughZero3(tile[c+4], tile[c+8], tile[c+12]);
			c--;
		}
	}
	else if (direction == 2)
	{
		c = 1;
		for (int i = 0; i < 4; i++)
		{
			successCount += tile[c].throughZero1(tile[c-1]);
			c++;
			successCount += tile[c].throughZero2(tile[c-1], tile[c-2]);
			c++;
			successCount += tile[c].throughZero3(tile[c-1], tile[c-2], tile[c-3]);
			c += 2;
		}
		c = 1;
		for (int i = 0; i < 4; i++)
		{
			successCount += tile[c].combineWith(tile[c-1]);
			c++;
			successCount += tile[c].combineWith(tile[c-1]);
			c++;
			successCount += tile[c].combineWith(tile[c-1]);
			c += 2;
		}
		c = 1;
		for (int i = 0; i < 4; i++)
		{
			successCount += tile[c].throughZero1(tile[c-1]);
			c++;
			successCount += tile[c].throughZero2(tile[c-1], tile[c-2]);
			c++;
			successCount += tile[c].throughZero3(tile[c-1], tile[c-2], tile[c-3]);
			c += 2;
		}
	}
	else
	{
		c = 14;
		for (int i = 0; i < 4; i++)
		{
			successCount += tile[c].throughZero1(tile[c+1]);
			c--;
			successCount += tile[c].throughZero2(tile[c+1], tile[c+2]);
			c--;
			successCount += tile[c].throughZero3(tile[c+1], tile[c+2], tile[c+3]);
			c -= 2;
		}
		c = 14;
		for (int i = 0; i < 4; i++)
		{
			successCount += tile[c].combineWith(tile[c+1]);
			c--;
			successCount += tile[c].combineWith(tile[c+1]);
			c--;
			successCount += tile[c].combineWith(tile[c+1]);
			c -= 2;
		}
		c = 14;
		for (int i = 0; i < 4; i++)
		{
			successCount += tile[c].throughZero1(tile[c+1]);
			c--;
			successCount += tile[c].throughZero2(tile[c+1], tile[c+2]);
			c--;
			successCount += tile[c].throughZero3(tile[c+1], tile[c+2], tile[c+3]);
			c -= 2;
		}
	}
	if (successCount > 0)
		tileGenerator(tile);
}

void displayBoard(sf::RenderWindow & window, sf::Texture tileTexture[], Tile tile[])
{
	window.clear();
	for (int i = 0; i < 16; i++)
    {
        tile[i].setSprite(tileTexture, i);
        tile[i].drawTile(window);
    }
    window.display();
}

bool ggCheck(Tile tile[])
{
	for (int i = 0; i < 16; i++)
		if (tile[i].getValue() == 0)
			return 0;
	for (int i = 0; i < 12; i++)
	{
		if (tile[i].getValue() == tile[i+4].getValue())
			return 0;
	}
	for (int i = 4; i < 16; i++)
	{
		if (tile[i].getValue() == tile[i-4].getValue())
			return 0;
	}
	int c = 1;
	for (int i = 0; i < 4; i++)
	{
		if (tile[c].getValue() == tile[c-1].getValue())
			return 0;
		c++;
		if (tile[c].getValue() == tile[c-1].getValue())
			return 0;
		c++;
		if (tile[c].getValue() == tile[c-1].getValue())
			return 0;
		c += 2;
	}
	c = 14;
	for (int i = 0; i < 4; i++)
	{
		if (tile[c].getValue() == tile[c+1].getValue())
			return 0;
		c--;
		if (tile[c].getValue() == tile[c+1].getValue())
			return 0;
		c--;
		if (tile[c].getValue() == tile[c+1].getValue())
			return 0;
		c -= 2;
	}
	return 1;
}

bool winCheck(Tile tile[])
{
	for (int i = 0; i < 16; i++)
		if (tile[i].getValue() == 2048)
			return 1;
	return 0;
}

std::string filePath(const std::string & fileName)
{
    return "images/" + fileName;
}

bool loadGraphics(sf::Texture tileTexture[])
{
    if (!tileTexture[0].loadFromFile(filePath("0.png")) ||
        !tileTexture[1].loadFromFile(filePath("2.png")) ||
        !tileTexture[2].loadFromFile(filePath("4.png")) ||
        !tileTexture[3].loadFromFile(filePath("8.png")) ||
        !tileTexture[4].loadFromFile(filePath("16.png")) ||
        !tileTexture[5].loadFromFile(filePath("32.png")) ||
        !tileTexture[6].loadFromFile(filePath("64.png")) ||
        !tileTexture[7].loadFromFile(filePath("128.png")) ||
        !tileTexture[8].loadFromFile(filePath("256.png")) ||
        !tileTexture[9].loadFromFile(filePath("512.png")) ||
        !tileTexture[10].loadFromFile(filePath("1024.png")) ||
        !tileTexture[11].loadFromFile(filePath("2048.png")))
        return 0;
    return 1;
}


int main()
{
	Tile tile[16];
	srand(time(NULL));
	bool playAgain = false, invalid = false;
	sf::RenderWindow window;
	window.create(sf::VideoMode(320,320), "2048", sf::Style::Close | sf::Style::Titlebar);
	window.setKeyRepeatEnabled(0);
	sf::Texture tileTexture[12];
	if (!loadGraphics(tileTexture))
    {
        std::cout << "\n\nLoading failed.";
        sf::Event myEvent1;
        while (window.isOpen())
        {
            window.waitEvent(myEvent1);
            if (myEvent1.type == sf::Event::Closed)
            {
                window.close();
                return 0;
            }
        }
    }
	do
	{
		invalid = true;
		std::cout << "Use the arrow keys to slide the tiles. Matching tiles combine.\n\n"
                  << "Your goal is to reach 2048.";
		for (int i = 0; i < 16; i++)
			tile[i].setValue(0);
		tileGenerator(tile);
		tileGenerator(tile);
		displayBoard(window, tileTexture, tile);
		do
		{
		    sf::Event myEvent;
		    if (window.waitEvent(myEvent))
                switch (myEvent.type)
                {
                    case sf::Event::Closed:
                        window.close();
                        return 0;
                    case sf::Event::KeyPressed:
                        switch (myEvent.key.code)
                        {
                            case sf::Keyboard::Up:
                                tileMove(1, tile);
                                break;
                            case sf::Keyboard::Left:
                                tileMove(2, tile);
                                break;
                            case sf::Keyboard::Down:
                                tileMove(3, tile);
                                break;
                            case sf::Keyboard::Right:
                                tileMove(4, tile);
                                break;
                            default: break;
                        }
                        break;
                    default: break;
                }
			displayBoard(window, tileTexture, tile);
		} while (!ggCheck(tile));
		if (winCheck(tile))
			std::cout << "\n\n\n\nYou win!";
		else
			std::cout << "\n\n\n\nYou lose :c";
		std::cout << "\n\nPlay again? Y/N\n\n\n\n";
		do
		{
		    invalid = true;
		    sf::Event myEvent2;
			if (window.waitEvent(myEvent2))
                switch (myEvent2.type)
                {
                    case sf::Event::Closed:
                        window.close();
                        return 0;
                    case sf::Event::KeyPressed:
                        switch (myEvent2.key.code)
                        {
                            case sf::Keyboard::Y:
                                playAgain = true;
                                invalid = false;
                                break;
                            case sf::Keyboard::N:
                                playAgain = false;
                                invalid = false;
                                break;
                            default: break;
                        }
                        break;
                    default: break;
                }
		} while (invalid);
	} while (playAgain && window.isOpen());
	return 0;
}
