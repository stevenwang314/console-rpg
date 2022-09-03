#pragma once
#include <iostream>
#include <string>
#include "Equip.h"
#include <vector>
#include "Item.h"
#include "Skill.h"

class Game
{
public:
	static std::string userName;
	static int health;
	static int max_health;
	static int mana;
	static int max_mana;
	static int attack;
	static int defense;
	static int level;
	static int exp;
	static int max_exp;
	static int gold;


	static int MaxHealth();
	static int Attack();
	static int Defense();
	static Equip* equip_hat;
	static Equip* equip_shirt;
	static Equip* equip_pants;
	static Equip* equip_gloves;
	static Equip* equip_shoes;
	static Equip* equip_weapon;
	static std::vector<InventoryItem> inventory;
	static std::string enemyName;
	static int enemy_health;
	static int enemy_max_health;
	static int enemy_attack;
	static int EnemyAttack();
	static int enemy_defense;
	static int EnemyDefense();
	static int enemy_level;
	static int enemy_exp;
	static int enemy_gold;
	static int enemy_dodge;
	static std::vector<Skill> enemy_skill_list;
	static std::vector<Equip*> equip_list;

	static bool player_attack_up;
	static int player_attack_turn;
	static bool defense_down;
	static int defense_down_turn;
	static bool attack_down;
	static int attack_down_turn;
	static bool poison;
	static int poison_turn;
	static bool enemy_defense_up;
	static int enemy_defense_up_turn;
	static bool enemy_attack_up;
	static int enemy_attack_up_turn;
	static bool blind;
	static int blind_turn;
	static bool stun;
	static int stun_turn;
	static bool silence;
	static int silence_turn;
	static bool player_defense_up;
	static int player_defense_turn;
	static bool burn;
	static int burn_turn;
	static int burn_damage;
	static int current_zone;
	static int game_level;
	static int kill_req;
	static int water_bottle;
public:
	static void Init();
	static void Input();
	static void MainLoop();
	static void EnemyAI();
	static void AttackPlayer(std::string name, float value);
	static void BuffEnemy();
	static void Reset();
	static void BossReset(int index);
	static void HealEnemy();
	static void FightEvents();
	static void EquipMenu(std::vector<Equip*> eqp, EquipCategory type);
	static void ShopMenu();
	static void EquipShopMenu();
	static void InventoryMenu();

	static void AttackEnemy();
	static void PowerStrike();
	static void AttackUp();
	static void TripleStrike();
	static void HealPlayer();
	static void Protect();

	static void Heal(int hp, int mp);

	static void SaveGame();
	static bool UsePotion(size_t get_index);
};

