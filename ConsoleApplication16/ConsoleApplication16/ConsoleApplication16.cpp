/****************************************************************
File: Project 4 Creative Assignment
Description: The Riddler’s Dungeon Role Playing Game

Author: Giovany Turrubiartes, Shawn Rahlwes, Stephen Randolph, Wesley Feenstra
Class:	CSCI 110
Date:	7 August 2018

I hereby certify that this program is entirely our own work.
*****************************************************************/

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <Windows.h>
#include "Header.h"


using namespace std;

const string basicChest = "basic"; // Basic Chest
const string regularChest = "regular"; // Regular Chest 
const string grandChest = "grand"; // Grand Chest
const string edgyChest = "edgy"; // Edgy Chest
string playerAction;
vector <int> Potions (4);
Items items;
Entities entities;
int MAXHealth, MAXAttack, MAXDefense, NUM = -1,swarmDamage = 0;
bool Boss, Swarm, gameOver;

Entities::Player player, emptyPlayer;
Items::Armor playerArmor, newArmor;
Items::Weapon playerWeapon, newWeapon;
Entities::Enemy enemy[100];




//Whenever there's a batle sequence, this function will be called
void Battle() {
	entities.retrieveStats(player);
	if (Boss)
		entities.is_boss(enemy[NUM + 1]);
	if (Swarm)
		entities.is_swarm(enemy[NUM + 1]);
	//Here, the program loads and saves the stats of the player and applies stat boosts from gear
	entities.createEnemy(enemy[++NUM], player.dungeon);
	MAXHealth = player.health + playerArmor.healthBoost;
	MAXDefense = player.defense + playerArmor.defenseBoost;
	MAXAttack = player.attack + playerWeapon.attackBoost;
	player.attack = MAXAttack;
	player.defense = MAXDefense;
	player.health = MAXHealth;
	//The damage dealt by swarm enemies are applied here
	if (Swarm)
		player.health -= swarmDamage;
	cout << "A " << enemy[NUM].type << " appears!" << endl;
	Sleep(1000);
	entities.battlePrint(player, enemy[NUM]);
	do {
		cout << "Your health: " << player.health << "\t\t Enemy health: " << enemy[NUM].health << endl;
		Sleep(1000);
		//The program randomly chooses who attacks; it is NOT turn based (like real life)
		if (rand() % 2 == 0) {
			cout << "You see a chance to make a move." << endl;
			Sleep(750);
			do {
				cout << "What will you do (Strike or Potion)? ";
				cin >> playerAction;
				playerAction.at(0) = toupper(playerAction.at(0));
				//If the player chooses to attack, the program statistically decides if the strike lands and damage is dealt
				if (playerAction == "Strike") {
					if (rand() % 1000 + 1 > playerWeapon.accuracy * 1000) {
						cout << "Your strike misses!" << endl;
					}
					else
						entities.playerStrike(player, enemy[NUM]);
				}
				//If the user uses a potion, the program checks if it's possible; if so, the potion effect is applied
				else if (playerAction == "Potion") {
					if (Potions.at(0) == 0 && Potions.at(1) == 0 && Potions.at(2) == 0) {
						cout << "You don't have any usable potions." << endl;
						Sleep(250);
						playerAction.clear();
					}
					else if (player.health == MAXHealth) {
						cout << "You're already at full health. " << endl;
						Sleep(750);
						playerAction.clear();
					}
					//Displays the potions
					else {
						items.printPotions(Potions);
						cout << "What size potion would you like to use (S, M, or L)? ";
						playerAction.clear();
						cin >> playerAction;
						playerAction.at(0) = toupper(playerAction.at(0));
						//uses potion according to user's input
						if (playerAction == "L"&&Potions.at(2) != 0) {
							entities.usePotion(player, MAXHealth, MAXHealth * 3 / 4);
							cout << "You used a Large Potion. Your health is now " << player.health << endl;
						}
						else if (playerAction == "M"&&Potions.at(1) != 0) {
							entities.usePotion(player, MAXHealth, MAXHealth / 2);
							cout << "You used a Medium Potion. Your health is now " << player.health << endl;
						}
						else if (playerAction == "S"&&Potions.at(0) != 0) {
							entities.usePotion(player, MAXHealth, 20);
							cout << "You used a Small Potion. Your health is now " << player.health << endl;
						}
						else {
							cout << "Invalid entry." << endl;
							Sleep(250);
							playerAction.clear();
						}
					}
				}
				else {
					cout << "Invalid entry." << endl;
					Sleep(250);
					playerAction.clear();
				}
			} while (playerAction.empty());
			cout << endl;
		}
		//If the program chooses to enemy to attack first, the move is made here.
		else {
			cout << "The " << enemy[NUM].type << " strikes!" << endl;
			Sleep(750);
			//The accuracy of the enemy is determined by its class; the program decides statistically if the attack lands
			if (enemy[NUM].classType == "Warrior") {
				if (rand() % 1000 + 1 < 900) {
					entities.enemyStrike(player, enemy[NUM]);
				}
				else
					cout << "The enemy's strike misses!" << endl;
			}
			else if (enemy[NUM].classType == "Juggernaut") {
				if (rand() % 1000 + 1 < 800) {
					entities.enemyStrike(player, enemy[NUM]);
				}
				else
					cout << "The enemy's strike misses!" << endl;
			}
			else if (enemy[NUM].classType == "Knight") {
				if (rand() % 1000 + 1 < 700) {
					entities.enemyStrike(player, enemy[NUM]);
				}
				else
					cout << "The enemy's strike misses!" << endl;
			}
			else if (enemy[NUM].classType == "Warlock") {
				if (rand() % 1000 + 1 < 850) {
					entities.enemyStrike(player, enemy[NUM]);
				}
				else
					cout << "The enemy's strike misses!" << endl;
			}
			else {
				//for fun
				cout << "The enemy slime dances around in a way that makes you want to kill it more..." << endl;
			}
			cout << endl;
			if (Swarm)
				swarmDamage = MAXHealth - player.health;
		}
		//If the player dies and has revives, they can choose to use one here
		if (player.health == 0 && Potions.at(3) != 0) {
			cout << "You are on the verge of death. ";
			Sleep(1000);
			do {
				cout << "Would you like to use a revive? ";
				cin >> playerAction;
				playerAction.at(0) = toupper(playerAction.at(0));
				if (playerAction == "Yes") {
					player.health = MAXHealth;
					cout << "You use a revive. You are now at full health." << endl;
				}
				else if (playerAction == "No") {
					break;
				}
				else {
					cout << "Invalid entry." << endl;
					playerAction.clear();
					Sleep(250);
				}
			} while (playerAction.empty());
		}
		Sleep(1000);
	} while (player.health != 0 && enemy[NUM].health != 0);
	
	//death message giving the player a choice to try again
	if (player.health == 0) {
		entities.retrieveStats(player);
		cout << "You've died. All items gathered in this dungeon have been lost." << endl;
		gameOver = true;
	}
	else if (!gameOver) {
		entities.retrieveStats(player);
		cout << "You killed the " << enemy[NUM].type << "!" << endl;
		Sleep(1000);
		cout << "You gain ";
		if (Boss) {
			cout << enemy[NUM].level * 19;
			player.XP += enemy[NUM].level * 19;
			player.dungeon++;
		}
		else {
			cout << enemy[NUM].level * 9;
			player.XP += enemy[NUM].level * 9;
		}
		cout << " XP!" << endl << endl;
		Sleep(1000);
		if (player.XP >= player.level*player.level * 10)
			entities.levelUp(player);
	}
	entities.saveStats(player);
	Sleep(1000);
}

//When a player receives a chest, this function is called
void Items::chest(string chestType) // Int p is the global variable for chest type
{
	int randNum;
	int chestItems = 0;
	//basic chest has 1 item
	if (chestType == "basic") {
		cout << "You find one item in the chest." << endl<<endl;
		Sleep(1000);
		createPotion(Potions, chestLootLevel());
	}
	//regular chest has 2 items
	else if (chestType == "regular")
	{
		cout << "You find two items in the chest." << endl<<endl;
		Sleep(1000);
		for (int i = 0; i < 2; i++)
		{
			createPotion(Potions, chestLootLevel());
			Sleep(750);
		}
	}
	//grand chest can give gear or potions; 3 items
	else if (chestType == "grand")
	{
		cout << "You find three items in the chest." << endl<<endl;
		Sleep(1000);
		for (int i = 0; i < 3; i++)
		{
			randNum = rand() % 3;
			if (randNum == 0) {
				cout << "You've received a new set of armor!" << endl;
				Sleep(1000);
				items.createArmor(newArmor,player.dungeon, chestLootLevel());
				cout << "New armor:" << endl;
				items.printArmor(newArmor);
				cout << endl;
				Sleep(1000);
				cout << "Current armor:" << endl;
				items.printArmor(playerArmor);
				cout << endl;
				Sleep(1000);
				do{
					cout << "Would you like to switch armors?" << endl;
					cin >> playerAction;
					playerAction.at(0) = toupper(playerAction.at(0));
					if (playerAction == "Yes") {
						playerArmor = newArmor;
					}
					else if (playerAction == "No") {
						break;
					}
					else {
						cout << "Invalid entry." << endl;
						Sleep(250);
						playerAction.clear();
					}
				} while (playerAction.empty());
			}
			else if (randNum == 1) {
				cout << "You've received a new weapon!" << endl;
				Sleep(1000);
				items.createWeapon(newWeapon, player.dungeon, chestLootLevel());
				cout << "New weapon:" << endl;
				items.printWeapon(newWeapon);
				cout << endl;
				Sleep(1000);
				cout << "Current weapon: " << endl;
				items.printWeapon(playerWeapon);
				cout << endl;
				Sleep(1000);
				do{
					cout << "Would you like to switch weapons?" << endl;
					cin >> playerAction;
					playerAction.at(0) = toupper(playerAction.at(0));
					if (playerAction == "Yes") {
						playerWeapon = newWeapon;
					}
					else if (playerAction == "No") {
						break;
					}
					else {
						cout << "Invalid entry." << endl;
						Sleep(250);
						playerAction.clear();
					}
				} while (playerAction.empty());
			}
			else {
				createPotion(Potions, chestLootLevel());
			}
			Sleep(1000);
		}
	}

	//edgy chest gan give gear or potions, nothing, or could trip a battle sequence
	else if (chestType == "edgy")
	{
		int loot = 0;
		loot = rand() % 3;

		if (loot == 0) // Regular Chest 
		{
			cout << "You find two items in the chest." << endl<<endl;
			Sleep(1000);
			for (int i = 0; i < 2; i++)
			{
				randNum = rand() % 3;
				if (randNum == 0) { //gives new armor
					cout << "You've received a new set of armor!" << endl;
					Sleep(1000);
					items.createArmor(newArmor, player.dungeon, chestLootLevel());
					cout << "New armor:" << endl;
					items.printArmor(newArmor);
					cout << endl;
					Sleep(1000);
					cout << "Current armor:" << endl;
					items.printArmor(playerArmor);
					Sleep(1000);
					cout << endl;
					do {
						cout << "Would you like to switch armors?" << endl;
						cin >> playerAction;
						playerAction.at(0) = toupper(playerAction.at(0));
						if (playerAction == "Yes") {
							playerArmor = newArmor;
						}
						else if (playerAction == "No") {
							break;
						}
						else {
							cout << "Invalid entry." << endl;
							Sleep(250);
							playerAction.clear();
						}
					} while (playerAction.empty());
				}
				else if (randNum == 1) { //gives a new weapon
					cout << "You've received a new weapon!" << endl;
					Sleep(1000);
					items.createWeapon(newWeapon, player.dungeon, chestLootLevel());
					cout << "New weapon:" << endl;
					items.printWeapon(newWeapon);
					cout << endl;
					Sleep(1000);
					cout << "Current weapon: " << endl;
					items.printWeapon(playerWeapon);
					cout << endl;
					Sleep(1000);
					do {
						cout << "Would you like to switch weapons?" << endl;
						cin >> playerAction;
						playerAction.at(0) = toupper(playerAction.at(0));
						if (playerAction == "Yes") {
							playerWeapon = newWeapon;
						}
						else if (playerAction == "No") {
							break;
						}
						else {
							cout << "Invalid entry." << endl;
							Sleep(250);
							playerAction.clear();
						}
					} while (playerAction.empty());
				}
				else { //gives a potion
					createPotion(Potions, chestLootLevel());
				}
				Sleep(1000);
			}
		}

		else if (loot == 1) //trap chest that leads to a battle
		{
			cout << "The chest was a trap!" << endl;
			Sleep(1000);
			Battle();
			
		}

		else if (loot == 2) //Empty Chest
		{
			cout << "The chest is empty, it seems like someone has already been here...\n";
			Sleep(1000);
			cout << "Maybe another chest will lie ahead of us in this journey.\n";
			Sleep(1000);
		}
	}

	return;
}

//Easy mode
void path1()
{
	do {
		string route = "";
		cout << "As you walk down the path, you run into another adventurer who seems to be too happy for his own good." << endl;
		Sleep(1000);
		cout << "After a friendly conversation, the stranger gives you a small chest." << endl;
		Sleep(1000);
		items.chest(basicChest);
		Sleep(1000);
		cout << "Suddenly you're attacked!" << endl;
		Sleep(1000);
		Battle();
		if (gameOver)
			break;
		cout << "After winning the battle, you continue your trek." << endl;
		Sleep(1000);
		cout << "There is a fork ahead. It looks dark and dangerous, who knows what lies ahead...\n";
		Sleep(1000);
		do {
			cout << "Will you go through it, young blood?\n";
			route.clear();
			cin >> route;
			route.at(0) = tolower(route.at(0));
			if (route == "yes")
			{
				cout << "You come upon a group of enemies!" << endl;
				Sleep(1000);
				for (int i = 0; i < 3; i++) {
					Swarm = true;
					Battle();
					if (gameOver)
						break;
				}
				Swarm = false;
				swarmDamage = 0;
				if (gameOver)
					break;
				entities.saveStats(player);
				cout << "You managed to survive the swarm." << endl;
				Sleep(1000);
				cout << "You find a small chest amongst the corpses." << endl;
				Sleep(1000);
				items.chest(regularChest);
			}

			else if (route == "no")
			{
				cout << "The fork looks to be too dangerous, maybe that edgy choice paid off!\n";
				Sleep(1000);
				cout << "The end is near but, what lies ahead...?\n";
				Sleep(1000);
			}
			else {
				cout << "That's not a choice." << endl;
				Sleep(250);
				route.clear();
			}
		} while (route.empty());
		if (gameOver)
			break;
		cout << "There seems to be something blocking the path to the end!\n";
		Sleep(1000);
		cout << "It's an enemy blocking your path! It seems stronger than the rest...\n";
		Sleep(1000);
		cout << "But it must be defeated before you can move forward.\n";
		Sleep(1000);

		Boss = true;
		Battle(); //boss fight
		if (gameOver)
			break;
		Sleep(1000);
		cout << "WOW! You've defeated the boss! That's pretty good.\n";
		Sleep(1000);
		cout << "You receive a large chest for killing the " << enemy[NUM].type << "." << endl;
		Sleep(1000);
		items.chest(grandChest);
		Sleep(1000);
		items.saveArmor(playerArmor);
		items.saveWeapon(playerWeapon);
		items.savePotions(Potions);
		cout << "The next path awaits you...\n";
		Sleep(1000);
		gameOver = true;
	} while (!gameOver);
}

//Medium mode
void path2()
{
	do {
		
		string route = "";
		cout << "You find a basic Chest on the side of the road!" << endl;
		Sleep(1000);
		items.chest(basicChest);
		Sleep(1000);
		cout << "It was a trap!" << endl;
		Sleep(1000);
		Battle();
		if (gameOver)
			break;

		cout << "With the small amount of treasure you've gained, you warily press on..." << endl;
		Sleep(1000);
		cout << "The air begins to feel more humid and mold surrounds the path as you walk further..." << endl;
		Sleep(1000);
		cout << "You see something hidden in the tall grass..." << endl;
		Sleep(1000);
		cout << "It's an old looking chest of some sort...\n";
		Sleep(1000);
		cout << "There might still be something inside!\n";
		Sleep(1000);
		do {
			cout << "Would you like to open the chest?\n";
			route.clear();
			cin >> route;
			route.at(0) = tolower(route.at(0));
			if (route == "yes")
			{
				cout << "You decide to take a look inside." << endl;
				Sleep(1000);
				items.chest(edgyChest);
				if (gameOver)
					break;
			}

			else if (route == "no")
			{
				cout << "You ignore the chest, even though you could be missing out on something...\n";
				Sleep(1000);
				cout << "You'll never know...\n";
				Sleep(1000);
			}
			else {
				cout << "That's not a choice." << endl;
				route.clear();
				Sleep(250);
			}
		} while (route.empty());
		if (gameOver)
			break;
		cout << "Something up ahead is blocking your path. You slowly approach to see what it is...\n";
		Sleep(1000);
		cout << "Wait! It looks to be an enemy!\n";
		Sleep(1000);

		Battle();
		if (gameOver)
			break;
		Sleep(1000);
		cout << "You made too much noise during the fight and attracted attention...\n";
		Sleep(1000);

		Battle();
		if (gameOver)
			break;

		cout << "There is a break in the path." << endl;
		Sleep(1000);
		do {
			cout << "Would like to go through the fork?\n";
			route.clear();
			cin >> route;
			route.at(0) = tolower(route.at(0));

			if (route == "yes")
			{
				cout << "There's a swarm of enemies down the fork!" << endl;
				Sleep(1000);
				for (int i = 0; i < 2; i++) {
					Swarm = true;
					Battle();
					if (gameOver)
						break;
				}
				Swarm = false;
				swarmDamage = 0;
				if (gameOver)
					break;
				entities.saveStats(player);
				Sleep(1000);
			}

			else if (route == "no")
			{
				cout << "As you move through the path, you notice something moving in the brush...\n";
				Sleep(1000);
				cout << "Another enemy covered in blood of his fellow comrades spots you.\n";
				Sleep(1000);
				cout << "He runs out and attacks you!\n";
				Sleep(1000);
				Battle();
				if (gameOver)
					break;
				cout << "You find a chest in the remains of the crazed enemy. He must've stolen it and tried to espace." << endl;
				Sleep(1000);
				items.chest(regularChest);
				Sleep(1000);
			}
			else {
				cout << "That's not a choice." << endl;
				route.clear();
				Sleep(250);
			}
		} while (route.empty());
		if (gameOver)
			break;

		cout << "Something rises from the ground and blocks your path ahead!\n";
		Sleep(1000);
		cout << "It appears to be stronger than the rest...\n";
		Sleep(1000);

		Boss = true;
		Battle(); //boss fight
		if (gameOver)
			break;
		Sleep(1000);
		cout << "You receive a large chest for killing the " << enemy[NUM].type << "." << endl;
		Sleep(1000);
		items.chest(grandChest);
		Sleep(1000);
		items.saveArmor(playerArmor);
		items.saveWeapon(playerWeapon);
		items.savePotions(Potions);

		cout << "The next path awaits you...\n";
		Sleep(1000);
		gameOver = true;
	}while (!gameOver);
}

//Hard mode
void path3()
{
	do {
		string route = "";
		string riddle = "";
		string hangman = "";
		do {
			cout << "There's a chest ahead, do you want to open it?\n";
			route.clear();
			cin >> route;
			route.at(0) = tolower(route.at(0));

			if (route == "no")
			{
				cout << "Looks like you missed out on the valuable loot, only true champions open the chest";
				Sleep(500);
				cout << ".";
				Sleep(500);
				cout << ".";
				Sleep(500);
				cout << ".";
				Sleep(1000);
				cout << "\n";
				cout << "Onward!\n";
				Sleep(1000);
			}

			else if (route == "yes")
			{
				cout << "Looks like someone has already been here.\n";
				Sleep(1000);
				cout << "You hear a voice echo all around you." << endl;
				Sleep(1000);
				cout << "\"Young blood ya gotta be quick AF, ya feel me dog!\"\n";
				Sleep(1000);
				cout << "That was weird. Hopefully you don't have to deal with that the entire time." << endl;
				Sleep(2000);
			}
			else {
				cout << "That's not a choice." << endl;
				route.clear();
				Sleep(250);
			}
		} while (route.empty());

		// Riddle time

		cout << "Wait it looks to be someone in front of your path...\n";
		Sleep(1000);
		cout << "It's someone in a green suit!\n";
		Sleep(1000);
		cout << "WAIT! It's the Riddler coming to riddle you!\n";
		Sleep(1000);
		cout << "\"Riddle me THIS, What do you call two witches that live together?\"\n";
		riddle.clear();
		cin >> riddle;

		if (riddle == "THIS")
		{
			cout << "\"Looks like you've riddled the riddler; the riddler doesn't like how that riddles!\"\n";
			Sleep(1000);
			cout << "The riddler summons his riddles to riddle you!\n";
			Sleep(1000);
			cout << "A couple enemies approach." << endl;
			Sleep(1000);
			for (int i = 0; i < 2; i++) {
				Swarm = true;
				Battle();
				if (gameOver)
					break;
			}
			Swarm = false;
			swarmDamage = 0;
			if (gameOver)
				break;
			entities.saveStats(player);
			Sleep(1000);
			cout << "The corpses of the fallen enemies fade away and a small chest is left in their place." << endl;
			Sleep(1000);
			items.chest(basicChest);
		}

		else
		{
			cout << "You've failed to impress the riddler with his riddles, now he's going to riddle with your riddles!\n";
			Sleep(1000);
			for (int i = 0; i < 4; i++) {
				Swarm = true;
				Battle();
				if (gameOver)
					break;
			}
			Swarm = false;
			swarmDamage = 0;
			if (gameOver)
				break;
			entities.saveStats(player);
		}
		if (gameOver)
			break;
		cout << "You kill the final enemy and continue down the path." << endl;
		Sleep(1000);

		// Edgy Chest Riddle Edition
		cout << "As you walk, you notice a chest hidden in the brush. It could have been from the riddlers gang.\n";
		Sleep(1000);
		do {
			cout << "Would you like open the chest?\n";
			route.clear();
			cin >> route;
			route.at(0) = tolower(route.at(0));

			if (route == "yes")
			{
				cout << "There lies a piece of paper in the chest with some writing on it...\n";
				Sleep(1000);
				cout << "On the back of the paper it reads...\n";
				Sleep(1000);
				cout << "\"What did the hangman give his wife for her birthday?\"\n";
				riddle.clear();
				cin >> riddle;
				riddle.at(0) = toupper(riddle.at(0));

				if (riddle == "Choker")
				{
					cout << "The riddler falls out of a nearby tree and looks at you in awe of your extensive knowledge of this field.\n";
					Sleep(1000);
					cout << "The riddler says nothing and gives you a chest for your correct answer." << endl;
					Sleep(1000);
					cout << "He walks away slowly with his head held down.\n";
					Sleep(1000);

					items.chest(basicChest);
					Sleep(1000);
				}

				else
				{
					cout << "You've been riddled by the riddler with his riddles, riddler summons his loyal riddlers to attack you.\n";
					Sleep(1000);
					Battle();
					if (gameOver)
						break;
				}
			}

			else if (route == "no")
			{
				cout << "I think the riddler was trying to riddle you with his riddles, maybe his riddles will riddle you riddleless later...\n";
				Sleep(1000);
				cout << "Forward on our path of riddles!\n";
				Sleep(1000);
			}
			else {
				cout << "That's not a choice." << endl;
				route.clear();
				Sleep(250);
			}
		} while (route.empty());
		if (gameOver)
			break;
		cout << "Look, there's a small chest ahead!\n";
		Sleep(1000);
		items.chest(basicChest);
		Sleep(1000);

		// Hidden Enemy (Cave)

		cout << "A storm is fast approaching. You need to take shelter quick." << endl;
		Sleep(1000);
		cout << "You find a cave ahead...\n";
		Sleep(1000);
		cout << "It looks dark and gloomy but will provide shelter from the storm!\n";
		Sleep(1000);
		cout << "Or you can avoid it and go through the storm. The terrian ahead looks hilly with a spooky skelly forest!\n";
		Sleep(1000);
		cout << "What will your route be?????????\n";
		Sleep(1000);
		do {
			cout << "Will you go through the cave?" << endl;
			route.clear();
			cin >> route;
			route.at(0) = tolower(route.at(0));
			//cave route
			if (route == "yes")
			{
				cout << "You enter the cave. It's dark and cold...\n";
				Sleep(1000);
				cout << "As your walk through the dark you hear something further in...\n";
				Sleep(1000);
				cout << "You find a torch to guide you through the cave." << endl;
				Sleep(1000);
				cout << "Do you want to light it and risk being seen ? \n";
				Sleep(1000);
				do {
					cout << "Will you light the torch?\n";
					hangman.clear();
					cin >> hangman;
					hangman.at(0) = tolower(hangman.at(0));
					//no hangman encounter
					if (hangman == "yes")
					{
						cout << "The light from the torch has provided a path through the cave...\n";
						Sleep(1000);
						cout << "You see the the end of the cave and something moving behind you...\n";
						Sleep(1000);
						cout << "You start running away from the sounds and make it out of the cave!\n";
						Sleep(1000);
					}
					//hangman encounter
					else if (hangman == "no")
					{
						cout << "You drop the torch and keep walking in the dark...\n";
						Sleep(1000);
						cout << "You hear something getting closer and closer to you...\n";
						Sleep(1000);
						cout << "A giggle is heard nearby...\n";
						Sleep(1000);
						cout << "You're terrified. Your natural instincts tell you to run, but you feel like standing still is safer.\n";
						Sleep(1000);
						do {
							cout << "Will you run?" << endl;
							hangman.clear();
							cin >> hangman;
							hangman.at(0) = tolower(hangman.at(0));
							//no hangman encounter
							if (hangman == "yes")
							{
								cout << "You feel something try to grab your shoulder from behind.\n";
								Sleep(1000);
								cout << " You manage to run away fast enough to break lose from its grip...\n";
								Sleep(1000);
								cout << "You see the light at the end of the cave and make it out to live and tell the tale...\n";
								Sleep(1000);
							}
							//hangman encounter
							else if (hangman == "no")
							{
								cout << "You stand still and feel something go up your back...\n";
								Sleep(1000);
								cout << "You continue to stand still, frozen with fear.\n";
								Sleep(1000);
								cout << "You hear a rope sliding on the ground and then wrap around your neck...\n";
								Sleep(1000);
								cout << "You find out too late it's the hangman with his choker looking for his wife...\n";
								Sleep(1000);
								cout << "Mrs. Hangman left Mr. Hangman for the Riddler and Mr. Hangman is feeling lonely...\n";
								Sleep(1000);
								cout << "You manage to use your weapon to cut the rope and face one of Mr. Hangman's henchmen...\n";
								Sleep(1000);
								Battle();
								if (gameOver)
									break;
								Sleep(1000);
								cout << "You finally make it out of the cave." << endl;
								Sleep(1000);
								cout << "As you walk away, you hear the cries of the hangman. " << endl;
								Sleep(1000);
								cout << "First his wife leaves him, then you break into his house and kill his only friend." << endl;
								Sleep(1000);
								cout << "You're the real monster here." << endl;
								Sleep(1000);
							}
							else {
								cout << "That's not a choice." << endl;
								hangman.clear();
								Sleep(250);
							}
							if (gameOver)
								break;
						} while (hangman.empty());
					}
					else {
						cout << "That's not a choice." << endl;
						hangman.clear();
						Sleep(250);
					}
				} while (hangman.empty());
				if (gameOver)
					break;
				//Another riddle
				cout << "The riddler appears and you must answer his riddles to move on...\n";
				Sleep(1000);
				cout << "\"Riddle me this, what is the meaning of life?\"\n";
				riddle.clear();
				cin >> riddle;

				if (riddle == "42")
				{
					cout << "\"That's correct but, you've been riddled!\"\n";
					Sleep(1000);
					cout << "The riddler realizes he has been living his life the wrong way and cannot live with himself...\n";
					Sleep(1000);
					cout << "The riddler walks away slowly in the sunset and off a cliff into the spooky skelly forest.\n";
					Sleep(1000);
					cout << "His fan club attacks you." << endl;
					Sleep(1000);
					for (int i = 0; i < 4; i++) {
						Swarm = true;
						Battle();
						if (gameOver)
							break;
					}
					Swarm = false;
					swarmDamage = 0;
					if (gameOver)
						break;
					entities.saveStats(player);
					cout << "You manage to fight off the weeaboos and continue through the forest." << endl;
					Sleep(1000);
				}

				else
				{
					cout << "You've failed to impress the riddler with his riddles, time to get absolutely friggin RIDDLED!\n";
					Sleep(1000);
					for (int j = 0; j < 2; j++) {
						for (int i = 0; i < 3; i++) {
							Swarm = true;
							Battle();
							if (gameOver)
								break;
						}
						Swarm = false;
						swarmDamage = 0;
						if (gameOver)
							break;
						entities.saveStats(player);
						cout << "You get a chance to catch your breath before the next group of enemies comes." << endl;
						Sleep(1000);
					}
				}
				if (gameOver)
					break;
			}
			//no cave route
			else if (route == "no")
			{
				cout << "The storm begins to rain and you run into the spooky skelly forest to take shelter...\n";
				Sleep(1000);
				cout << "You see a group of unknown creatures ahead of you...\n";
				Sleep(1000);
				cout << "It's an enemy wave looking to kill you...\n";
				Sleep(1000);
				for (int i = 0; i < 4; i++) {
					Swarm = true;
					Battle();
					if (gameOver)
						break;
				}
				Swarm = false;
				swarmDamage = 0;
				if (gameOver)
					break;
				entities.saveStats(player);
				cout << "Wow you managed to survive the enemy wave and now you can leave the forest...\n";
				Sleep(1000);
				cout << "But there is someone in your path preventing you from making it to the end...\n";
				Sleep(1000);
				Battle();
				if (gameOver)
					break;
			}
			else {
				cout << "That's not a choice." << endl;
				route.clear();
				Sleep(250);
			}
		} while (route.empty());
		if (gameOver)
			break;
		Boss = true; //boss fight
		Battle();
		if (gameOver)
			break;
		Sleep(1000);
		cout << "You receive a large chest for killing the " << enemy[NUM].type << "." << endl;
		Sleep(1000);
		items.chest(grandChest);
		Sleep(1000);
		items.saveArmor(playerArmor);
		items.saveWeapon(playerWeapon);
		items.savePotions(Potions);
		cout << "The next path awaits you...\n";
		Sleep(1000);
		gameOver = true;
	} while (!gameOver);
}

int main()
{
	cout << "|||||  |||| |||    |||     ||      ||||||  |||||   ||  ||||        |||    ||     || |||    ||   ||||||||  ||||||  ||||||  |||    ||              " << endl;
	cout << "||  ||  ||  || ||  || ||   ||      ||      ||  ||     ||           || ||  ||     || ||||   ||  ||         ||     ||    || ||||   ||          " << endl;
	cout << "|||||   ||  ||  || ||  ||  ||      ||      |||||      ||           ||  || ||     || || ||  || |||         ||     ||    || || ||  ||          " << endl;
	cout << "||      ||  ||  || ||  ||  ||      ||||||  ||          |||         ||  || ||     || ||  || || |||  |||||  |||||| ||    || ||  || ||            " << endl;
	cout << "||||    ||  ||  || ||  ||  ||      ||      ||||          ||        ||  || ||     || ||   |||| |||      || ||     ||    || ||   ||||           " << endl;
	cout << "|| ||   ||  || ||  || ||   ||      ||      || ||         ||        || ||  ||     || ||    |||  ||      || ||     ||    || ||    |||           " << endl;
	cout << "||  || |||| |||    |||     ||||||  ||||||  ||  ||     ||||         |||      |||||   ||     ||   ||||||||  ||||||  ||||||  ||     ||                " << endl;
	cout << endl;
	Sleep(1000);
	srand(time(0));
	entities.retrieveStats(player);
	//continue adventure
	if (player.level != 0) {
		entities.printStats(player);
		Sleep(1000);
		do {
			cout << "Would you like to continue? ";
			playerAction.clear();
			cin >> playerAction;
			playerAction.at(0) = toupper(playerAction.at(0));
			if (playerAction == "Yes") {
				gameOver = false;
				break;
			}
			//Delete save data
			else if (playerAction == "No") {
				do {
					cout << "You're about to start a new game. ALL PREVIOUS SAVED DATA WILL BE LOST. Are you sure? ";
					playerAction.clear();
					cin >> playerAction;
					playerAction.at(0) = toupper(playerAction.at(0));
					if (playerAction == "Yes") {
						entities.saveStats(emptyPlayer);
						items.createArmor(playerArmor, 0, "Common");
						items.saveArmor(playerArmor);
						items.createWeapon(playerWeapon, 0, "Common");
						items.saveWeapon(playerWeapon);
						for (int i = 1; i < 4; i++) 
							Potions.at(i) = 0;
						Potions.at(0) = 1;
						items.savePotions(Potions);
						items.retrievePotions(Potions);
						entities.retrieveStats(player);
						items.retrieveArmor(playerArmor);
						items.retrieveWeapon(playerWeapon);
					}
					else if (playerAction == "No") {
						playerAction.clear();
						break;
					}
					else {
						cout << "Invalid entry." << endl;
						playerAction.clear();
					}
				} while (playerAction.empty());
			}
			else {
				cout << "Invalid entry." << endl;
				playerAction.clear();
			}
		} while (playerAction.empty());
		playerAction.clear();
	}

	//new game created here; class choice and new character save
	if (player.level == 0) {
		cout << " Class  | " << "Juggernaut |" << setw(9) << "Warrior" << setw(4) << "|" << setw(9) << "Knight" << setw(4) << "|" << endl;
		cout << " Health |" << setw(7) << "28" << setw(6) << "|" << setw(7) << "22" << setw(6) << "|" << setw(7) << "22" << setw(6) << "|" << endl;
		cout << " Attack |" << setw(7) << "11" << setw(6) << "|" << setw(7) << "14" << setw(6) << "|" << setw(7) << "11" << setw(6) << "|" << endl;
		cout << " Defense|" << setw(7) << "11" << setw(6) << "|" << setw(7) << "11" << setw(6) << "|" << setw(7) << "14" << setw(6) << "|" << endl;
		do {
			cout << "Which class would you like? ";
			playerAction.clear();
			cin >> playerAction;
			playerAction.at(0) = toupper(playerAction.at(0));
			if (playerAction == "Warrior")
				entities.saveStats(entities.newPlayer ("Warrior",22,14,12));
			else if (playerAction == "Juggernaut")
				entities.saveStats(entities.newPlayer("Juggernaut",28,11,11));
			else if (playerAction == "Knight")
				entities.saveStats(entities.newPlayer("Knight",22,11,14));
			else {
				cout << "Invalid choice." << endl;
				playerAction.clear();
			}
		} while (playerAction.empty());
		do {
			cout << "Are you ready to enter your first dungeon? ";
			playerAction.clear();
			cin >> playerAction;
			playerAction.at(0) = toupper(playerAction.at(0));
			if (playerAction == "Yes")
				gameOver = false;
			else if (playerAction == "No") {
				cout << "Come back when you're ready." << endl;
				gameOver = true;
			}
			else {
				cout << "Invalid entry." << endl;
				playerAction.clear();
			}
		} while (playerAction.empty());
	}

	//The game is played here
	while (!gameOver) {
		Boss = false;
		Swarm = false;
		NUM = -1;
		/*
		//delete this and the next line for the real game; only for testing
		items.createWeapon(playerWeapon, 1000000, items.chestLootLevel()); 
		items.saveWeapon(playerWeapon);
		*/
		entities.retrieveStats(player);
		items.retrieveArmor(playerArmor);
		items.retrievePotions(Potions);
		items.retrieveWeapon(playerWeapon);

		cout << "Three paths lie before you." << endl;
		Sleep(750);
		//Dungeon difficulty choice
		do {
			cout << "Which will you follow (Easy, Medium, or Hard)? ";
			playerAction.clear();
			cin >> playerAction;
			playerAction.at(0) = toupper(playerAction.at(0));
			if (playerAction == "Easy") {
				cout << "You go down the well-trodden path." << endl<<endl;
				Sleep(1000);
				path1();
			}
			else if (playerAction == "Medium") {
				cout << "You decide to go on a bit of an adventure." << endl<<endl;
				Sleep(1000);
				path2();
			}
			else if (playerAction == "Hard") {
				cout << "You go down the darkest path with high hopes of finding treasure." << endl<<endl;
				Sleep(1000);
				path3();
			}
			else {
				cout << "Invalid choice." << endl;
				Sleep(250);
				playerAction.clear();
			}
		} while (playerAction.empty());
		//user chooses if they want to continue playing
		do {
			cout << "Will you continue into another dungeon? ";
			playerAction.clear();
			cin >> playerAction;
			playerAction.at(0) = toupper(playerAction.at(0));
			if (playerAction == "Yes")
				gameOver = false;
			else if (playerAction == "No")
				gameOver = true;
			else {
				cout << "Invalid entry." << endl;
				Sleep(250);
				playerAction.clear();
			}
		} while (playerAction.empty());
	}
	return 0;
}


