#include "SFML/Graphics.hpp"
#include <time.h>
#include <SFML/Graphics/Text.hpp>
#include <string>
#include "SFML/Window.hpp"
#include <iostream>
#include <SFML/Audio.hpp>
#include <windows.h>
int INF = 202051681;
using namespace std;
const int Size = 10;
const int RAZMER = 9;
const int planets = 8;
const int speed = 10;

//some textures
sf::Texture MATRIX_t, PLANETS_t, HEAD, victory;
int tileSize = 66;

//flags, when they are true, you cannot insert more new values
//and some other function do not work when either of this flags is true
bool SWAPING = false;
bool action_On = false;

struct Object_Game {
	Object_Game(int);//constructor

	sf::RenderWindow PANEL;
	sf::Vector2i position;//positions to access x and y locations
	
	int sc = 0;
	int LEVEL;
	int max;
	int POINTS;
	int x1, y1, x2, y2;//coordinates representing distance between the planets
	int x_c1 = INF, y_c1, x_c2, y_c2;//coordinates representing distance between the planets
	char action = '0';
	int XYCord[2] = { 0 };//indicates if first and second planet were selected
	void init();
	void ACTION();
	void UPDATE();
	void COMPUTE();
};

struct planet {
	int x, y;
	int colomn, row;
	int planetNumber;//Since I have 8 planets the range is [1,8]
	int nuLL;
	int alpha;//fading, and detecting when planets are crushed. Since when alpa is 0, it means that the planet is
				//crushed
	planet() {
		nuLL = 0;
		alpha = 255;
	}
}MATRIX[Size][Size];

Object_Game::Object_Game(int a) :PANEL(sf::VideoMode(800, 800), "candy crash") {//740, 480
	sf::Texture loading;
	loading.loadFromFile("loading.png");
	sf::Sprite load(loading);
	PANEL.draw(load);
	PANEL.display();
	srand(time(0));
	for (int i = 1; i <= RAZMER; i++) {
		for (int j = 1; j <= RAZMER; j++){
			MATRIX[i][j].planetNumber = rand() % planets;//randomly generate candy
			MATRIX[i][j].colomn = j;
			MATRIX[i][j].row = i;
			MATRIX[i][j].x = j * tileSize;
			MATRIX[i][j].y = i * tileSize;
			cout << MATRIX[i][j].planetNumber << " ";
		}
		cout << endl;
	}
	LEVEL = a;
	POINTS = (LEVEL * 100) * 50 / 100;
	if (LEVEL == 1) POINTS = 30;
	max = (POINTS / 10) - 2;
	if (LEVEL == 1) max = 3;
}

void Object_Game::ACTION(){
	sf::Event e;
	while (PANEL.pollEvent(e)) {
		switch (e.type) {
			case sf::Event::Closed: {
				PANEL.close();
				break;
			}
			case sf::Event::KeyReleased: {
				if (e.key.code == sf::Keyboard::Escape)
					PANEL.close();
				if (e.key.code == sf::Keyboard::I) {
					if (!SWAPING && !action_On) {
						cout << "Enter coordinates and action" << endl;
						cout << "Y[1-8], X[1-8], Action(d,u,l,r) : ";
						cin >> x1;
						cin >> y1;
						cin >> action;
						switch (action) {
						case 'u': {
							x2 = x1;
							y2 = y1 - 1;
							cout << "UP" << endl;
							x_c1 = x1;
							y_c1 = y1;
							x_c2 = x2;
							y_c2 = y2;
							break;
						}
						case 'd': {
							x2 = x1;
							y2 = y1 + 1;
							cout << "DOWN" << endl;
							x_c1 = x1;
							y_c1 = y1;
							x_c2 = x2;
							y_c2 = y2;
							break;
						}
						case 'l': {
							x2 = x1 - 1;
							y2 = y1;
							cout << "LEFT" << endl;
							x_c1 = x1;
							y_c1 = y1;
							x_c2 = x2;
							y_c2 = y2;
							break;
						}
						case 'r': {
							x2 = x1 + 1;
							y2 = y1;
							cout << "RIGHT" << endl;
							x_c1 = x1;
							y_c1 = y1;
							x_c2 = x2;
							y_c2 = y2;
							break;
						}
						default: {break; }
						}
					}
				}
				break;
			}
		}
	}

	//action
	action_On = false;
	//creates dx and dy that correcspend to speed on axis x and y
	//need for creating animation of planets that you see in the game
	for (int i = 1; i <= RAZMER; i++) {
		for (int j = 1; j <= RAZMER; j++) {
			int dx, dy;
			for (int k = 0; k < speed; k++) {//speed indicates how fast the action going to be
				dx = MATRIX[i][j].x - MATRIX[i][j].colomn*tileSize;
				dy = MATRIX[i][j].y - MATRIX[i][j].row*tileSize;
				if (dx) {
					if (dx == -1) dx = -1 * dx;
					MATRIX[i][j].x -= dx / abs(dx);
				}
				if (dy) {
					MATRIX[i][j].y -= dy / abs(dy);
				}
			}
			if (dx) action_On = true;
			if (dy) action_On = true;
		}
	}

	//if no action, fade planets that are crushed
	if (!action_On) {
		for (int i = 1; i <= RAZMER; i++) {
			for (int j = 1; j <= RAZMER; j++) {
				if (MATRIX[i][j].nuLL) {
					if (MATRIX[i][j].alpha > Size) {//if alpha more than certain amount make it fade
						MATRIX[i][j].alpha -= Size;//reduce the alpha parameter, make it fade
						action_On = true;
					}
				}
			}
		}
	}
	bool flag = false;
	for (int i = 1; i <= RAZMER; i++) {
		for (int j = 1; j <= RAZMER; j++) {//if there is empty element, flag = true, otherwise false from back interchange
			if (MATRIX[i][j].nuLL != 0) flag = true;
		}
	}

	//back if it is wrong combination
	if (SWAPING && !action_On) {
		if (!flag && (x_c1 >= 1 && x_c1 <= 8)) {
			planet planet1 = MATRIX[y_c1][x_c1], planet2 = MATRIX[y_c2][x_c2];
			int TEMP1, TEMP2;

			TEMP1 = planet1.row;
			planet1.row = planet2.row;
			planet2.row = TEMP1;

			TEMP2 = planet1.colomn;
			planet1.colomn = planet2.colomn;
			planet2.colomn = TEMP2;

			MATRIX[planet1.row][planet1.colomn] = planet1;
			MATRIX[planet2.row][planet2.colomn] = planet2;

		}
		SWAPING = 0;
	}
}

//updating matrix every time this function is called
void Object_Game::UPDATE(){
	//if there is empty element(no planet), pull down the planet above
	if (!action_On){
		for (int i = RAZMER; i > 0; i--){
			for (int j = 1; j <= RAZMER; j++){
				//if null detected, it means that there is empty space
				if (MATRIX[i][j].nuLL){
					for (int k = i; k > 0; k--){
						if (!MATRIX[k][j].nuLL){//since we know that this place will not be empty
							//invert it and interchange with empty element
							planet planet1 = MATRIX[k][j], planet2 = MATRIX[i][j];
							int TEMP1, TEMP2;

							TEMP1 = planet1.row;
							planet1.row = planet2.row;
							planet2.row = TEMP1;

							TEMP2 = planet1.colomn;
							planet1.colomn = planet2.colomn;
							planet2.colomn = TEMP2;

							MATRIX[planet1.row][planet1.colomn] = planet1;
							MATRIX[planet2.row][planet2.colomn] = planet2;
							break;
						}
					}
				}
			}
		}
		//if matrix element is null meaning this planet is crushed and there is empty element
		//replace by a new randomly generated one from the top
		srand(time(0));
		for (int j = 1; j <= RAZMER; j++) {
			for (int i = RAZMER; i > 0; i--) {
				if (MATRIX[i][j].nuLL){//if empty, generate planet and set its values to initial conditions
					MATRIX[i][j].planetNumber = rand() % planets;//generate new one
					MATRIX[i][j].y = 0;//since its postion has to be 0 on y axis
					MATRIX[i][j].nuLL = 0;
					MATRIX[i][j].alpha = 255;
				}
			}
		}
	}
}

void Object_Game::COMPUTE(){
	if((x1>=1 && x1<=8) && (y1 >= 1 && y1 <= 8) && action != '0'){//here look if my coordianets were defined
																	//if they werent they would be equal to INF
		    //interchange two planets
			planet planet1, planet2;
			planet1 = MATRIX[y1][x1];
			planet2 = MATRIX[y2][x2];
			int TEMP1, TEMP2;

			TEMP1 = planet1.row;
			planet1.row = planet2.row;
			planet2.row = TEMP1;

			TEMP2 = planet1.colomn;
			planet1.colomn = planet2.colomn;
			planet2.colomn = TEMP2;

			MATRIX[planet1.row][planet1.colomn] = planet1;
			MATRIX[planet2.row][planet2.colomn] = planet2;

			SWAPING = 1;
			cout << "DELETED----------------" << endl;
			x1 = INF;
			x2 = INF;
			y1 = INF;
			y2 = INF;
			action = '0';
			max--;
	}

	//load some visual componenets 
	sf::Font f;
	f.loadFromFile("Lobster_1.3.otf");
	sf::Text chance("", f, 30);
	chance.setStyle(sf::Text::Bold);
	chance.setFillColor(sf::Color::Black);
	chance.setPosition(550, 720);
	chance.setString("Chance : " + std::to_string(max));

	sf::Font font;
	font.loadFromFile("Lobster_1.3.otf");
	sf::Text text("0", font);
	text.setCharacterSize(30);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color::Black);
	text.setPosition(150 - (to_string(sc).length()), 720);
	
	sf::Text level("1", font);
	level .setCharacterSize(20);
	level.setStyle(sf::Text::Regular);
	level.setFillColor(sf::Color::Black);
	level.setPosition(280, 70);
	level.setString("Level : " + std::to_string(LEVEL) + "|  Points needed : " + std::to_string(POINTS)); 

	text.setString("Score : " + std::to_string(sc));

	//check for crush
	for (int i = 1; i <= RAZMER; i++) {
		for (int j = 1; j <= RAZMER; j++) {
			//check the vertical axis
			//from planet and +1 and -1 of its location
			if ((MATRIX[i][j].planetNumber == MATRIX[i - 1][j].planetNumber)&&(MATRIX[i][j].planetNumber == MATRIX[i + 1][j].planetNumber)) {
				for (int k = -1; k <= 1; k++) {
					//decalre as null, check if this planet need to be crushed
					MATRIX[i + k][j].nuLL++;
				}
			}
			//horizontal same as vertical
			if ((MATRIX[i][j].planetNumber == MATRIX[i][j - 1].planetNumber) && (MATRIX[i][j].planetNumber == MATRIX[i][j + 1].planetNumber)) {
				for (int k = -1; k <= 1; k++) {
					MATRIX[i][j + k].nuLL++;
				}
			}
		}
	}

	int check = 0;

	for (int i = 1; i <= RAZMER; i++) {
		for (int j = 1; j <= RAZMER; j++) {
			//check if there is no action right now, and calculate how many 
			//planets were crushed in a row, for this you may recieve more points
			if(action_On == false) check += MATRIX[i][j].nuLL;
		}
	}
	//points, if combination detected
	if (check > 0) {
		cout << "Matched is " << check << endl;
		if (check == 3) sc += 10;
		else if (check > 3 && check <= 6) sc += 20;
		else if (check > 6 && check <= 9) sc += 25;
		else if (check > 9 && check <= 12) sc += 40;
		else if (check > 12) sc += 50;
	}

	//load some visuals
	MATRIX_t.loadFromFile("grid.png");
	PLANETS_t.loadFromFile("planets.png");
	sf::Sprite MATRIX_s(MATRIX_t);
	sf::Sprite PLANETS_s(PLANETS_t);
	if (sc < POINTS) {
		PANEL.draw(MATRIX_s);
		for (int i = 1; i <= RAZMER; i++) {
			for (int j = 1; j <= RAZMER; j++) {
				//planet p = MATRIX[i][j];
				//Set the sub-rectangle of the texture that the sprite will display.
				//since my loaded spires are 49x49, starting from 0, take every planet separetely
				PLANETS_s.setTextureRect(sf::IntRect(MATRIX[i][j].planetNumber * 49, 0, 49, 49));
				PLANETS_s.setColor(sf::Color(255, 255, 255, MATRIX[i][j].alpha));
				PLANETS_s.setPosition(MATRIX[i][j].x, MATRIX[i][j].y);
				PLANETS_s.move(44, 44);//since there is a distance between the point where you want to draw you planets
										//and tiles, move your sprites by a certain ammount
										//of pixels, to allight them with tiles
				PANEL.draw(PLANETS_s);
				PANEL.draw(chance);
				PANEL.draw(text);
				PANEL.draw(level);//draw everything on the panel
			}
		}
		PANEL.display();//display everything
	}
	//if scored points > need points, u won
	if (sc > POINTS){
		PANEL.clear();
		victory.loadFromFile("congrats.png");
		sf::Sprite victory_s(victory);
		PANEL.draw(victory_s);
		PANEL.display();
		Sleep(5000);
		PANEL.close();
	}
}

void Object_Game::init() {
	PANEL.clear();
	sf::SoundBuffer buffer;//from <SFML/Audio.hpp>//positive.wav
	if (!buffer.loadFromFile("Hydrogen.wav")) {
		MessageBox(NULL, "Audio file wasn't found!", "Error!", MB_OK | MB_ICONWARNING);
	}
	sf::Sound sound;//variable that plays a music
	sound.setBuffer(buffer);
	sound.play();
	while (PANEL.isOpen() && max != 0) {
		ACTION();
		UPDATE();
		COMPUTE();
	}
	if (sc < POINTS) {//if game is finished and you dont have enough score, you fail the level and loser.png is loaded
		PANEL.clear();
		sf::Texture l;
		l.loadFromFile("loser.png");
		sf::Sprite L(l);
		PANEL.draw(L);
		PANEL.display();
		Sleep(3000);
	}
	else LEVEL++;
}