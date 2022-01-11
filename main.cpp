#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <iostream>
#include <time.h>
#include "Game.h"
#include <SFML/Audio.hpp>
#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>

int WIDTH = 800;
int HEIGHT = 800;
int LEVEL = 1;

int main()
{
	ifstream inFile;
	ofstream outFile;
	inFile.open("record.txt");

	if (!inFile) {
		MessageBox(NULL, "Could not load record", "Error!", MB_OK | MB_ICONWARNING);
	}

	int Record;
	string name, str;
	getline(inFile, str);
	istringstream s(str);
	s >> name >> Record;
	sf::Font font;
	font.loadFromFile("Lobster_1.3.otf");
    sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("Crystal.wav")) {
		MessageBox(NULL, "Audio file wasn't found!", "Error!", MB_OK | MB_ICONWARNING);
	}
    sf::Sound sound;
    sound.setBuffer(buffer);
again:
	if (Record < LEVEL) Record = LEVEL;
	string te = "Current record holder : " + name + " " + std::to_string(Record);
	sound.play();

	//set textures, fonts and other things
	sf::RenderWindow panel(sf::VideoMode(WIDTH, HEIGHT), "Candy Crush");//create window (a.k.a working panel)
	panel.setFramerateLimit(120);//simple frame limit function, kinda useless by why not to add...
	
    sf::Texture texture_1;
    texture_1.loadFromFile("begin.png");//load the image
    sf::Sprite beginio(texture_1);//create variable for texture to load
	sf::Text rec(te, font);
	sf::Text currentLevel("Current level : " + std::to_string(LEVEL), font);
	currentLevel.setCharacterSize(20);//character size in pixels
	currentLevel.setStyle(sf::Text::Bold); 
	currentLevel.setFillColor(sf::Color::Black);
	currentLevel.setPosition(334, 145);//intial location and x and y to draw the data

	rec.setCharacterSize(20);
	rec.setStyle(sf::Text::Bold);
	rec.setFillColor(sf::Color::White);
	rec.setPosition(20, 720);

	panel.draw(beginio);
	panel.draw(rec);
	panel.draw(currentLevel);
	panel.display();
	while (panel.isOpen()){
		sf::Event event;
		while (panel.pollEvent(event)){//if keyboard was activated it pops the lasat performed action on keyboard
			//manage keyboard keys
			switch (event.type){
				case sf::Event::Closed: {
					panel.close();
					break; 
				}
				case sf::Event::MouseButtonPressed: {
					if (event.key.code == sf::Mouse::Left) {
						sf::Vector2i pos;
						pos = sf::Mouse::getPosition(panel);
						cout << pos.x << " " << pos.y << endl;
						if (pos.x > 20 && pos.x < 271 && pos.y > 362 && pos.y < 422) {//here if the mouse location when clicked
																					// is in the range of the Start, start if case is accessed
							Object_Game game(LEVEL);
							cout << "Button Enter: Game has started!" << endl;
							panel.close();//close menu
							sound.stop();//and stop the sound
							game.init();//run game
							if (game.LEVEL == LEVEL) goto again;//infinit loop
							else LEVEL++;
							goto again;//infintin loop 
						}
						if (pos.x > 27 && pos.x < 193 && pos.y > 516 && pos.y < 574) {//here if the mouse location when clicked
																					// is in the range of the exit, exit if case is accessed
							panel.close();
							cout << "Exit pressed. Game is closed" << endl;
						}
					}
					break;
				}
			}
		}
	}
	//write new record to a txt file to save it
	outFile.open("record.txt");
	if(Record < LEVEL) Record = LEVEL;
	outFile << name << " " << Record;
	inFile.close();
	outFile.close();
	return 0;
}