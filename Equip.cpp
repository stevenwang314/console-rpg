#include "Equip.h"

std::vector<Equip*> EquipDB::equip_list;

void EquipDB::AddEquip(Equip* equip)
{
    equip_list.push_back(equip);
}

Equip* EquipDB::GetEquip(int index)
{
    return equip_list[index];
}

int EquipDB::GetCount()
{
    return (int)equip_list.size();
}

bool EquipDB::CheckExist(Equip* equip)
{
    return std::find_if(equip_list.begin(), equip_list.end(), [&](Equip* get_equip) {
        return equip == get_equip;
    }) == equip_list.end();
}

std::string Equip::ToString()
{
    std::ostringstream ostr;
    ostr << name << (health > 0 ? " HP: " + std::to_string(health) : "") << (attack > 0 ? " Attack: " + std::to_string(attack) : "") << (defense > 0 ? " Defense: " + std::to_string(defense) : "");
    return ostr.str();
}
