#pragma once
#include <string>

enum class SkillType {Attack, Passive_Dodge, Buff_Defense, Passive_DoubleHit, Buff_Attack, Heal};
enum class StatusType {None,Poison,Stun,DefenseDown,DefenseUp,AttackDown,AttackUp,Blind,Silence,Burn};
class Skill
{
	bool can_double_hit = false;
	float double_hit_chance = 0;
	float dodge_chance = 0;
	int turns = 5;
	int current_attacks = 1;
	int attacks = 1;
public:
	float value;
	SkillType type;
	StatusType status_type = StatusType::None;
	std::string skill_name;
	Skill(std::string name, SkillType type, float value);
	Skill(std::string name,SkillType type, float value, StatusType status_type, int turns);
	void SetMaxAttacks(int attacks);
	void CastToPlayer();
	void CastToEnemy();
};

