/*
Гра Сапер v1.00
Автор Солод Т.А.
*/

#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace sf;

const int w = 32; //розмір одного квадратного елемента текстури 
const int weight = 840, height = 700; //розміри головного вікна

int size; // розмір ігрового поля
int mine; // кількіть мін на ігровому полі
int kmine; // кількість прапорців для позначення потенційно замінованих комірок
int knull; // кількість комірок, відкритих гравцем
int empty; // кількість незаміновних комірок
int grid[w][w]; // масив логічної частини ігрового поля 
int sgrid[w][w]; // масив графічної частини ігрового поля 

std::stringstream toStringConv;
std::string tempString;

// перетворення числа в рядок
std::string intToString(int m)
{
	toStringConv.clear();
	toStringConv << m;
	toStringConv >> tempString;
	return tempString;
}

// створення та відображення меню гри, перемальовування вікна для обраного рівня
void menu(RenderWindow& window)
{
	Texture mt10, mt12, mt15, exit, bg1;
	bg1.loadFromFile("resources/bg1.png");
	mt10.loadFromFile("resources/10.png");
	mt12.loadFromFile("resources/12.png");
	mt15.loadFromFile("resources/15.png");
	exit.loadFromFile("resources/ex.png");

	Vector2u sizemt10 = mt10.getSize(), sizemt12 = mt12.getSize(), sizemt15 = mt12.getSize(), sizeexit = exit.getSize();

	Sprite background(bg1), menu10(mt10), menu12(mt12), menu15(mt15), mexit(exit);
	bool isMenu = 1;
	int menuNum = 0;

	background.setPosition(30, 0);
	menu10.setPosition(weight - sizemt10.x - w, height / 2 - sizemt10.y * 2);
	menu12.setPosition(weight - sizemt12.x - w, height / 2);
	menu15.setPosition(weight - sizemt15.x - w, height / 2 + sizemt15.y * 2);
	mexit.setPosition(weight - sizeexit.x - w, height - sizeexit.y * 2);

	while (isMenu)
	{
		menu10.setColor(Color::Black);
		menu12.setColor(Color::Black);
		menu15.setColor(Color::Black);
		menuNum = 0;
		window.clear(Color::White);

		if (IntRect(weight - sizemt10.x - w, height / 2 - sizemt10.y * 2, sizemt10.x, sizemt10.y).contains(Mouse::getPosition(window))) { menu10.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(weight - sizemt12.x - w, height / 2, sizemt12.x, sizemt12.y).contains(Mouse::getPosition(window))) { menu12.setColor(Color::Blue); menuNum = 2; }
		if (IntRect(weight - sizemt15.x - w, height / 2 + sizemt15.y * 2, sizemt15.x, sizemt15.y).contains(Mouse::getPosition(window))) { menu15.setColor(Color::Blue); menuNum = 3; }
		if (IntRect(weight - sizeexit.x - w, height - sizeexit.y * 2, sizeexit.x, sizeexit.y).contains(Mouse::getPosition(window))) { menuNum = 4; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) { size = 10; mine = 10; isMenu = false; window.create(VideoMode((size + 2) * w, (size + 2) * w), "", Style::None); }
			if (menuNum == 2) { size = 12; mine = 15; isMenu = false; window.create(VideoMode((size + 2) * w, (size + 2) * w), "", Style::None); }
			if (menuNum == 3) { size = 15; mine = 25; isMenu = false; window.create(VideoMode((size + 2) * w, (size + 2) * w), "", Style::None); }
			if (menuNum == 4) { window.close(); isMenu = false; }
		}
		window.draw(background);
		window.draw(menu10);
		window.draw(menu12);
		window.draw(menu15);
		window.draw(mexit);
		window.display();
	}
}
/* відмальовування ігрового поля;
всім елементам масиву grid[i][j] присвоюємо значення відповідного порядкового номера комірки текстури:
0 - відкрита пуста комірка;
1-8 - цифри від 1 до 8;
9 - міна;
10 - закрита комірка;
11 - прапорець.
*/
void field(RenderWindow& window, int x, int y)
{
	Texture t;
	t.loadFromFile("resources/tiles.png");
	Sprite tiles(t);
	for (int i = 1; i <= size; i++)
		for (int j = 1; j <= size; j++)
		{
			if (sgrid[x][y] == 9)
			{
				sgrid[i][j] = grid[i][j];
			}
			tiles.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
			tiles.setPosition(i * w, j * w);
			window.draw(tiles);
		}
}
//розкидання мін по ігровому полю за допомогою генератора випадкових чисел
void scatter_mines()
{
	kmine = mine;
	knull = 0;
	empty = size * size - mine;
	while (mine > 0)
	{
		int i = 1 + rand() % size;
		int j = 1 + rand() % size;
		if (grid[i][j] == 0) { grid[i][j] = 9; mine--; }
	}
}
// заповнення ігрового поля цифрами, підрховуючи кількість мін, які оточують кожну комірку
void game()
{
	for (int i = 1; i <= size; i++)
		for (int j = 1; j <= size; j++)
		{
			sgrid[i][j] = 10;
			grid[i][j] = 0;
		}
	scatter_mines();
	for (int i = 1; i <= size; i++)
		for (int j = 1; j <= size; j++)
		{
			int n = 0;
			if (grid[i][j] == 9) continue;
			if (grid[i + 1][j] == 9) n++;
			if (grid[i][j + 1] == 9) n++;
			if (grid[i - 1][j] == 9) n++;
			if (grid[i][j - 1] == 9) n++;
			if (grid[i + 1][j + 1] == 9) n++;
			if (grid[i - 1][j - 1] == 9) n++;
			if (grid[i - 1][j + 1] == 9) n++;
			if (grid[i + 1][j - 1] == 9) n++;
			grid[i][j] = n;

		}
}
//відкриття порожніх комірок навколо порожньої комірки, відкритої гравцем
void around(int i, int j)
{
	if (sgrid[i][j] == 0)
	{
		if (sgrid[i + 1][j] == 10) { sgrid[i + 1][j] = grid[i + 1][j]; knull++; }
		if (sgrid[i][j + 1] == 10) { sgrid[i][j + 1] = grid[i][j + 1]; knull++; }
		if (sgrid[i - 1][j] == 10) { sgrid[i - 1][j] = grid[i - 1][j]; knull++; }
		if (sgrid[i][j - 1] == 10) { sgrid[i][j - 1] = grid[i][j - 1]; knull++; }
		if (sgrid[i + 1][j + 1] == 10) { sgrid[i + 1][j + 1] = grid[i + 1][j + 1]; knull++; }
		if (sgrid[i - 1][j - 1] == 10) { sgrid[i - 1][j - 1] = grid[i - 1][j - 1]; knull++; }
		if (sgrid[i - 1][j + 1] == 10) { sgrid[i - 1][j + 1] = grid[i - 1][j + 1]; knull++; }
		if (sgrid[i + 1][j - 1] == 10) { sgrid[i + 1][j - 1] = grid[i + 1][j - 1]; knull++; }
	}
}
//відкриття сусідніх порожніх комірок на вьому ігровом полі, відносно відкритих
void arounds(int i, int j)
{
	around(i, j);
	for (int k = 1; k <= size; k++)
		for (i = 2; i <= size - 1; i++)
			for (j = 2; j <= size - 1; j++)
			{
				if (sgrid[i + 1][j] == 0) { around(i + 1, j); }
				if (sgrid[i + 1][j + 1] == 0) { around(i + 1, j + 1); }
				if (sgrid[i + 1][j - 1] == 0) { around(i + 1, j - 1); }
				if (sgrid[i - 1][j + 1] == 0) { around(i - 1, j + 1); }
				if (sgrid[i - 1][j - 1] == 0) { around(i - 1, j - 1); }
				if (sgrid[i - 1][j] == 0) { around(i - 1, j); }
				if (sgrid[i][j + 1] == 0) { around(i, j + 1); }
				if (sgrid[i][j - 1] == 0) { around(i, j - 1); }
			}
}
//створення та відображення фінального вікна: виграшу або програшу
void win_over(RenderWindow& window, Sprite& f, int sizex, int sizey)
{
	RenderWindow windowfin(VideoMode(size * w, sizey), "", Style::None);
	f.setPosition(size * w / 2 - sizex / 2, 0);
	bool fin = 1;
	while (fin)
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (IntRect(0, 0, size * w, sizey).contains(Mouse::getPosition(windowfin)))
			{
				windowfin.close();
				fin = 0;
				window.create(VideoMode(weight, height), "", Style::None);
				menu(window);
				game();
			}
		}
		windowfin.clear(Color::White);
		windowfin.draw(f);
		windowfin.display();
	}

}



int main()
{
	Texture over, win;
	over.loadFromFile("resources/over.png");
	win.loadFromFile("resources/win.png");
	Vector2u sizeover = over.getSize(), sizewin = win.getSize();
	Sprite gameover(over), gamewin(win);

	srand(time(0));
	RenderWindow window(VideoMode(weight, height), "", Style::None);
	menu(window);
	game();

	while (window.isOpen())
	{
		Font font;
		font.loadFromFile("resources/Vinograd.otf");
		Text tmine("", font, 38);

		tmine.setFillColor(Color::Red);
		tmine.setPosition(size + w, size * w + w / 2);

		int x = 0;
		int y = 0;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == event.MouseButtonReleased)
			{
				Vector2i pos = Mouse::getPosition(window);
				x = pos.x / w;
				y = pos.y / w;
			}
			if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right)
			{
				if (sgrid[x][y] == 10)
				{
					if (kmine > 0)
					{
						sgrid[x][y] = 11;
						kmine--;
					}
				}
				else if (sgrid[x][y] == 11) { sgrid[x][y] = 10; kmine++; }
			}
			if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left)
			{
				if (sgrid[x][y] == 10 || sgrid[x][y] == 11)  knull++;
				if (sgrid[x][y] == 11) kmine++;
				sgrid[x][y] = grid[x][y];

				if (IntRect(w, w, size * w, size * w).contains(Mouse::getPosition(window)))
				{
					arounds(x, y);
				}
			}
		}
		window.clear(Color::White);
		field(window, x, y);
		tmine.setString("Flags : " + intToString(kmine));
		window.draw(tmine);
		window.display();
		//виграш
		if (knull == empty)
		{
			win_over(window, gamewin, sizewin.x, sizewin.y);
		}
		//програш
		if (sgrid[x][y] == 9)
		{
			win_over(window, gameover, sizeover.x, sizeover.y);
		}
	}
	return 0;
}

