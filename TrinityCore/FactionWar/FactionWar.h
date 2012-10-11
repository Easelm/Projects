/*******************************************************************************************
*						 __                           __ _   							   *
*					  /\ \ \___  _ __ ___  ___  ___  / _| |_ 							   *
*					 /  \/ / _ \| '_ ` _ \/ __|/ _ \| |_| __|							   *
*					/ /\  / (_) | | | | | \__ \ (_) |  _| |_ 							   *
*					\_\ \/ \___/|_| |_| |_|___/\___/|_|  \__|	- www.Nomsoftware.com -	   *
*                               The policy of Nomsoftware states: Releasing our software   *
*                               or any other files are protected. You cannot re-release    *
*                               anywhere unless you were given permission.                 *
*                           (C) Nomsoftware 'Nomsoft' 2011-2012. All rights reserved.      *
********************************************************************************************/
#include "ScriptPCH.h"
using namespace std;


struct Location
{
	float x, y, z, o;
};

struct Waypoints
{
	uint32 wpId;
	float x, y, z, o;
};

class FactionWar
{
   public:   
	   uint32             FoodCapReached() const { return foodCap; }

	   bool               IsReadyForFood() const { return foodReady; }

	   void               MakeFoodReady(bool ready);
	   void               RaiseLowerFoodCap(bool lower);

	   bool               activeBattle;
	   bool               stopSendingUnits;
	   bool               foodReady;

	   uint64             _playerGUID;

	   uint32             unitsSent; // How many units your base sent out
	   uint32             unitsKilled; // How many the enemy kills your units received
	   uint32             enemiesSent; // How many enemy units sent
	   uint32             enemiesKilled; // How many enemies killed your units
	   uint32             unlockedCount; // Player's/Computer's unlocked units
	   uint32             splinterTreeGuards;
	   uint32             foodCap;

	   string             baseType;
		
		
};