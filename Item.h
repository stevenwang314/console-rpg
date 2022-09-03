#pragma once
#include <string>
#include <vector>

enum class ItemType {Recovery,Antidote,EyeDrop,Garlic,WaterBottle,Cure};

struct Item
{
public:
	std::string name="";
	std::string description="";
	
	int healthRestore=0;
	int manaRestore=0;
	float buy_cost = 0;
	ItemType type = ItemType::Recovery;
	
	std::string ToString();
};

class ItemDB {
	static std::vector<Item> item_list;
public:
	static int GetCount();
	static void AddItem(Item item);
	static Item GetItem(int index);
};
class InventoryItem {
public:
	Item item;
	int quantity = 0;
	InventoryItem() = default;
	InventoryItem(Item item);
};