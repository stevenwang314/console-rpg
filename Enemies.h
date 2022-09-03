#pragma once
#include <string>
#include <vector>
#include "Skill.h"

struct Enemy {

public:
	std::string name;
	int health;
	int attack;
	int defense;
	int exp;
	int gold;
	std::vector<Skill> skill;
	
	Enemy() = default;
	Enemy(std::string name, int health, int attack, int defense, int exp, int gold, std::vector<Skill> skill);
};
class EnemyData {
	static std::vector<Enemy> enemy_data;
	static std::vector<Enemy> boss_data;
public:
	static void AddEnemy(Enemy enemy);
	static Enemy GetData(int index);
	static int GetCount();

	static void AddBossEnemy(Enemy enemy);
	static Enemy GetBossData(int index);
	static int GetBossCount();

};