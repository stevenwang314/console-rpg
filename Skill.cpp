#include "Skill.h"
#include "Game.h"
#include "Utility.h"

Skill::Skill(std::string name, SkillType type, float value) : value(value),type(type),skill_name(name)
{
	if (type == SkillType::Passive_DoubleHit) {
		this->double_hit_chance = value;
		this->value = 1.0f;
		this->can_double_hit = true;
	}
	if (type == SkillType::Passive_Dodge) {
		this->dodge_chance = value;
	}
}

Skill::Skill(std::string name, SkillType type, float value, StatusType status_type, int turns) : Skill(name, type, value)
{
	this->status_type = status_type;
	this->turns = turns;
}

void Skill::SetMaxAttacks(int attacks)
{
	this->attacks = attacks;
}

void Skill::CastToPlayer()
{
	if (type == SkillType::Attack || type == SkillType::Passive_DoubleHit) {
		current_attacks = attacks;
		while (current_attacks >= 1) {
			Game::AttackPlayer(skill_name, value);
			if (this->can_double_hit) {
				if (Utility::GenerateRandomInteger(0, 100) < double_hit_chance)
					Game::AttackPlayer(skill_name, value * 0.5f);
			}
			current_attacks--;
		}
	}
	if (type == SkillType::Buff_Defense) {
		Game::enemy_defense_up = true;
		Game::enemy_defense_up_turn = turns;
		Game::BuffEnemy();

	}
	if (type == SkillType::Heal) {
		Game::enemy_health += (int)(Game::EnemyAttack() * value);
		if (Game::enemy_health >= Game::enemy_max_health)
			Game::enemy_health = Game::enemy_max_health;
		Game::HealEnemy();
	}
	if (type == SkillType::Buff_Attack) {
		Game::enemy_attack_up = true;
		Game::enemy_attack_up_turn = turns;
		Game::BuffEnemy();
	}
	if (status_type == StatusType::DefenseDown) {
		Game::defense_down = true;
		Game::defense_down_turn = turns;
	}
	if (status_type == StatusType::Poison) {
		Game::poison = true;
		Game::poison_turn = turns;
	}
	if (status_type == StatusType::Burn) {
		Game::burn = true;
		Game::burn_turn = turns;
		Game::burn_damage = (int)(Game::EnemyAttack() * 0.420f);
	}
	if (status_type == StatusType::AttackDown) {
		Game::attack_down = true;
		Game::attack_down_turn = turns;
	}
	if (status_type == StatusType::Stun) {
		Game::stun= true;
		Game::stun_turn = turns;
	}
	if (status_type == StatusType::Blind) {
		Game::blind = true;
		Game::blind_turn = turns;
	}
	if (status_type == StatusType::Silence) {
		Game::silence = true;
		Game::silence_turn = turns;
	}
	//Use static game class for player
}

void Skill::CastToEnemy()
{
	//Use static game class for enemies
}
