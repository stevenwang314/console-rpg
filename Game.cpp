#include "Game.h"
#include <conio.h>
#include <Windows.h>
#include <thread>
#include "Enemies.h"
#include "Utility.h"
#include "GameState.h"
#include <limits>
#include <algorithm>
#include <fstream>

static const float EXP_RATE = 1.f;
static const float GOLD_RATE = 1.f;

Equip*  Game::equip_hat;
Equip*  Game::equip_shirt;
Equip*  Game::equip_pants;
Equip*  Game::equip_gloves;
Equip*  Game::equip_shoes;
Equip*  Game::equip_weapon;
int Game::gold=0;
std::string Game::userName = "";
int  Game::health = 100;
int  Game::max_health=100;
int  Game::attack = 10;
int  Game::defense = 5;
int Game::mana=100;
int Game::max_mana=100;
int  Game::level = 1;
int Game::exp=0;
int Game::max_exp=10;
std::string Game::enemyName="";
int Game::enemy_health=100;
int Game::enemy_attack=10;
int Game::enemy_defense=5;
int Game::enemy_level=1;
int Game::enemy_dodge = 0;
std::vector<Skill> Game::enemy_skill_list;

std::vector<Equip*> Game::equip_list;
std::string player_message = "";
std::string enemy_message = "";
std::string system_message = "";

bool Game::player_attack_up;
int Game::player_attack_turn = 0;;
int Game::enemy_exp;
int Game::enemy_gold;
bool Game::poison;
int Game::poison_turn;
bool Game::enemy_defense_up;
int Game::enemy_defense_up_turn;
bool  Game::attack_down = false;
int  Game::attack_down_turn = 0;
bool Game::enemy_attack_up = false;
int Game::enemy_attack_up_turn=0;
bool  Game::blind;
int Game::blind_turn;
bool Game::stun;
int Game::stun_turn;
bool Game::player_defense_up;
int Game::player_defense_turn;
int Game::water_bottle;
int Game::enemy_max_health = 0;
bool is_active = true;

std::vector<InventoryItem> Game::inventory;
int player_attack = 1;
bool exit_game = false;
bool enable_input = true;
bool idle_mode = false;
bool in_potion_menu = false;
GameState gameState = GameState::Menu;

bool  Game::defense_down = false;
int Game::defense_down_turn=0;

int Game::game_level=1;
int Game::current_zone=0;
int Game::kill_req=50;
bool Game::burn = false;
int Game::burn_turn = 0;
int Game::burn_damage = 0;
bool boss_fight = false;
std::vector<std::string> Area = { "Grasslands","Desert","Mountain","Beach","Volcano","Abyss" };

bool Game::silence = false;
int Game::silence_turn = 0;
int potion_page = 0;
#undef max
#undef min
void Game::Init()
{
	std::thread thr(Game::Input);
	thr.detach();
	//Area 1
	EnemyData::AddEnemy({ "Slime", 80,12,3,10,3 , std::vector<Skill>{Skill("attacked",SkillType::Attack,1.0f)} });
	EnemyData::AddEnemy({ "Mushroom",120,14,7 ,13,6, std::vector<Skill>{Skill("attacked", SkillType::Attack,1.0f)} });
	EnemyData::AddEnemy({ "Snail",150,12,6,15,8 ,  std::vector<Skill>{ Skill("attacked",SkillType::Attack,1.0f)} });
	EnemyData::AddEnemy({ "Pig",175,20,8,18,12,  std::vector<Skill>{ Skill("attacked",SkillType::Attack,1.0f)} });
	EnemyData::AddEnemy({ "Rabbit",250,25,10,25,15,  std::vector<Skill>{ Skill("attacked",SkillType::Attack,1.0f)} });
	//Area 2
	EnemyData::AddEnemy({ "Cactus", 400,50,20,40,20, std::vector<Skill>{ Skill("poked",SkillType::Attack,1.0f),Skill("spiked",SkillType::Attack,2.25f,StatusType::DefenseDown,5)} });
	EnemyData::AddEnemy({ "Snake",500,65,30 ,50,30,  std::vector<Skill>{ Skill("bite",SkillType::Attack,1.0f),Skill("poison",SkillType::Attack,1.5f,StatusType::Poison,5) } }); //Can inflict poison at 25% chance dealing 3% of the targets maximum health per turn
	EnemyData::AddEnemy({ "Lizard",650,42,15,50,30, std::vector<Skill>{ Skill("slapped",SkillType::Attack,1.0f),Skill("dodge",SkillType::Passive_Dodge,25)} }); //25% chance to dodge attacks
	EnemyData::AddEnemy({ "Sand Giant",1000,40,25,65,30, std::vector<Skill>{ Skill("attacked",SkillType::Attack,1.0f),Skill("buffed_up",SkillType::Buff_Defense,0.0f,StatusType::DefenseUp,5)} }); //Can use shield to block 50% damage for 3 turns
	EnemyData::AddEnemy({ "Coyote",500,75,8,60,25, std::vector<Skill>{ Skill("attacked",SkillType::Attack,1.0f),Skill("double hit",SkillType::Passive_DoubleHit,30)} }); //25% chance to strike again
	//Area 3
	EnemyData::AddEnemy({ "Tiger", 1350,150,75,90,65 , std::vector<Skill>{Skill("attacked",SkillType::Attack,1.0f),Skill("slashed",SkillType::Attack,2.5f)} });
	EnemyData::AddEnemy({ "Lion",1600,110,105 ,95,70, std::vector<Skill>{Skill("attacked", SkillType::Attack,1.0f),Skill("slashed",SkillType::Attack,2.25f)} });
	EnemyData::AddEnemy({ "Bear",2500,120,80,100,75 ,  std::vector<Skill>{ Skill("attacked",SkillType::Attack,1.0f),Skill("wound",SkillType::Attack,3.3f,StatusType::DefenseDown,5)} });
	EnemyData::AddEnemy({ "Lynx",1200,150,55,115,80,  std::vector<Skill>{ Skill("attacked",SkillType::Attack,1.0f),Skill("dodge",SkillType::Passive_Dodge,55)} });
	EnemyData::AddEnemy({ "Wolf",1800,180,40,125,90,  std::vector<Skill>{ Skill("attacked",SkillType::Attack,1.0f),Skill("wound",SkillType::Attack,3.3f,StatusType::DefenseDown,5),Skill("fear",SkillType::Attack,0.2f,StatusType::AttackDown,10)} });
	//Area 4
	EnemyData::AddEnemy({ "Crab", 3200,250,150,200,155 , std::vector<Skill>{Skill("slashed",SkillType::Attack,1.0f),Skill("pinched",SkillType::Attack,1,StatusType::Stun,1)} });
	EnemyData::AddEnemy({ "Dolphin",3500,200,200 ,225,170, std::vector<Skill>{Skill("attacked", SkillType::Attack,1.0f),Skill("double hit",SkillType::Passive_DoubleHit,30)} });
	EnemyData::AddEnemy({ "Shark",4000,320,180,250,210 ,  std::vector<Skill>{ Skill("bite",SkillType::Attack,1.0f),Skill("wound",SkillType::Attack,2.1f,StatusType::DefenseDown,5)} });
	EnemyData::AddEnemy({ "Squid",3800,280,220,265,255,  std::vector<Skill>{ Skill("whipped",SkillType::Attack,1.0f),Skill("ink_spray",SkillType::Attack,1.5f,StatusType::Blind,5)} });
	EnemyData::AddEnemy({ "Piranha",2800,300,125,305,280,  std::vector<Skill>{ Skill("bite",SkillType::Attack,1.0f),Skill("wound",SkillType::Attack,2.5f,StatusType::DefenseDown,5)} });
	//Area 5
	EnemyData::AddEnemy({ "Battlehound", 6500,550,650,350,300 , std::vector<Skill>{Skill("bite",SkillType::Attack,1.0f),Skill("ferocious bite",SkillType::Attack,3.3f),Skill("Swipe",SkillType::Attack,0.5f)} });
	EnemyData::GetData(20).skill[2].SetMaxAttacks(4);
	EnemyData::AddEnemy({ "Fire spirit",7000,700,400 ,395,350, std::vector<Skill>{Skill("attacked", SkillType::Attack,1.0f),Skill("Burn",SkillType::Attack,1.5,StatusType::Burn,3)} });
	EnemyData::AddEnemy({ "Salamander",5000,680,300,410,450 ,  std::vector<Skill>{ Skill("bite",SkillType::Attack,1.0f),Skill("dodge",SkillType::Passive_Dodge,40), Skill("tail strike",SkillType::Passive_DoubleHit,35.f) } });
	EnemyData::AddEnemy({ "Magma Golem",12000,800,500,450,475,  std::vector<Skill>{ Skill("whipped",SkillType::Attack,1.0f),Skill("Immolate",SkillType::Attack,3.25f,StatusType::Burn,5),Skill("Barrier",SkillType::Buff_Defense,0,StatusType::DefenseUp,4)} });
	EnemyData::AddEnemy({ "Dark Fiend",8000,450,300,520,505,  std::vector<Skill>{ Skill("bite",SkillType::Attack,1.0f),Skill("curse",SkillType::Attack,1.85f,StatusType::Blind,5),Skill("Ignition",SkillType::Attack,2.4f,StatusType::Burn,5)} });
	//Area 6 - FInal area
	EnemyData::AddEnemy({ "Abyssal Guard", 30000,800,1500,850,750 , std::vector<Skill>{Skill("attacked",SkillType::Attack,1.0f),Skill("guard",SkillType::Buff_Defense,0,StatusType::DefenseUp,8)} });
	EnemyData::AddEnemy({ "Abyssal Slayer", 42000,1500,800,840,720 , std::vector<Skill>{Skill("attacked",SkillType::Attack,1.0f),Skill("imperial strike",SkillType::Attack,0.75f),Skill("swipe",SkillType::Passive_Dodge,33.f) } });
	EnemyData::AddEnemy({ "Corrupted Eye", 27500,1200,1200,900,800 , std::vector<Skill>{Skill("attacked",SkillType::Attack,1.0f),Skill("eye strike",SkillType::Attack,1.2f), Skill("laser burn",SkillType::Attack,3.f,StatusType::Burn,4) } });
	EnemyData::AddEnemy({ "Grim Reaper", 25000,1300,900,820,680 , std::vector<Skill>{Skill("double_strike",SkillType::Passive_DoubleHit,25.f),Skill("swipe",SkillType::Attack,1.0f),Skill("forever curse",SkillType::Attack,1.3f,StatusType::Blind,10), Skill("Scythe Strike",SkillType::Attack,3.3f,StatusType::DefenseDown,4) } });
	
	EnemyData::GetData(27).skill[1].SetMaxAttacks(3);
	EnemyData::GetData(26).skill[1].SetMaxAttacks(3);
	
	EnemyData::AddBossEnemy({ "King Slimy Mushroom", 3500,35,35,250,150 , std::vector<Skill>{Skill("slap",SkillType::Attack,1.2f), Skill("Stomp",SkillType::Attack,1.5f), Skill("pounced",SkillType::Attack,3.0f)} });
	EnemyData::AddBossEnemy({ "Mud Golem", 12500,95,55,750,500 , std::vector<Skill>{Skill("pummel",SkillType::Attack,1.2f), Skill("sand attack",SkillType::Attack,2.2f,StatusType::Blind,2), Skill("glued",SkillType::Attack,1.5f,StatusType::Silence,2), Skill("Mud Slap",SkillType::Attack,3.f)} });
	EnemyData::AddBossEnemy({ "Dragon", 50000,240,150,1500,1000 , std::vector<Skill>{Skill("strike",SkillType::Attack,1.2f), Skill("whip",SkillType::Attack,0.7f), Skill("bite",SkillType::Attack,2.5f,StatusType::DefenseDown,5), Skill("Enrage",SkillType::Buff_Attack,3.f) ,Skill("breathe fire",SkillType::Attack,2.8f,StatusType::Poison,8) } });
	EnemyData::AddBossEnemy({ "Kraken", 100000,400,250,3500,2000 , std::vector<Skill>{Skill("strike",SkillType::Attack,1.f), Skill("stab",SkillType::Attack,3.3f), Skill("ink spray",SkillType::Attack,0.75f,StatusType::Blind,8), Skill("Enrage",SkillType::Buff_Attack,5.f) ,Skill("Jabbed",SkillType::Attack,2.0f,StatusType::Stun,1) } });
	EnemyData::AddBossEnemy({ "Molten Golem", 333333,1050,500,15000,10000 , std::vector<Skill>{Skill("strike",SkillType::Attack,1.f), Skill("burning",SkillType::Attack,1.5f,StatusType::Burn,3), Skill("Enrage",SkillType::Buff_Attack,5.f), Skill("Enrage",SkillType::Buff_Defense,5.f) } });
	EnemyData::AddBossEnemy({ "Death Seeker", 1000000,2222,1337,0,0 , std::vector<Skill>{Skill("strike",SkillType::Attack,1.f),Skill("death scythed",SkillType::Attack,0.69f),Skill("obliterate",SkillType::Attack,4.2f), Skill("burning",SkillType::Attack,2.1f,StatusType::Burn,10), Skill("decayed",SkillType::Attack,1.69f,StatusType::Silence,5), Skill("ripped",SkillType::Attack,2.71f,StatusType::Stun,2),Skill("healed",SkillType::Heal,6.9f)} });
	EnemyData::GetBossData(2).skill[1].SetMaxAttacks(3);
	EnemyData::GetBossData(3).skill[1].SetMaxAttacks(5);
	EnemyData::GetBossData(5).skill[1].SetMaxAttacks(4);
	EquipDB::AddEquip(new Equip{ "Basic Hat",15,0,5,EquipCategory::Hat ,200 });
	EquipDB::AddEquip(new Equip{ "Basic Shirt",10,0,4,EquipCategory::Top,200 });
	EquipDB::AddEquip(new Equip{ "Basic Pants",0,3,4,EquipCategory::Bottom,200 });
	EquipDB::AddEquip(new Equip{ "Basic Glove",0,6,3,EquipCategory::Glove,200 });
	EquipDB::AddEquip(new Equip{ "Basic Shoes",10,0,3,EquipCategory::Shoes,200 });
	EquipDB::AddEquip(new Equip{ "Basic Sword",0,12,3,EquipCategory::Weapon,500 });
	EquipDB::AddEquip(new Equip{ "Brown-Leathered Hat",50,0,10,EquipCategory::Hat ,1000 });
	EquipDB::AddEquip(new Equip{ "Brown-Leathered Shirt",35,0,5,EquipCategory::Top,1000 });
	EquipDB::AddEquip(new Equip{ "Brown-Leathered Pants",0,5,10,EquipCategory::Bottom,1000 });
	EquipDB::AddEquip(new Equip{ "Brown-Leathered Glove",0,10,8,EquipCategory::Glove,1000 });
	EquipDB::AddEquip(new Equip{ "Brown-Leathered Shoes",40,0,10,EquipCategory::Shoes,1000 });
	EquipDB::AddEquip(new Equip{ "Wooden Sword",0,35,12,EquipCategory::Weapon,1800 });
	EquipDB::AddEquip(new Equip{ "Silver Hat",150,0,15,EquipCategory::Hat ,2500 });
	EquipDB::AddEquip(new Equip{ "Silver Shirt",100,0,18,EquipCategory::Top,2500 });
	EquipDB::AddEquip(new Equip{ "Silver Pants",0,10,30,EquipCategory::Bottom,2500 });
	EquipDB::AddEquip(new Equip{ "Silver Glove",50,20,20,EquipCategory::Glove,2500 });
	EquipDB::AddEquip(new Equip{ "Silver Shoes",120,0,20,EquipCategory::Shoes,2500 });
	EquipDB::AddEquip(new Equip{ "Sharp-edge Sword",0,80,20,EquipCategory::Weapon,6000 });
	EquipDB::AddEquip(new Equip{ "Iron Hat",320,0,35,EquipCategory::Hat ,7000 });
	EquipDB::AddEquip(new Equip{ "Iron Shirt",250,0,25,EquipCategory::Top,7000 });
	EquipDB::AddEquip(new Equip{ "Iron Pants",100,35,55,EquipCategory::Bottom,7000 });
	EquipDB::AddEquip(new Equip{ "Iron Glove",250,40,40,EquipCategory::Glove,7000 });
	EquipDB::AddEquip(new Equip{ "Iron Shoes",250,0,50,EquipCategory::Shoes,7000 });
	EquipDB::AddEquip(new Equip{ "Titanium Sword",0,160,40,EquipCategory::Weapon,18000 });
	EquipDB::AddEquip(new Equip{ "Green Cotton Hat",800,0,70,EquipCategory::Hat ,22000 });
	EquipDB::AddEquip(new Equip{ "Green Cotton Shirt",700,0,60,EquipCategory::Top,22000 });
	EquipDB::AddEquip(new Equip{ "Green Cotton Pants",700,75,100,EquipCategory::Bottom,22000 });
	EquipDB::AddEquip(new Equip{ "Green Cotton Glove",450,80,90,EquipCategory::Glove,22000 });
	EquipDB::AddEquip(new Equip{ "Green Cotton Shoes",800,0,80,EquipCategory::Shoes,22000 });
	EquipDB::AddEquip(new Equip{ "Scimitar Sword",0,480,130,EquipCategory::Weapon,45000 });
	EquipDB::AddEquip(new Equip{ "Golden Eagle Hat",1800,0,150,EquipCategory::Hat , 70000 });
	EquipDB::AddEquip(new Equip{ "Golden Eagle Shirt",1500,150,100,EquipCategory::Top,70000 });
	EquipDB::AddEquip(new Equip{ "Golden Eagle Pants",1500,250,200,EquipCategory::Bottom,70000 });
	EquipDB::AddEquip(new Equip{ "Golden Eagle Glove",1500,300,180,EquipCategory::Glove,70000 });
	EquipDB::AddEquip(new Equip{ "Golden Eagle Shoes",1200,100,150,EquipCategory::Shoes,70000 });
	EquipDB::AddEquip(new Equip{ "Legendary Sword",0,1050,250,EquipCategory::Weapon,120000 });

	ItemDB::AddItem({ "Red Potion","Restores %hp HP",100,0 ,75,ItemType::Recovery });
	ItemDB::AddItem({ "Blue Potion","Restores %mp MP",0,60 ,100 ,ItemType::Recovery });
	ItemDB::AddItem({ "Medium Red Potion","Restores %hp HP",300,0,150,ItemType::Recovery });
	ItemDB::AddItem({ "Medium Blue Potion","Restores %mp MP",0,200,200 ,ItemType::Recovery });
	ItemDB::AddItem({ "Apple","Delicious Apple, Restores %hp HP",800,0,250 ,ItemType::Recovery });
	ItemDB::AddItem({ "Lemon","Very sour, Restores %mp MP",0,500,350,ItemType::Recovery });
	ItemDB::AddItem({ "Cake","Sweet Cake, Restores %hp HP and %mp MP",1000,1000,700,ItemType::Recovery });
	ItemDB::AddItem({ "Meat","Yum meat cooked and grilled, Restores %hp HP",2500,0,1000,ItemType::Recovery });
	ItemDB::AddItem({ "Orange Juice","Yummy Drink, Restores %mp MP",0,2000,1200 ,ItemType::Recovery });
	ItemDB::AddItem({ "Biscuit","Portable Biscuit, Restores %hp HP",4000,0,1800,ItemType::Recovery });
	ItemDB::AddItem({ "Sandwich","Delicious Sandwitch, Restores %hp HP",10000,0,2500,ItemType::Recovery });
	ItemDB::AddItem({ "Mountain Dew","Fresh from the dew, Restores %mp MP",0,4000,2800,ItemType::Recovery });
	ItemDB::AddItem({ "Watermeleon","Heavy Watermelon, Restores %hp HP and %mp MP",25000,5000,5000,ItemType::Recovery });
	ItemDB::AddItem({ "Doritos","Triangular Chips that sets you on fire, Restores %hp HP and %mp MP",42069,9001,9001,ItemType::Recovery });
	ItemDB::AddItem({ "Antidote","Removes Poison Status Effect",0,0,100,ItemType::Antidote });
	ItemDB::AddItem({ "Eyedrop","Removes Blind Status Effect",0,0,150,ItemType::EyeDrop });
	ItemDB::AddItem({ "Garlic","Removes Stat debuffs Status Effect",0,0,150 ,ItemType::Garlic});
	ItemDB::AddItem({ "Water Bottle","Prevents any burned damage onto the character for 12 turns.",0,0,200 ,ItemType::WaterBottle });
	ItemDB::AddItem({ "Medicine","Removes all forms of Status Effect",0,0,500,ItemType::Cure });
	Reset();
}
void Game::SaveGame()
{
	std::cout << "Your progress has been saved! Press any key to continue." << std::endl;
	std::ofstream file;
	file.open("stuff.sav");
	file << userName << std::endl;
	file << "HP=" << max_health << std::endl;
	file << "MP=" << max_mana << std::endl;
	file << "ATK=" << attack << std::endl;
	file << "DEF=" << defense << std::endl;
	file << "LV=" << level << std::endl;
	file << "EXP=" << exp << std::endl;
	file << "MAXEXP=" << max_exp << std::endl;
	file << "GOLD=" << gold << std::endl;
	file << "ZONE=" << current_zone << std::endl;
	file << "KILLS_LEFT=" << kill_req << std::endl;
	file << "GAME_LEVEL=" << game_level << std::endl;
	file << "ITEM=";
	for (size_t i = 0; i < inventory.size(); i++) {
		file << inventory[i].quantity;
		if (i + 1 < inventory.size()) {
			file << " ";
		}
	}
	file << std::endl;
	for (int i = 0; i < equip_list.size(); i++) {
		file << "EQUIP=" << equip_list[i]->name << "|" << equip_list[i]->health << "|" << equip_list[i]->attack << "|" << equip_list[i]->defense << "|" << (int)equip_list[i]->category << std::endl;

	}
	if (equip_hat)
		file << "HAT=" << equip_hat->name << "|" << equip_hat->health << "|" << equip_hat->attack << "|" << equip_hat->defense << "|" << (int)equip_hat->category << std::endl;
	if (equip_shirt)
		file << "TOP=" << equip_shirt->name << "|" << equip_shirt->health << "|" << equip_shirt->attack << "|" << equip_shirt->defense << "|" << (int)equip_shirt->category << std::endl;
	if (equip_pants)
		file << "BOTTOM=" << equip_pants->name << "|" << equip_pants->health << "|" << equip_pants->attack << "|" << equip_pants->defense << "|" << (int)equip_pants->category << std::endl;
	if (equip_gloves)
		file << "GLOVE=" << equip_gloves->name << "|" << equip_gloves->health << "|" << equip_gloves->attack << "|" << equip_gloves->defense << "|" << (int)equip_gloves->category << std::endl;
	if (equip_shoes)
		file << "SHOE=" << equip_shoes->name << "|" << equip_shoes->health << "|" << equip_shoes->attack << "|" << equip_shoes->defense << "|" << (int)equip_shoes->category << std::endl;
	if (equip_weapon)
		file << "WEAPON=" << equip_weapon->name << "|" << equip_weapon->health << "|" << equip_weapon->attack << "|" << equip_weapon->defense << "|" << (int)equip_weapon->category << std::endl;

	file.close();
	//Press any key
	int a = _getch();
}

void Game::Input() {
	
	while (!exit_game) {
		int get_key = 0;

		if (gameState == GameState::Fight) {
			int get_key = _getch();
			if (!in_potion_menu) {
				switch (get_key) {
					case 49: { //1 Key 
						if (player_attack == 1 && !idle_mode && !in_potion_menu) {
							AttackEnemy();
						}
						break;
					}
					case 50: { //2 Key 
						if (player_attack == 1 && !idle_mode && !in_potion_menu) {
							if (!silence)
								PowerStrike();
						}
						break;
					}
					case 51: { //3 Key 
						if (player_attack == 1 && !idle_mode && !in_potion_menu) {
							in_potion_menu = true;
							system_message = "";
						}
						break;
					}
					case 52: { //4 Key 
						if (player_attack == 1 && !idle_mode && !in_potion_menu) {
							if (!silence)
								AttackUp();
						}
						break;
					}
					case 53: { //5 Key 
						if (player_attack == 1 && !idle_mode && !in_potion_menu) {
							if (!silence)
								TripleStrike();
						}
						break;
					}
					case 54: { //6 Key 
						if (player_attack == 1 && !idle_mode && !in_potion_menu) {
							if (!silence)
								HealPlayer();
						}
						break;
					}
					case 55: { //7 Key 
						if (player_attack == 1 && !idle_mode && !in_potion_menu) {
							if (!silence)
								Protect();
						}
						break;
					}
					case 81:
					case 113: {//Q Key (virtual keys are 81/113 are upper/lower case 'Q')
						if (!in_potion_menu)
							idle_mode = !idle_mode;
						break;
					}
					case 75: //Left Arrow
						if (current_zone > 0) {
							//Check if we aren't in a boss fight.
							if (!boss_fight) {
								current_zone--;
								system_message = "Travelling to the west area...";
							}
							else {
								system_message = "Leaving Boss Fight...";
							}
							Reset();				
						}
						break;
					case 77: //Right Arrow
						if (current_zone < 6) {
							if (current_zone +1 < game_level) {
								current_zone++;
								Reset();
								system_message = "Travelling to the east area...";
							}
							else if (kill_req > 0) {
								//did not meet the Kill requirement								
							}
							else if (kill_req <= 0) {
								BossReset(game_level);
								system_message = "Engaging Boss Fight...";
								//Battle boss
							}
						}
						break;
					case VK_ESCAPE: {
						system("cls");
						enable_input = true;
						gameState = GameState::Menu;
						break;
					}
					case VK_RETURN: {
						if (!enable_input) {
							Reset();
						}
						break;
					}
				}
			}
			else {
				switch (get_key) {
				default:
						if (get_key >= 49 && get_key <= 57) {
							//Get the index of the item. Since we have item pages in the combat menu, we have to consider that. Each page contains 9 items.
							size_t get_index = (size_t)(get_key)-49 + (size_t)(potion_page) * 9;
							UsePotion(get_index);
						}
						if (get_key == 48) {
							in_potion_menu = false;
						}
						break;
				case 75: //Left Arrow
					if (potion_page > 0) {
						potion_page --;
					}
					break;
				case 77: //Right Arrow
					if (potion_page < 2) {
						potion_page += 1;
					}
					break;
				}
			}
		}
	}
}
void Game::EnemyAI()
{
	//player_message = "";
	if (stun) {
		player_attack = 0;
		player_message = "You couldn't do anything due to being stunned...\n";
	}
	if (idle_mode && player_attack == 1) {
	if (!silence) {

			is_active = false;
			if (mana >= 30 && (
				((boss_fight && 
					((current_zone != 6 && health < Utility::GenerateRandomInteger(enemy_attack * 2, enemy_attack * 5)) || 
					(current_zone == 6 && health < Utility::GenerateRandomInteger(enemy_attack * 4, enemy_attack * 8)))) ||
				(!boss_fight && health < Utility::GenerateRandomInteger((int)(enemy_attack * 1.5), (int)(enemy_attack * 2.5)))
			))) {
				HealPlayer();
				goto Finish;
			}
			if (mana >= 25 && max_health * (boss_fight ? 0.25 : 0.1) < enemy_attack * 1.5) {
				if (player_defense_turn <= 0) {
					Protect();
					goto Finish;
				}
			}

			if (mana >= 100 && boss_fight || !boss_fight) {

				if (mana >= 25 && Attack() * 5 < enemy_health) {
					if (player_attack_turn <= 0) {
						AttackUp();
						goto Finish;
					}
				}

				if (mana >= 35) {
					if (Attack() * 3 >= enemy_health) {
						if (Utility::GenerateRandomInteger(0, 99) <= (enemy_health / (Attack() * 3) * 100)) {
							TripleStrike();
							goto Finish;
						}
					}
					else {
						TripleStrike();
						goto Finish;
					}
				}
				//Player AI (Idle Mode ON Artificial Intelligence)
				if (mana >= 10) {

					if (Attack() * 2.5 >= enemy_health) {
						if (Utility::GenerateRandomInteger(0, 99) <= (enemy_health / (Attack() * 2.5) * 100)) {
							PowerStrike();
							goto Finish;
						}
						else {
							goto BasicAttack;
						}
					}
					else {
						PowerStrike();
						goto Finish;
					}

				}
			}
		}
	else if (boss_fight) {
		//Attempt to use potion if silenced.
		if (!UsePotion((size_t)18))
			goto BasicAttack;
		else
			goto Finish;
	}
	BasicAttack:
		AttackEnemy();
	Finish: {

		}

	}

	if (player_attack == 0) {
		enemy_message = "";
		//Reduce the debuff counter on each turn.
		Game::defense_down_turn--;
		Game::defense_down = (Game::defense_down_turn > 0);
		Game::poison_turn--;
		Game::poison = (Game::poison_turn > 0);
		Game::enemy_defense_up_turn--;
		Game::enemy_defense_up = (Game::enemy_defense_up_turn > 0);
		Game::enemy_attack_up_turn--;
		Game::enemy_attack_up = (Game::enemy_attack_up_turn > 0);
		Game::blind_turn--;
		Game::blind = (Game::blind_turn > 0);
		Game::stun_turn--;
		Game::stun = (Game::stun_turn > 0);
		Game::silence_turn--;
		Game::silence = (Game::silence_turn > 0);
		Game::player_attack_turn--;
		Game::player_attack_up = (Game::player_attack_turn > 0);
		Game::player_defense_turn--;
		Game::player_defense_up = (Game::player_defense_turn > 0);
		Game::burn_turn--;
		Game::burn = (Game::burn_turn > 0);
		Game::burn_damage = (int)(Game::burn_turn <= 0 ? 0 : Game::burn_damage * 1.02f);
		Game::water_bottle--;
		if (enemy_health > 0) {
			int index = Utility::GenerateRandomInteger(0, (int)enemy_skill_list.size() - 1);
			enemy_skill_list[index].CastToPlayer();
		}
		if (poison) {
			float damage = Game::max_health * 0.03f;
			Game::health -= (int)damage;
			player_message += "\nYou took " + Utility::ToString((float)damage) + " damage from poison.";
		}
		if (burn && water_bottle <= 0) {
			Game::health -= burn_damage;
			player_message += "\nYou took " + Utility::ToString((float)burn_damage) + " damage from burns.";
		}

	}
	if (idle_mode && !enable_input) {
		Reset();
	}
	Sleep(1000);
}
int Game::MaxHealth()
{
	return max_health +
		(equip_gloves ? equip_gloves->health : 0) +
		(equip_hat? equip_hat->health : 0) +
		(equip_shirt ? equip_shirt->health : 0) +
		(equip_pants? equip_pants->health : 0) +
		(equip_shoes ? equip_shoes->health : 0) +
		(equip_weapon ? equip_weapon->health : 0);
}
int Game::Defense()
{
	return  (int)((defense +
		(equip_gloves ? equip_gloves->defense : 0) + 
		(equip_hat ? equip_hat->defense : 0) +
		(equip_shirt ? equip_shirt->defense : 0) +
		(equip_pants ? equip_pants->defense : 0) + 
		(equip_shoes ? equip_shoes->defense : 0) + 
		(equip_weapon ?  equip_weapon->defense : 0)) * (defense_down ? 0.5 : 1) * (player_defense_up ? 1.5 : 1));
}
int Game::Attack()
{
	return  (int)((attack +
		(equip_gloves ? equip_gloves->attack : 0) +
		(equip_hat ? equip_hat->attack : 0) +
		(equip_shirt ? equip_shirt->attack : 0) +
		(equip_pants ? equip_pants->attack : 0) +
		(equip_shoes ? equip_shoes->attack : 0) +
		(equip_weapon ? equip_weapon->attack : 0)) * ((attack_down ? 0.5 : 1) * (player_attack_up ? 1.5 : 1)));
}

void Game::EquipMenu(std::vector<Equip*> eqp, EquipCategory type)
{

	size_t result = 0;
	while (true) {
		system("cls");
		std::cout << "(0) nothing" << std::endl;
		for (int i = 0; i < eqp.size(); i++) {
			std::cout << "(" << i + 1 << ") " << eqp[i]->ToString() << std::endl;
		}
		std::cin >> result;
		if (std::cin.bad()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else if (result == 0) {
			if (type == EquipCategory::Hat) {
				if (equip_hat) {
					equip_list.push_back(equip_hat);
					equip_hat = nullptr;
				}
			}
			if (type == EquipCategory::Shoes) {
				if (equip_shoes) {
					equip_list.push_back(equip_shoes);
					equip_shoes = nullptr;
				}
			}
			if (type == EquipCategory::Bottom) {
				if (equip_pants) {
					equip_list.push_back(equip_pants);
					equip_pants = nullptr;
				}
			}
			if (type == EquipCategory::Top) {
				if (equip_shirt) {
					equip_list.push_back(equip_shirt);
					equip_shirt = nullptr;
				}
			}
			if (type == EquipCategory::Weapon) {
				if (equip_weapon) {
					equip_list.push_back(equip_weapon);
					equip_weapon = nullptr;
				}
			}
			if (type == EquipCategory::Glove) {
				if (equip_gloves) {
					equip_list.push_back(equip_gloves);
					equip_gloves = nullptr;
				}
			}

			break;
		}
		else if (result-1 < eqp.size()) {
			//Remove equip from equip inventory when equip and retrieve it back when unequipped.
			if (eqp[result-1]->category == EquipCategory::Hat)
				equip_hat = eqp[result-1];
			if (eqp[result-1]->category == EquipCategory::Shoes)
				equip_shoes = eqp[result-1];
			if (eqp[result-1]->category == EquipCategory::Bottom)
				equip_pants = eqp[result-1];
			if (eqp[result-1]->category == EquipCategory::Top)
				equip_shirt = eqp[result-1];
			if (eqp[result-1]->category == EquipCategory::Weapon)
				equip_weapon = eqp[result-1];
			if (eqp[result-1]->category == EquipCategory::Glove)
				equip_gloves = eqp[result-1];
			//Erase-remove idiom.
			equip_list.erase(std::remove(equip_list.begin(), equip_list.end(), eqp[result - 1]));
			break;
		}
	}
}
void Game::AttackPlayer(std::string name, float value)
{

	int damage = Utility::GenerateRandomInteger((int)(0.5 * Game::EnemyAttack()), Game::EnemyAttack());
	damage = (int)(damage * Utility::ComputeDamageMult(Game::EnemyAttack(), Defense()) * value);
	health -= damage;
	enemy_message += enemyName + " " +  name +" you for " + std::to_string(damage) + " damage!\n";
	player_attack = 1;
}

int Game::EnemyAttack()
{
	return  (int)(enemy_attack * (enemy_attack_up ? 1.5 : 1));
}

int Game::EnemyDefense()
{
	return (int)(enemy_defense * (enemy_defense_up ? 1.5 : 1));
}
void Game::PowerStrike()
{
	if (mana >= 10) {
		player_message = "";
		enemy_message = "";
		if (Utility::GenerateRandomInteger(1, 100) > Game::enemy_dodge && Utility::GenerateRandomInteger(1, 100) > (blind ? 50 : 0)) {
			int damage = Utility::GenerateRandomInteger((int)(0.5 * Attack()), Attack());
			damage = (int)(damage * Utility::ComputeDamageMult(Attack(), enemy_defense) * 2.5);
			enemy_health -= damage;
			player_message = "You power strike " + enemyName + " for " + std::to_string(damage) + " damage!";

		}
		else {
			player_message = "You power strike " + enemyName + " but your attacked missed...";
		}

		player_attack = 0;
		mana -= 10;
	}
	else {
		system_message = "You do not have enough mana to cast Power Strike.";
	}
}
void Game::AttackUp()
{

	if (mana >= 25) {
		player_message = "";
		enemy_message = "";
		player_attack_up = true;
		player_attack_turn = 6;
		player_attack = 0;
		player_message = "You empowered up your weapon increasing your attack by 50% for 5 turns.";
		mana -= 25;
	}
	else {
		system_message = "You do not have enough mana to cast Rage.";
	}
}
void Game::TripleStrike()
{

	if (mana >= 35) {
		player_message = "";
		enemy_message = "";
		for (int i = 0; i < 3; i++) {
			if (Utility::GenerateRandomInteger(1, 100) > Game::enemy_dodge && Utility::GenerateRandomInteger(1, 100) > (blind ? 50 : 0)) {
				int damage = Utility::GenerateRandomInteger((int)(0.75 * Attack()), Attack());
				damage = (int)(damage * Utility::ComputeDamageMult(Attack(), Game::EnemyDefense()));
				enemy_health -= damage;
				player_message += "You strike " + enemyName + " for " + std::to_string(damage) + " damage!\n";

			}
			else {
				player_message += "You strike " + enemyName + " but your attacked missed...\n";
			}
		}
		mana -= 35;
		player_attack = 0;
	} 
	else {
		system_message = "You do not have enough mana to cast Triple Strike.";
	}

}
void Game::HealPlayer()
{

	if (mana >= 30) {
		player_message = "";
		enemy_message = "";

		int heal_amount = Utility::GenerateRandomInteger((int)(Attack() * 1.5), (int)(Attack() * 3));
		player_attack = 0;
		health += heal_amount;
		player_message = "You healed recovering " + Utility::ToString((float)heal_amount) + " health.";
		mana -= 30;
	}
	else {
		system_message = "You do not have enough mana to cast Rage.";
	}
}
void Game::Protect()
{
	if (mana >= 25) {
		player_message = "";
		enemy_message = "";
		player_defense_up = true;
		player_defense_turn = 6;
		player_attack = 0;
		player_message = "You have guarded yourself with your defenses increasing your defense by 50% for 5 turns.";
		mana -= 25;
	}
	else {
		system_message = "You do not have enough mana to cast Protect.";
	}
}
bool Game::UsePotion(size_t get_index)
{

	if (inventory[get_index].quantity > 0) {
		if (inventory[get_index].item.type == ItemType::Antidote) {
			if (Game::poison) {
				Game::poison = false;
				Game::poison_turn = 0;
				player_message = "You used a antidote to remove yourself from poison ";
				inventory[get_index].quantity--;
				in_potion_menu = false;
				player_attack = 0;
			}
			else {
				system_message = "You are not poisoned afflicting you.";
				return false;
			}
		}
		else  if (inventory[get_index].item.type == ItemType::Garlic) {
			if (Game::attack_down || Game::defense_down) {
				Game::attack_down_turn = 0;
				Game::defense_down_turn = 0;
				Game::attack_down = false;
				Game::defense_down = false;
				player_message = "You used a garlic to remove debuffs. ";
				inventory[get_index].quantity--;
				in_potion_menu = false;
				player_attack = 0;
			}
			else {
				system_message = "Error. There are no stat debuffs afflicting you.";
				return false;
			}
		}
		else  if (inventory[get_index].item.type == ItemType::EyeDrop) {
			if (Game::blind) {
				Game::blind_turn = 0;
				Game::blind = false;
				player_message = "You used a eyedrop to remove blind.";
				inventory[get_index].quantity--;
				in_potion_menu = false;
				player_attack = 0;
			}
			else {
				system_message = "Error. There are no blind afflicting you.";
				return false;
			}
		}
		else  if (inventory[get_index].item.type == ItemType::Cure) {
			if (Game::blind || Game::burn || Game::poison || Game::silence) {
				Game::blind_turn = 0;
				Game::blind = false;
				Game::burn = false;
				Game::burn_turn = 0;
				Game::poison = 0;
				Game::poison_turn = false;
				Game::silence = false;
				Game::silence_turn = 0;
				player_message = "You used a medicine to cure all status effects.";
				inventory[get_index].quantity--;
				in_potion_menu = false;
				player_attack = 0;
				return false;
			}
			else {
				system_message = "Error. There are no status effects afflicting you.";
			}
		}
		else  if (inventory[get_index].item.type == ItemType::WaterBottle) {
			water_bottle = 15;
			player_message = "You used a water bottle to protect yourself from fire.";
			inventory[get_index].quantity--;
			in_potion_menu = false;
			player_attack = 0;

		}
		else {
			Heal(inventory[0].item.healthRestore, inventory[get_index].item.manaRestore);
			inventory[get_index].quantity--;
			in_potion_menu = false;

		}
		return true;

	}
	else {
		system_message = "Error. There are no potions of that kind left.";
		return false;
	}
}
void Game::ShopMenu()
{
	while (true) {
		system("cls");
		std::cout << "(0)" << " Return to menu" << std::endl;
		std::cout << "Welcome to the shop (Gold: " << Game::gold << ")" << std::endl;
		for (int i = 0; i < ItemDB::GetCount(); i++) {
			std::cout << "(" << i + 1 << ") " << ItemDB::GetItem(i).name << " " << ItemDB::GetItem(i).ToString() << " " << ItemDB::GetItem(i).buy_cost << " gold" << std::endl;
		}
	
		if (system_message != "") {
			std::cout << system_message << std::endl;
		}
		int result = 0;
		if (std::cin >> result) {
			if (result == 0) {
				gameState = GameState::Menu;
				break;
			}
			if (result - 1 < ItemDB::GetCount()) {
				if (gold >ItemDB::GetItem(result - 1).buy_cost) {
					inventory[(size_t)result - 1].quantity++;
					system_message = "Successfully purchased " + ItemDB::GetItem(result - 1).name + " for " + Utility::ToString(ItemDB::GetItem(result - 1).buy_cost) + " Gold.";
					gold -= (int)ItemDB::GetItem(result - 1).buy_cost;
				}
				else {
					system_message = "Not enough gold to purchase " + ItemDB::GetItem(result - 1).name;
				}
			}
		
		}
		else {
			//This is to fix the infinite cin loop. We clear first then we ignore using a function we searched up on google haha.
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			//Tell the user is invalid response.
			system_message = "Error: Invalid response";
		}
	}
}

void Game::InventoryMenu()
{
	system("cls");
	std::cout << "Your inventory" << std::endl;
	for (int i = 0; i < inventory.size(); i++) {
		std::cout << "(" << (i + 1) << ") " << inventory[i].item.name << " x" << inventory[i].quantity << std::endl;
	}
	for (int j = 0; j < equip_list.size(); j++) {
		std::cout << "(" << (inventory.size() + 1 + j) << ") " << equip_list[j]->ToString() << std::endl;
	}
	std::cout << "Press any key to return to menu" << std::endl;
	if (system_message != "") {
		std::cout << system_message << std::endl;
		system_message = "";
	}
	std::string result = "";
	if (std::cin >> result) {
		gameState = GameState::Menu;
	}
	else {
		//This is to fix the infinite cin loop. We clear first then we ignore using a function we searched up on google haha.
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		//Tell the user is invalid response.
		system_message = "Error: Invalid response";
	}
}

void Game::EquipShopMenu()
{
	while (true) {
		system("cls");
		std::cout << "(0)" << " Return to menu" << std::endl;
		std::cout << "Welcome to the shop (Gold: " << Game::gold << ")" << std::endl;
		for (int i = 0; i <  EquipDB::GetCount(); i++) {
			std::cout << "(" << i + 1 << ") " <<  EquipDB::GetEquip(i)->ToString() << " " << EquipDB::GetEquip(i)->buy_cost << " gold" << std::endl;
		}
	
		if (system_message != "") {
			std::cout << system_message << std::endl;
		}
		int result = 0;
		if (std::cin >> result) {
			if (result == 0) {
				gameState = GameState::Menu;
				break;
			}
			if (result - 1 < EquipDB::GetCount()) {
				if (gold > EquipDB::GetEquip(result - 1)->buy_cost) {
					if (std::find_if(equip_list.begin(), equip_list.end(), [&](Equip* get_equip) {
						return get_equip->name == EquipDB::GetEquip(result - 1)->name;
					}) == equip_list.end() && 
						(!equip_gloves || equip_gloves && equip_gloves->name != EquipDB::GetEquip(result - 1)->name) &&
						(!equip_hat || equip_hat && equip_hat->name != EquipDB::GetEquip(result - 1)->name) &&
						(!equip_shirt || equip_shirt && equip_shirt->name != EquipDB::GetEquip(result - 1)->name) &&
						(!equip_pants || equip_pants && equip_pants->name != EquipDB::GetEquip(result - 1)->name) &&
						(!equip_shoes || equip_shoes && equip_shoes->name != EquipDB::GetEquip(result - 1)->name) &&
						(!equip_weapon|| equip_weapon && equip_weapon->name != EquipDB::GetEquip(result - 1)->name))
					{

						equip_list.push_back(EquipDB::GetEquip(result - 1));
						system_message = "Successfully purchased " + EquipDB::GetEquip(result - 1)->name + " for " + Utility::ToString((float)EquipDB::GetEquip(result - 1)->buy_cost) + " Gold.";
						gold -= EquipDB::GetEquip(result - 1)->buy_cost;
					}
					else {
						system_message = "Error: Equip already exist\n";
					}
				}
				else {
					system_message = "Not enough gold to purchase " + EquipDB::GetEquip(result - 1)->name;
				}
			}
	
		}
		else {
			//This is to fix the infinite cin loop. We clear first then we ignore using a function we searched up on google haha.
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			//Tell the user is invalid response.
			system_message = "Error: Invalid response";
		}
	}
}

void Game::Heal(int hp, int mp)
{

	player_message = "";
	enemy_message = "";
	health += hp;
	mana += mp;
	player_message = "You used a potion on yourself to restore ";
	if (hp > 0) {
		player_message += Utility::ToString((float)hp) + " health ";
	}
	if (mp > 0) {
		player_message += "and " + Utility::ToString((float)mp) + " mana";
	}
	player_message += "!";
	player_attack = 0;
	if (health >= MaxHealth()) {
		health = MaxHealth();
	}
	if (mana >= max_mana) {
		mana = max_mana;
	}
}

void Game::BuffEnemy()
{
	enemy_message += enemyName + " has casted a buff for 5 turns";
	player_attack = 1;
}
void Game::HealEnemy()
{
	enemy_message += enemyName + " has healed himself and restore parts of his HP";
	player_attack = 1;
}
void Game::BossReset(int index) {
	player_message = "";
	enemy_message = "";
	health = Game::MaxHealth();
	
	mana = Game::max_mana;
	boss_fight = true;
	enemy_health = EnemyData::GetBossData(index-1).health;
	enemy_max_health = EnemyData::GetBossData(index - 1).health;
	enemy_attack = EnemyData::GetBossData(index-1).attack;
	enemy_defense = EnemyData::GetBossData(index-1).defense;
	enemyName = EnemyData::GetBossData(index-1).name;
	enemy_gold = EnemyData::GetBossData(index-1).gold;
	enemy_exp = EnemyData::GetBossData(index-1).exp;
	enemy_skill_list = EnemyData::GetBossData(index-1).skill;
	enemy_dodge = 0;
	for (int i = 0; i < enemy_skill_list.size();)
		if (enemy_skill_list[i].type == SkillType::Passive_Dodge) {
			enemy_dodge = (int)enemy_skill_list[i].value;
			enemy_skill_list.erase(enemy_skill_list.begin() + i);
		}
		else {
			i++;
		}
	player_attack = 1;
	Game::defense_down_turn = 0;
	Game::poison_turn = 0;
	Game::enemy_defense_up_turn = 0;
	Game::stun_turn = 0;
	Game::blind_turn = 0;
	Game::defense_down = false;
	Game::poison = false;
	Game::enemy_defense_up = false;
	Game::stun = false;
	Game::blind = false;
	enable_input = true;
}
void Game::Reset()
{
	player_message = "";
	enemy_message = "";
	health = Game::MaxHealth();
	mana = Game::max_mana;
	boss_fight = false;
	int choice = Utility::GenerateRandomInteger(0 + 5 * current_zone,std::min( 4 + 5 * current_zone,EnemyData::GetCount()-1));
	enemy_health = EnemyData::GetData(choice).health;
	enemy_max_health = EnemyData::GetData(choice).health;
	enemy_attack = EnemyData::GetData(choice).attack;
	enemy_defense = EnemyData::GetData(choice).defense;
	enemyName = EnemyData::GetData(choice).name;
	enemy_gold = EnemyData::GetData(choice).gold;
	enemy_exp = EnemyData::GetData(choice).exp;
	enemy_skill_list = EnemyData::GetData(choice).skill;
	enemy_dodge = 0;
	is_active = true;
	for (size_t i =0 ; i < enemy_skill_list.size();)
		if (enemy_skill_list[i].type == SkillType::Passive_Dodge) {
			enemy_dodge = (int)enemy_skill_list[i].value;
			enemy_skill_list.erase(enemy_skill_list.begin() + i);
		}
		else {
			i++;
		}
	player_attack = 1;
	Game::burn = 0;
	Game::defense_down_turn = 0;
	Game::poison_turn = 0;
	Game::enemy_defense_up_turn = 0;
	Game::enemy_attack_up_turn = 0;
	Game::stun_turn = 0;
	Game::blind_turn = 0;
	Game::defense_down = false;
	Game::poison = false;
	Game::enemy_defense_up = false;
	Game::enemy_attack_up = false;
	Game::stun = false;
	Game::blind = false;
	Game::burn_turn = false;
	Game::silence_turn = 0;
	Game::silence = false;
	enable_input = true;
}
void Game::AttackEnemy()
{
	player_message = "";
	enemy_message = "";
	if (Utility::GenerateRandomInteger(1, 100) > Game::enemy_dodge && Utility::GenerateRandomInteger(1,100) > (blind ? 50: 0)) {
		int damage = Utility::GenerateRandomInteger((int)(0.5 * Attack()), Attack());
		damage = (int)(damage * Utility::ComputeDamageMult(Attack(), Game::EnemyDefense()));
		enemy_health -= damage;
		player_message += "You strike " + enemyName + " for " + std::to_string(damage) + " damage!\n";

	}
	else {
		player_message += "You strike " + enemyName + " but your attacked missed...\n";
	}
	mana += 5;
	if (mana >= max_mana) {
		mana = max_mana;
	}
	player_attack = 0;
}
void Game::FightEvents()
{
	system("cls");
	std::cout << userName << " Level: " << level << std::endl;
	std::cout << "Exp: " << Game::exp << "/" << Game::max_exp << " Gold: " << Game::gold << std::endl;
	std::cout << "Health: " << health << "/" << MaxHealth() << std::endl;
	std::cout << "Mana: " << mana << "/" << max_mana << std::endl;
	std::cout << "Attack: " << Attack() << std::endl;
	std::cout << "Defense: " << Defense() << std::endl;
	if (player_attack_up) {
		std::cout << "Attack Boost (Turn " << player_attack_turn << ")" << std::endl;
	}
	if (player_defense_up) {
		std::cout << "Defense Boost (Turn " << player_defense_turn << ")" << std::endl;
	}
	if (defense_down)
		std::cout << "Defense down (Turn " << defense_down_turn << ")" << std::endl;
	if (attack_down)
		std::cout << "Attack down (Turn " << attack_down_turn << ")" << std::endl;
	if (poison)
		std::cout << "Poison (Turn " << poison_turn << ")" << std::endl;
	if (blind)
		std::cout << "Blind (Turn " << blind_turn << ")" << std::endl;
	if (stun)
		std::cout << "Stunned (Turn " << stun_turn << ")" << std::endl;
	if (silence)
		std::cout << "Silenced (Turn " << silence_turn << ")" << std::endl;
	if (burn)
		std::cout << "Burn (Turn " << burn_turn << ")" << std::endl;
	if (water_bottle > 0)
		std::cout << "Water: (Turn " << water_bottle << ")" << std::endl;
	std::cout << "================Enemy============================" << std::endl;
	std::cout << enemyName << " Level: " << enemy_level << std::endl;
	std::cout << "Health: " << enemy_health << std::endl;
	std::cout << "Attack: " << Game::EnemyAttack() << std::endl;
	std::cout << "Defense: " << Game::EnemyDefense() << std::endl;
	if (enemy_defense_up)
		std::cout << "Defense up (Turn " << enemy_defense_up_turn << ")" << std::endl;
	std::cout << "=============Commands===========================" << std::endl;
	if (!in_potion_menu) {
		std::cout << "(1) Attack	(6) Heal" << std::endl << "(2) Power Strike	 (7) Guard" << std::endl << "(3) Use Potion" << std::endl << "(4) Rage" << std::endl << "(5) Triple Strike"<< std::endl;
		std::cout << "(Q) Idle Mode: " << idle_mode << std::endl << "(Esc) Quit" << std::endl;
		std::cout << "(<-) Previous Zone | ";
		if (current_zone + 1 < game_level)
		{
			std::cout << " (->) Next Zone" << std::endl;
		}
		else if (kill_req > 0) {
			std::cout << " [Kill " << kill_req << " more monsters to fight the boss for next zone]" << std::endl;
		}
		else {
			std::cout << "(->) Fight Area Boss" << std::endl;
		}
		std::cout << "Current Zone: " << Area[current_zone]  << (boss_fight ? " [Boss]" : "" )<< std::endl;
	}
	else {

		std::cout << "Use Potions:" << std::endl;
		for (int i = potion_page*9; i < std::min((potion_page+1) * 9,(int)inventory.size()); i++) {
			std::cout << "(" << i - potion_page*9 + 1 << ") " << inventory[i].item.name << " " << inventory[i].item.ToString() << " x" << inventory[i].quantity << std::endl;
		}
		std::cout << "(0)" << " Return to battle" << std::endl;
		if (system_message != "")
			std::cout << system_message << std::endl;
	

	}
	if (player_message != "" || enemy_message != "") {
		std::cout << "===================Message=======================" << std::endl;
		if (player_message != "")
			std::cout << player_message << std::endl;
		if (enemy_message != "")
			std::cout << enemy_message << std::endl;
	}
	if (health <= 0) {
		std::cout << "Nooo, we are defeated.... Press Enter to continue..." << std::endl;
		enable_input = false;
		player_message = "";
		enemy_message = "";


	}
	else if (enemy_health <= 0) {
		if (!(boss_fight && game_level == 6)) {
			std::cout << "Horray, we are victorious... Press Enter to continue..." << std::endl;
			std::cout << "Exp earned: " << Game::enemy_exp * EXP_RATE * (is_active ? 1.5f : 1) << " Gold earned: " << Game::enemy_gold * GOLD_RATE * (is_active ? 1.5f : 1) << std::endl;
		}
		else {
			std::cout << "Death Seeker has finally fallen!" << std::endl;
			std::cout << "Horray, we are victorious... Press.. (Oh Wait)" << std::endl;
			Sleep(10000);
			std::cout << "WAIT WHAT? THERES A STORYLINE FOR THIS FIRST GAME THIS DEVELOPER MADE? NO WAY? I CAN'T BELIEVE THIS. Well, an epilogue at least." << std::endl;
			Sleep(6000);
		}
		enable_input = false;
		player_message = "";
		enemy_message = "";
		kill_req--;
		Game::exp += (int)(Game::enemy_exp * EXP_RATE * (is_active ? 1.5 : 1));
		Game::gold += (int)(Game::enemy_gold * GOLD_RATE * (is_active ? 1.5 : 1));
		if (boss_fight) {
			

			if (game_level == 6) {
				gameState = GameState::Ending;
				std::cout << "Now that the source of evil has ben extinguished by the player, the world is at peace. But for how long?" << std::endl << std::endl;
				Sleep(12000);
				std::cout << "After the Death Seeker's Defeat, the player is now inflicted with a permanent curse for the rest of his own life" << std::endl << std::endl;
				Sleep(12000);
				std::cout << "the death seeker has dissipiated but before his final resolve, he cursed the entire world with plague, chaos, and even the unknown species which kicks in after his disapperance." << std::endl << std::endl;
				Sleep(12000);
				std::cout << "will the player ever regain his conscious and strength or will he fall to the everlasting aftermath of the curse?" << std::endl << std::endl;
				Sleep(12000);
				std::cout << "That will remain a mystery... " << std::endl;
				Sleep(12000);
				std::cout << "Congratulations, you have completed the game. Thank you very much for playing the game and hope you experience some fun RPG (idling or not) :)" << std::endl << std::endl;
				Sleep(6000);
				std::cout << "By the way, if you have unfinished business you want to finish for some reason. Press any key to continue and we'll let you carry on" << std::endl;
				int a = _getch();
				gameState = GameState::Menu;
				
				enable_input = true;
				return;
			}
			else {
				game_level++;
				std::cout << "Boss have slained, you can now advance to the next area. " << std::endl;
				kill_req = 50 + 25 * (int)std::floor(game_level / 2);
			}
		}
		//When player reaches sufficient exp, the player levels up gaining additional stats. 
		//This will also increase the max exp needed to level up.
		while (Game::exp >= Game::max_exp) {
			Game::max_health += (int)(10 + (float)(10 * level/2.5f));
			Game::max_mana += (int)(5 + (float)( 5 * level /3.f));
			Game::attack += (int)(2  + (float)(2 * level/3.f));
			Game::defense +=    (int)(1  + (float)(1 * level/3.f));
			Game::exp -= Game::max_exp;
			Game::level += 1;
			
			Game::max_exp += (int)(10 * (1.35 * (level - 1.)));
			std::cout << "LEVEL UP. you are now level: " << Game::level << std::endl;
		}

	}

	//Perform Enemy AI
	EnemyAI();

	if (!in_potion_menu) {
		while (!enable_input) {

		}

		Sleep(100);
	}
}
void Game::MainLoop()
{
	//Clear everything before starting. uWu.

	while (!exit_game) {

		if (gameState == GameState::EquipMenu) {
			int result;
			system("cls");
			std::cout << "Welcome to the character screen. Press the number key to get started" << std::endl;
			std::cout << userName << " Level: " << level << std::endl;
			std::cout << "Exp: " << Game::exp << "/" << Game::max_exp << std::endl;
			std::cout << "Health: " << MaxHealth() << "/" << MaxHealth() << std::endl;
			std::cout << "Attack: " << Attack() << std::endl;
			std::cout << "Defense: " << Defense() << std::endl;
			std::cout << "============================================================" << std::endl;
			std::cout << "(1) Helmet -" << (equip_hat ? equip_hat->ToString() : "") << std::endl;
			std::cout << "(2) Shirt -" << (equip_shirt ? equip_shirt->ToString() : "") << std::endl;
			std::cout << "(3) Pants -" << (equip_pants ? equip_pants->ToString() : "") << std::endl;
			std::cout << "(4) Gloves -" << (equip_gloves ? equip_gloves->ToString() : "") << std::endl;
			std::cout << "(5) Shoes - " << (equip_shoes ? equip_shoes->ToString(): "") << std::endl;
			std::cout << "(6) Weapon -" << (equip_weapon ? equip_weapon->ToString() : "") << std::endl;
			std::cout << "(7) To Menu" << std::endl;
			if (system_message != "") {
				std::cout << system_message << std::endl;
				system_message = "";
			}
			if (std::cin >> result) {
				std::vector<Equip*> data;
				if (result == 1) {

					std::copy_if(equip_list.begin(), equip_list.end(), std::back_inserter(data),
						[&](Equip* get) {
						//Check if this tower is within the booster tower range.
						return get->category == EquipCategory::Hat;
					});
					EquipMenu(data,EquipCategory::Hat);
				}
				if (result == 2) {

					std::copy_if(equip_list.begin(), equip_list.end(), std::back_inserter(data),
						[&](Equip* get) {
						//Check if this tower is within the booster tower range.
						return get->category == EquipCategory::Top;
					});
					EquipMenu(data,EquipCategory::Top);
				}
				if (result == 3) {

					std::copy_if(equip_list.begin(), equip_list.end(), std::back_inserter(data),
						[&](Equip* get) {
						//Check if this tower is within the booster tower range.
						return get->category == EquipCategory::Bottom;
					});
					EquipMenu(data,EquipCategory::Bottom);
				}
				if (result == 4) {

					std::copy_if(equip_list.begin(), equip_list.end(), std::back_inserter(data),
						[&](Equip* get) {
						//Check if this tower is within the booster tower range.
						return get->category == EquipCategory::Glove;
					});
					EquipMenu(data,EquipCategory::Glove);
				}
				if (result == 5) {

					std::copy_if(equip_list.begin(), equip_list.end(), std::back_inserter(data),
						[&](Equip* get) {
						//Check if this tower is within the booster tower range.
						return get->category == EquipCategory::Shoes;
					});
					EquipMenu(data,EquipCategory::Shoes);
				}
				if (result == 6) {

					std::copy_if(equip_list.begin(), equip_list.end(), std::back_inserter(data),
						[&](Equip *get) {
						//Check if this tower is within the booster tower range.
						return get->category == EquipCategory::Weapon;
					});
					EquipMenu(data,EquipCategory::Weapon);
				}
				if (result == 7) {
					gameState = GameState::Menu;
				}
				if (result > 7) {
					system_message = "Error: No such number exist";
				}
			}
			else {
				//This is to fix the infinite cin loop. We clear first then we ignore using a function we searched up on google haha.
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				//Tell the user is invalid response.
				system_message = "Error: Invalid response";
			}
		}
		else if (gameState == GameState::Menu) {
			int result;
			system_message = "";
			system("cls");
			std::cout << "Welcome to the main menu of your character. Press the number key to get started" << std::endl;
			std::cout << "(1) Inventory" << std::endl;
			std::cout << "(2) Character equips" << std::endl;
			std::cout << "(3) To Battle" << std::endl;
			std::cout << "(4) Shop" << std::endl;
			std::cout << "(5) Equip Shop" << std::endl;
			std::cout << "(6) Save" << std::endl;
			std::cout << "(7) Quit" << std::endl;
			if (system_message != "") {
				std::cout << system_message << std::endl;
				system_message = "";
			}
			if (std::cin >> result) {
				if (result == 1) {
					gameState = GameState::Inventory;
				}
				if (result == 2) {
					gameState = GameState::EquipMenu;
				}
				if (result == 3) {
					Reset();
					gameState = GameState::Fight;
					health = Game::MaxHealth();
				}
				if (result == 4) {
					gameState = GameState::Shop;		
				}
				if (result == 5) {
					gameState = GameState::EquipShop;
				}
				if (result == 6) {
					SaveGame();
				}
				if (result == 7) {
					exit_game = true;
				}
				if (result > 7) {
					system_message = "Error: No such number exist";
				}
			}
			else {
				//This is to fix the infinite cin loop. We clear first then we ignore using a function we searched up on google haha.
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				//Tell the user is invalid response.
				system_message = "Error: Invalid response";
			}
		}
		else if (gameState == GameState::Shop) {
			ShopMenu();
		}
		else if (gameState == GameState::Fight) {
			FightEvents();
		}
		else if (gameState == GameState::Inventory) {
			InventoryMenu();
		}
		else if (gameState == GameState::EquipShop) {
			EquipShopMenu();
		}
	}
}
