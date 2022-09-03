#include "Item.h"
#include "Utility.h"

std::vector<Item> ItemDB::item_list;

int ItemDB::GetCount()
{
    return (int)item_list.size();
}

void ItemDB::AddItem(Item item)
{
    item_list.push_back(item);
}

Item ItemDB::GetItem(int index)
{
    return item_list[index];
}

std::string Item::ToString()
{
    return Utility::Replace(Utility::Replace(description, "%hp", Utility::ToString(healthRestore)), "%mp", Utility::ToString(manaRestore));
}

InventoryItem::InventoryItem(Item item)
{
    this->item = item;
}
