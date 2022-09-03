#include "Enemies.h"
std::vector<Enemy>EnemyData::enemy_data;
std::vector<Enemy>EnemyData::boss_data;

void EnemyData::AddEnemy(Enemy enemy)
{
	enemy_data.push_back(enemy);
}

Enemy EnemyData::GetData(int index)
{
	return enemy_data[index];
}

int EnemyData::GetCount()
{
	return (int)enemy_data.size();
}

void EnemyData::AddBossEnemy(Enemy enemy)
{
	boss_data.push_back(enemy);
}

Enemy EnemyData::GetBossData(int index)
{
	return boss_data[index];
}

int EnemyData::GetBossCount()
{
	return (int)boss_data.size();
}

Enemy::Enemy(std::string name, int health, int attack, int defense, int exp, int gold, std::vector<Skill> skill) : name(name),health(health),attack(attack),defense(defense),exp(exp),gold(gold),skill(skill)
{
}
