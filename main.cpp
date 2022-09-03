/*
This is my first project made in C++ for school as a project and I couldn't think of anything else so I came up with a Turn-based RPG game in console format.
I was a gamer back in college and the only thing I could think on top of my head was... Games so thats how it is born

Why did I make this boring program?
Well it is a project for school and second it demonstrates my C++ Skills and apply it to build a project. The main benefit is that I am able to get more skills as a
programmer and general skills in general.

At least it is finished, you can hunt enemies, get better equips, level up and move on to different area and so on. Rinse and repeat. Woot Woot.

-Steven Wang (3/17/2017)
*/

#include <iostream>
#include <string>
#include <windows.h>
#include <iomanip>
#include "Game.h"
#include <fstream>
#include <iostream>
#include "Utility.h"

std::string output = "";
//The main function is what runs the console program. The reason it returns int is mainly for the exit code functionality.
//Whe no main with parameters? I don't need those parameters and main without parameters is much simpler and easier to understand for me.
int main() {
MainMenu:
	system("cls");
	//Begin by displaying hello world onto the console
	std::cout << "Hello world!" << std::endl;
	std::cout << "IDLE RPG" << std::endl
		<< "1) New Game" << std::endl
		<< "2) Load Game" << std::endl
		<< "3) About" << std::endl
		<< "4) Quit" << std::endl;
	if (output == "2") {
		std::cout << "Error: No save file found to load..." << std::endl;
	}
	else if (output != "3" && output != "") {
		std::cout << "Error: invalid choice, please choose the apprpriate number" << std::endl;
	}
	std::cin >> output;

	//Results
	//1) New Game
	if (output == "1") {
	NamePlayer:
		system("cls");
		std::cout << "Please enter your name" << std::endl << "[type 'quit' to return back to main menu]" << std::endl;
		std::getline(std::cin, output);
		if (output == "quit") {
			goto MainMenu;
		}
		//std::cin >> output;
		if (output != "") {
			std::cout << "Welcome " << output << std::endl;
			//Sleep(3000);
			system("cls");
			Game::userName = output;
			
			Game::Init();
			for (int i =0; i <ItemDB::GetCount();i++)
				Game::inventory.push_back(InventoryItem(ItemDB::GetItem(i)));
			Game::equip_list.push_back(EquipDB::GetEquip(5));
			Game::MainLoop();
		}
		else {
			goto NamePlayer;
		}
	}
	//2) Load Game
	else if (output == "2") {
		system("cls");
		Game::Init();
		std::ifstream file;
		file.open("stuff.sav");
		bool _fileOpen = file.is_open();
		file.clear(); //clear error of filestream
		file.seekg(0, std::ios::beg);
		if (_fileOpen) {
			std::string get_line;
			getline(file, get_line);
			Game::userName = get_line;
			while (getline(file, get_line)) {
				auto vector = Utility::SplitText(get_line, "=");
				
				std::string header = vector[0];
				std::string value = vector[1];
				//Get Text.
				if (header == "HP") {
					Game::max_health = stoi(value);
				}
				if (header == "MP") {
					Game::max_mana = stoi(value);
				}
				if (header == "ATK") {
					Game::attack = stoi(value);
				}
				if (header == "DEF") {
					Game::defense = stoi(value);
				}
				if (header == "LV") {
					Game::level = stoi(value);
				}
				if (header == "EXP") {
					Game::exp = stoi(value);
				}
				if (header == "MAXEXP") {
					Game::max_exp = stoi(value);
				}
				if (header == "GOLD") {
					Game::gold = stoi(value);
				}
				if (header == "ITEM") {
					auto item_count = Utility::SplitText(value, " ");
					for (int i = 0; i < item_count.size(); i++) {
						auto item = ItemDB::GetItem(i);
						InventoryItem get = item;
						get.quantity = stoi(item_count[i]);
						Game::inventory.push_back(get);
					}
				}
				if (header == "EQUIP") {
					auto stats = Utility::SplitText(value, "|");
					Game::equip_list.push_back(new Equip{ stats[0],stoi(stats[1]),stoi(stats[2]),stoi(stats[3]),(EquipCategory)stoi(stats[4]) });
				}
				if (header == "HAT") {
					auto stats = Utility::SplitText(value, "|");
					Game::equip_hat = new Equip{ stats[0],stoi(stats[1]),stoi(stats[2]),stoi(stats[3]),(EquipCategory)stoi(stats[4]) };
				}
				if (header == "TOP") {
					auto stats = Utility::SplitText(value, "|");
					Game::equip_shirt = new Equip{ stats[0],stoi(stats[1]),stoi(stats[2]),stoi(stats[3]),(EquipCategory)stoi(stats[4]) };
				}
				if (header == "BOTTOM") {
					auto stats = Utility::SplitText(value, "|");
					Game::equip_pants= new Equip{ stats[0],stoi(stats[1]),stoi(stats[2]),stoi(stats[3]),(EquipCategory)stoi(stats[4]) };
				}
				if (header == "GLOVE") {
					auto stats = Utility::SplitText(value, "|");
					Game::equip_gloves= new Equip{ stats[0],stoi(stats[1]),stoi(stats[2]),stoi(stats[3]),(EquipCategory)stoi(stats[4]) };
				}
				if (header == "SHOE") {
					auto stats = Utility::SplitText(value, "|");
					Game::equip_shoes= new Equip{ stats[0],stoi(stats[1]),stoi(stats[2]),stoi(stats[3]),(EquipCategory)stoi(stats[4]) };
				}
				if (header == "WEAPON") {
					auto stats = Utility::SplitText(value, "|");
					Game::equip_weapon = new Equip{ stats[0],stoi(stats[1]),stoi(stats[2]),stoi(stats[3]),(EquipCategory)stoi(stats[4]) };
				}
				if (header == "ZONE") {
					Game::current_zone = stoi(value);
				}
				if (header == "KILLS_LEFT") {
					Game::kill_req = stoi(value);
				}
				if (header == "GAME_LEVEL") {
					Game::game_level = stoi(value);
				}
			}
			Game::MainLoop();
		}
		else {
			goto MainMenu;
		}
	}
	else if (output == "3") {
		std::cout << "IDLE RPG" << std::endl;
		std::cout << "IDLE Rpg is a idle role playing turn-based game where the player goes on an adventure to battle countless of enemies." << std::endl;
		std::cout << "Unlike other turn-based games, you can choose to have the player perform their own actions without any cost!" << std::endl;
		std::cout << "Let the player fight and grind all the stuff for you so you can do your important activity. Heck you can leave your computer overnight and come back with good results!" <<std::endl << "You can also choose to play normally by using attacks/skills/items to defeat your foes" << std::endl;
		std::cout << "After slaying your enemies, you will accumulate gold! Spend your gold in stores to purchase items and equip that will help you on harder adventures!" << std::endl;
		std::cout << "Do you think what it got it takes to idle to victory? Hit the New/Load Game and start your adventure!" << std::endl;
		std::cout << std::endl << std::endl;
		std::cout << "This game is made by Steven Wang as his first game. Why it is console program? Because it is much simpler to make as there is no pictures, music, or animations required" << std::endl;
		std::cout << "Created Date: Sept 2017!" << std::endl;
		system("pause");
		goto MainMenu;
	}
	//3) Quit game
	else if (output == "4") {
		system("cls");
	}
	else {
		goto MainMenu;
	}
	//After the program leaves the main loop, we display this message in front of the user that we thanked them for playing our game and greet them farewell!
	std::cout << "Thank you for playing. See you soon!" << std::endl;
	//Because visual studio ends the console once return 0 is called, I found a neat function called system("pause") to pause the console and made it so that
	//any key  will then call the next function "return 0" to close the program by outputting the final value of 0.
	system("pause");
	//Once the main function returns 0, it leaves the main function in which the console program automatically closes because there is no need of that anymore.
	return 0;
}