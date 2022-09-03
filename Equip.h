#pragma once
#include <string>
#include <vector>
#include <sstream>
enum class EquipCategory {
	Hat, Top, Bottom, Glove, Shoes, Weapon, None
};
class Equip
{
public:
	std::string name="";
	int health=0;
	int attack=0;
	int defense=0;
	EquipCategory category;
	int buy_cost = 0;
	std::string ToString();

};

class EquipDB {
	static std::vector<Equip*> equip_list;
public:
	static void AddEquip(Equip* equip);
	static Equip* GetEquip(int index);
	static int GetCount();
	static bool CheckExist(Equip* equip);
};