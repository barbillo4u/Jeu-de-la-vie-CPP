#include <SFML/Graphics.hpp>

#define DEAD 0
#define LIVE 1
#define CELL_SIZE 12

#define WIDTH 110
#define HEIGHT 58

#define SCREEN_WIDTH CELL_SIZE*WIDTH
#define SCREEN_HEIGHT CELL_SIZE*HEIGHT

void init(int state[][HEIGHT], int temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT])
{
	for(int y = 0; y < HEIGHT; y++)
	{
		for(int x = 0; x < WIDTH; x++)
		{
			state[x][y] = DEAD;
			temp[x][y] = DEAD;
			cells[x][y] = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
			cells[x][y].setFillColor(sf::Color::White);
			cells[x][y].setOutlineColor(sf::Color::Red);
			cells[x][y].setOutlineThickness(2.f);
			cells[x][y].setPosition(x*CELL_SIZE + 2, y*CELL_SIZE + 2);
		}
	}
}
int count_neighbour(int state[][HEIGHT], int x, int y)
{
	int number_neighbour = 0;

	//Diagonal up-left
	if(state[(x-1)%WIDTH][(y+1)%HEIGHT] == LIVE)
	{
		number_neighbour++;
	}

	//up
	if(state[x][(y+1)%HEIGHT] == LIVE)
	{
		number_neighbour++;
	}

	//Diagonal up-right
	if(state[(x+1)%WIDTH][(y+1)%HEIGHT] == LIVE)
	{
		number_neighbour++;
	}

	//left
	if(state[(x-1)%WIDTH][y] == LIVE)
	{
		number_neighbour++;
	}

	//right
	if(state[(x+1)%WIDTH][y] == LIVE)
	{
		number_neighbour++;
	}

	//Diagonal down-left
	if(state[(x-1)%WIDTH][(y-1)%HEIGHT] == LIVE)
	{
		number_neighbour++;
	}

	//down
	if(state[x][(y-1)%HEIGHT] == LIVE)
	{
		number_neighbour++;
	}

	//Diagonal down-right
	if(state[(x+1)%WIDTH][(y-1)%HEIGHT] == LIVE)
	{
		number_neighbour++;
	}

	return number_neighbour;
}
bool check_empty(int state[][HEIGHT])
{
	bool result;
	for(int y = 0; y < HEIGHT; y++)
	{
		for(int x = 0; x < WIDTH; x++)
		{
			if(state[x][y] == LIVE)
				result = true;
			else
				result = false;
		}
	}
	return result;
}
void run(int state[][HEIGHT], int temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT])
{

	for(int y = 0; y < HEIGHT; y++)
	{
		for(int x = 0; x < WIDTH; x++)
		{
			int number_neighboor = count_neighbour(state, x, y);

			//mort par isolement
			if(state[x][y] == LIVE && number_neighboor < 2)
				temp[x][y] = DEAD;

			//reste en vie
			if(state[x][y] == LIVE && (number_neighboor == 2 || number_neighboor == 3))
				temp[x][y] = LIVE;

			//mort par surpopulation
			if(state[x][y] == LIVE && number_neighboor > 3)
				temp[x][y] = DEAD;

			if(state[x][y] == DEAD && number_neighboor == 3)
				temp[x][y] = LIVE;
		}
	}

	for(int y = 0; y < HEIGHT; y++)
	{
		for(int x = 0; x < WIDTH; x++)
		{
			state[x][y] = temp[x][y];
			if(state[x][y] == LIVE)
				cells[x][y].setFillColor(sf::Color::Black);
			else
				cells[x][y].setFillColor(sf::Color::White);
		}
	}
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Game Of Life", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
	sf::Event event;
	int state[WIDTH][HEIGHT] ;
	int temp[WIDTH][HEIGHT];
	sf::RectangleShape cells[WIDTH][HEIGHT];
	int running = false;

	init(state, temp, cells);
	while(window.isOpen())
	{
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}
			sf::Vector2i mouse_cursor = sf::Mouse::getPosition(window);
			if((mouse_cursor.x <= SCREEN_WIDTH && mouse_cursor.y <= SCREEN_HEIGHT) && (mouse_cursor.x > 0 && mouse_cursor.y > 0))
			{
				if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					cells[mouse_cursor.x/CELL_SIZE][mouse_cursor.y/CELL_SIZE].setFillColor(sf::Color::Black);
					state[mouse_cursor.x/CELL_SIZE][mouse_cursor.y/CELL_SIZE] = LIVE;
				}


				if(sf::Mouse::isButtonPressed((sf::Mouse::Right)))
				{
					cells[mouse_cursor.x/CELL_SIZE][mouse_cursor.y/CELL_SIZE].setFillColor(sf::Color::White);
					state[mouse_cursor.x/CELL_SIZE][mouse_cursor.y/CELL_SIZE] = DEAD;
				}
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::C))
			{
				for(int y = 0; y < HEIGHT; y++)
				{
					for(int x = 0; x < WIDTH; x++)
					{
						state[x][y] = DEAD;
						temp[x][y] = DEAD;
						cells[x][y].setFillColor(sf::Color::White);
					}
				}
			}
			if(sf::Keyboard::isKeyPressed((sf::Keyboard::Space)))
				running = true;
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				for(int y = 0; y < HEIGHT; y++)
				{
					for(int x = 0; x < WIDTH; x++)
					{
						state[x][y] = DEAD;
						temp[x][y] = DEAD;
						cells[x][y].setFillColor(sf::Color::White);
					}
				}
				running = false;
			}

		}
		if(running)
			run(state, temp, cells);
		window.clear();
		for(int y = 0; y < HEIGHT; y++)
		{
			for(int x = 0; x < WIDTH; x++)
			{
				window.draw(cells[x][y]);
			}
		}
		window.display();
	}


    return EXIT_SUCCESS;
}
