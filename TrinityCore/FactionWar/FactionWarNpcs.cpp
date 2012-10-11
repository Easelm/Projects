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
#include "FactionWar.h"
static FactionWar factionWar;

static Location locations[] = 
{
	/*  Astranaar NPC Spawns */
	{ 2728.304f, -430.972f, 108.816956f, 2.835257f },
	/* Splintertree NPC Spawns */
	{ 2278.343f, -2523.499f, 97.658371f, 3.582160f },
	/* Splintertree Base Guard Spawn Locations */
	{ 2220.397f, -2547.415f, 85.436203f, 2.351996f },
	{ 2220.747f, -2528.477f, 84.717392f, 3.218291f },
	{ 2246.476f, -2529.714f, 92.024796f, 4.033531f },
	{ 2247.379f, -2551.588f, 90.258766f, 2.395975f }
};

static Waypoints waypoints[] =
{
	{ },
	/* Splintertree NPC Waypoints */
	{ 1, 2232.689f, -2540.322f, 87.151680f, 2.994325f },
	{ 2, 2199.957f, -2525.335f, 81.887581f, 2.491666f },
	{ 3, 2161.294f, -2499.362f, 79.792213f, 2.625184f },
	{ 4, 2113.347f, -2470.833f, 81.907562f, 2.389565f },
	{ 5, 2060.820f, -2421.534f, 87.170036f, 2.695870f },
	{ 6, 2000.545f, -2395.062f, 89.300522f, 1.990349f },
	{ 7, 1950.070f, -2280.594f, 91.488533f, 1.754730f },
	{ 8, 1930.723f, -2163.582f, 93.615677f, 1.252074f },
	{ 9, 1968.390f, -1995.599f, 96.493195f, 1.204951f },
	{ 10, 2008.603f, -1899.979f, 98.502579f, 0.623756f },
	{ 11, 2171.254f, -1783.098f, 101.723885f, 0.521654f },
	{ 12, 2273.168f, -1724.195f, 114.672012f, 0.800470f },
	{ 13, 2324.556f, -1669.796f, 124.625069f, 1.157827f },
	{ 14, 2362.465f, -1553.154f, 125.286140f, 1.322760f },
	{ 15, 2437.212f, -1325.737f, 125.279434f, 1.499475f },
	{ 16, 2469.889f, -1155.896f, 125.181503f, 1.240294f },
	{ 17, 2516.389f, -984.656f, 129.455795f, 1.562307f },
	{ 18, 2513.096f, -875.639f, 134.581512f, 2.104232f },
	{ 19, 2479.504f, -799.719f, 137.150238f, 1.727241f },
	{ 20, 2445.496f, -557.477f, 115.004616f, 1.413082f },
	{ 21, 2453.443f, -510.455f, 114.829971f, 0.352794f },
	{ 22, 2632.077f, -448.599f, 106.659004f, 0.399918f }
};

class npc_start_battle : public CreatureScript // You can spawn this NPC anywhere
{
   public:
	   npc_start_battle() : CreatureScript("npc_start_battle") { }

	   bool OnGossipHello(Player * player, Creature * creature)
	   {
		   if(!factionWar.activeBattle)
			   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Fight Computer", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
		   else
			   player->GetSession()->SendAreaTriggerMessage("A battle is active. You must wait until it ends.");
		   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
		   player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
		   return true;
	   }

	   bool OnGossipSelect(Player * player, Creature * creature, uint32 sender, uint32 actions)
	   {
		   player->PlayerTalkClass->ClearMenus();
		   if(sender != GOSSIP_SENDER_MAIN)
			   return false;

		   switch(actions)
		   {
		       case GOSSIP_ACTION_INFO_DEF+1:
				   factionWar._playerGUID = player->GetGUID();
				   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Locations..", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
				   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
				   player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
				   break;

			   case GOSSIP_ACTION_INFO_DEF+2:
				   player->PlayerTalkClass->SendCloseGossip();
				   break;

				/* Locations */
			   case GOSSIP_ACTION_INFO_DEF+5:
				   switch(player->GetTeam())
				   {
				       case ALLIANCE:
					       player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Astranaar vs Splintertree", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
						   break;

					   case HORDE:
						   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Splintertree vs Alvanaar", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+7);
						   break;
				   }
				   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
				   player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
				   break;

			   case GOSSIP_ACTION_INFO_DEF+6:
				   factionWar.activeBattle = true;
				   factionWar.unlockedCount = 1;
				   factionWar.foodCap = 10;
				   factionWar.baseType = "Astranaar Night Elf";
				   ChatHandler(player).SendSysMessage("Battle is active! Your battle type is Astranaar vs Splintertree. Go to the commander to start.");
				   player->PlayerTalkClass->SendCloseGossip();
				   break;

			   case GOSSIP_ACTION_INFO_DEF+7:
				   factionWar.activeBattle = true;
				   factionWar.unlockedCount = 1;
				   factionWar.baseType = "Splintertree Orc";
				   ChatHandler(player).SendSysMessage("Battle is active! Your battle type is Splintertree vs Astranaar. Go to the commander to start.");
				   player->PlayerTalkClass->SendCloseGossip();
				   break;
		   }
		   return true;
	   }
};

// Commander
class npc_main_commander : public CreatureScript
{
   public:
	   npc_main_commander() : CreatureScript("npc_main_commander") { }

	   bool OnGossipHello(Player * player, Creature * creature)
	   {
		   if(factionWar._playerGUID == player->GetGUID())
		   {
			   if(factionWar.activeBattle)
				   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I forfeit this game!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+35);
			   if(!factionWar.stopSendingUnits && factionWar.activeBattle)
				   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Stop Sending Units..", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
			   else
				   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Send Units!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
		   }
		   else
			   player->GetSession()->SendAreaTriggerMessage("You cannot speak to the Commander!");
		   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
		   player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
		   return true;
	   }

	   bool OnGossipSelect(Player * player, Creature * creature, uint32 sender, uint32 actions)
	   {
		   player->PlayerTalkClass->ClearMenus();
		   if(sender != GOSSIP_SENDER_MAIN)
			   return false;

		   switch(actions)
		   {
			   case GOSSIP_ACTION_INFO_DEF+2:
				   player->PlayerTalkClass->SendCloseGossip();
				   break;

			   case GOSSIP_ACTION_INFO_DEF+3:
				   factionWar.stopSendingUnits = true;
				   ChatHandler(player).SendSysMessage("Stopping Units...");
				   player->PlayerTalkClass->SendCloseGossip();
				   break;

			   case GOSSIP_ACTION_INFO_DEF+4:
				   factionWar.stopSendingUnits = false;
				   ChatHandler(player).SendSysMessage("Sending Units...");
				   player->PlayerTalkClass->SendCloseGossip();
				   break;

			   case GOSSIP_ACTION_INFO_DEF+35: // Reseting
				   factionWar.activeBattle = false;
				   factionWar.unlockedCount = 0;
				   factionWar.baseType = "";
				   factionWar.enemiesKilled = 0;
				   factionWar.enemiesSent = 0;
				   factionWar.stopSendingUnits = false;
				   factionWar.unitsKilled = 0;
				   factionWar.unitsSent = 0;
				   factionWar._playerGUID = 0;
				   factionWar.foodCap = 10;
				   ChatHandler(player).SendSysMessage("Forfeit confirmed. Thanks for playing.");
				   player->PlayerTalkClass->SendCloseGossip();
				   break;
		   }
		   return true;
	   }

	   struct npc_main_commanderAI : public ScriptedAI
	   {
		   npc_main_commanderAI(Creature * creature) : ScriptedAI(creature) { }

		   uint32 spawnDifference;
		   uint32 foodStockTimer;

		   void Reset()
		   {
			   spawnDifference = 1000;
			   foodStockTimer = 1000;
		   }

		   void UpdateAI(const uint32 diff)
		   {
			   if(!factionWar.activeBattle)
			   {
				   spawnDifference = 1000;
				   foodStockTimer = 1000;
				   return;
			   }

			   if(foodStockTimer <= diff && factionWar.activeBattle)
			   {
				   if(factionWar.foodCap <= 0)
				   {
					   factionWar.stopSendingUnits = true;
					   factionWar.foodReady = true;
					   factionWar.foodCap = 0;
				   }
				   else
					   factionWar.stopSendingUnits = false;

				   if(factionWar.foodCap <= 2) // If less or equals two- it needs to be filled up
					   factionWar.foodReady = true;
				   int rand = urand(0, 5);
				   switch(rand) // Randomize this so the doubts will be there - Cap is 10
				   {
				      case 0:
						  break;

					  case 1:
						  factionWar.foodCap--;
						  break;

					  case 2:
						  break;

					  case 3:
						  factionWar.foodCap--;
						  break;

					  case 4:
						  break;
				   }
				   foodStockTimer = 30000; 
			   }
			   else
				   foodStockTimer -= diff;

			   if(spawnDifference <= diff && !factionWar.stopSendingUnits)
			   {
				   if (factionWar.activeBattle) // Commander will send units out
				   {
					   if (factionWar.baseType == "Astranaar Night Elf")
					   {
						   switch(factionWar.unlockedCount)
						   {
						      case 1: // Normal unit
								  me->SummonCreature(91000, locations[0].x, locations[0].y, locations[0].z, locations[0].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
								  me->SummonCreature(91001, locations[1].x, locations[1].y, locations[1].z, locations[1].o, TEMPSUMMON_MANUAL_DESPAWN, 0); // Computer
								  break;

							  case 2: // Normal and special (randomized)
								  int random = urand(0, 1);
								  switch(random)
								  {
								      case 0: // Special Unit
										  break;

									  case 1: // Normal Unit
										  me->SummonCreature(91000, locations[0].x, locations[0].y, locations[0].z, locations[0].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
										  me->SummonCreature(91001, locations[1].x, locations[1].y, locations[1].z, locations[1].o, TEMPSUMMON_MANUAL_DESPAWN, 0); // Computer
										  break;
								  }
								  break;
						   }
					   }
					   else if (factionWar.baseType == "Splintertree Orc")
					   {
						   switch(factionWar.unlockedCount)
						   {
						      case 1:
								  me->SummonCreature(91001, locations[1].x, locations[1].y, locations[1].z, locations[1].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
								  me->SummonCreature(91000, locations[0].x, locations[0].y, locations[0].z, locations[0].o, TEMPSUMMON_MANUAL_DESPAWN, 0); // Computer
								  break;

							  case 2: // Normal and special (randomized)
								  int random = urand(0, 1);
								  switch(random)
								  {
								      case 0: // Special Unit
										  break;

									  case 1: // Normal Units
										  me->SummonCreature(91001, locations[1].x, locations[1].y, locations[1].z, locations[1].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
										  me->SummonCreature(91000, locations[0].x, locations[0].y, locations[0].z, locations[0].o, TEMPSUMMON_MANUAL_DESPAWN, 0); // Computer
										  break;
								  }
								  break;
						   }
					   }
					   spawnDifference = urand(6000, 12000); // This timer goes for both Player and Computer
				   }
			   }
			   else
				   spawnDifference -= diff;
		   }
	   };

	   CreatureAI * GetAI(Creature * pCreature) const
	   {
		   return new npc_main_commanderAI(pCreature);
	   }
};

class npc_upgrade_dealer : public CreatureScript
{
   public:
	   npc_upgrade_dealer() : CreatureScript("npc_upgrade_dealer") { }
};

class npc_food_stocker : public CreatureScript
{
   public:
	   npc_food_stocker() : CreatureScript("npc_food_stocker") { }

	   bool OnGossipHello(Player * player, Creature * creature)
	   {
		   if(factionWar._playerGUID == player->GetGUID())
		   {
			   if(factionWar.foodReady)
				   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Purchase Food", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
			   else
				   player->GetSession()->SendAreaTriggerMessage("You're not ready to buy food yet!");
		   }
		   else
			   player->GetSession()->SendAreaTriggerMessage("You cannot speak to the Stocker!");
		   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
		   player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
		   return true;
	   }

	   bool OnGossipSelect(Player * player, Creature * creature, uint32 sender, uint32 actions)
	   {
		   player->PlayerTalkClass->ClearMenus();
		   if(sender != GOSSIP_SENDER_MAIN)
			   return false;

		   switch(actions)
		   {
		       case GOSSIP_ACTION_INFO_DEF+1:
				   switch(factionWar.foodCap) // We're only going by what the food cap check is set to
				   {
				      case 0:
						  factionWar.foodCap = 10;
						  break;

					  case 1:
						  factionWar.foodCap = 10;
						  break;

					  case 2:
						  factionWar.foodCap = 10;
						  break;
				   }
				   factionWar.foodReady = false;
				   player->PlayerTalkClass->SendCloseGossip();
				   break;

			   case GOSSIP_ACTION_INFO_DEF+2:
				   player->PlayerTalkClass->SendCloseGossip();
				   break;
		   }
		   return true;
	   }
};

class npcs_astranaar : public CreatureScript
{
   public:
	   npcs_astranaar() : CreatureScript("npcs_astranaar") { }

	   struct npcs_astranaarAI : public ScriptedAI
	   {
		   npcs_astranaarAI(Creature * c) : ScriptedAI(c) { }

		   int moved;

		   void Reset()
		   {
			   moved = 0;
			   me->SetSpeed(MOVE_RUN, 1.0f, true);
		   }

		   void MovementInform(uint32 type, uint32 id)
		   {
			   if (type != POINT_MOTION_TYPE)
				   return;
			   moved++;
		   }

		   void UpdateAI(uint32 const diff)
		   {
			   if(factionWar.activeBattle)
			   {
				   switch(moved)
				   {
				       case 0:
				           me->GetMotionMaster()->MovePoint(1, waypoints[22].x, waypoints[22].y, waypoints[22].z);
					       break;

				       case 1:
						   me->GetMotionMaster()->MovePoint(2, waypoints[21].x, waypoints[21].y, waypoints[21].z);
						   break;

				       case 2:
						   me->GetMotionMaster()->MovePoint(3, waypoints[20].x, waypoints[20].y, waypoints[20].z);
						   break;

				       case 3:
						   me->GetMotionMaster()->MovePoint(4, waypoints[19].x, waypoints[19].y, waypoints[19].z);
						   break;

				       case 4:
						   me->GetMotionMaster()->MovePoint(5, waypoints[18].x, waypoints[18].y, waypoints[18].z);
						   break;

				       case 5:
						   me->GetMotionMaster()->MovePoint(6, waypoints[17].x, waypoints[17].y, waypoints[17].z);
						   break;

				       case 6:
						   me->GetMotionMaster()->MovePoint(7, waypoints[16].x, waypoints[16].y, waypoints[16].z);
						   break;

				       case 7:
						   me->GetMotionMaster()->MovePoint(8, waypoints[15].x, waypoints[15].y, waypoints[15].z);
						   break;

				       case 8:
						   me->GetMotionMaster()->MovePoint(9, waypoints[14].x, waypoints[14].y, waypoints[14].z);
						   break;

				       case 9:
						   me->GetMotionMaster()->MovePoint(10, waypoints[13].x, waypoints[13].y, waypoints[13].z);
						   break;

				       case 10:
						   me->GetMotionMaster()->MovePoint(11, waypoints[12].x, waypoints[12].y, waypoints[12].z);
						   break;

				       case 11:
						   me->GetMotionMaster()->MovePoint(12, waypoints[11].x, waypoints[11].y, waypoints[11].z);
						   break;

				       case 12:
						   me->GetMotionMaster()->MovePoint(13, waypoints[10].x, waypoints[10].y, waypoints[10].z);
						   break;

				       case 13:
						   me->GetMotionMaster()->MovePoint(14, waypoints[9].x, waypoints[9].y, waypoints[9].z);
						   break;

				       case 14:
						   me->GetMotionMaster()->MovePoint(15, waypoints[8].x, waypoints[8].y, waypoints[8].z);
						   break;

				       case 15:
						   me->GetMotionMaster()->MovePoint(16, waypoints[7].x, waypoints[7].y, waypoints[7].z);
						   break;

				       case 16:
						   me->GetMotionMaster()->MovePoint(17, waypoints[6].x, waypoints[6].y, waypoints[6].z);
						   break;

				       case 17:
						   me->GetMotionMaster()->MovePoint(18, waypoints[5].x, waypoints[5].y, waypoints[5].z);
						   break;

				       case 18:
						   me->GetMotionMaster()->MovePoint(19, waypoints[4].x, waypoints[4].y, waypoints[4].z);
						   break;

				       case 19:
						   me->GetMotionMaster()->MovePoint(20, waypoints[3].x, waypoints[3].y, waypoints[3].z);
						   break;

				       case 20:
						   me->GetMotionMaster()->MovePoint(21, waypoints[2].x, waypoints[2].y, waypoints[2].z);
						   break;

				       case 21:
						   me->GetMotionMaster()->MovePoint(22, waypoints[1].x, waypoints[1].y, waypoints[1].z);
						   moved = 22;
						   break;
				   }
			   }
			   DoMeleeAttackIfReady();
		   }
		   
	   };
	   
	   CreatureAI * GetAI(Creature * pCreature) const
	   {
		   return new npcs_astranaarAI(pCreature);
	   }
};

class npcs_splintertree : public CreatureScript
{
   public:
	   npcs_splintertree() : CreatureScript("npcs_splintertree") { }

	   struct npcs_splintertreeAI : public ScriptedAI
	   {
		   npcs_splintertreeAI(Creature * c) : ScriptedAI(c) { }

		   uint32 waypointTimer;
		   int moved;

		   void Reset()
		   {
			   waypointTimer = 1000;
			   moved = 0;
			   me->SetSpeed(MOVE_RUN, 1.0f, true);
		   }

		   void MovementInform(uint32 type, uint32 id)
		   {
			   if (type != POINT_MOTION_TYPE)
				   return;
			   moved++;
		   }

		   void UpdateAI(uint32 const diff)
		   {
			   if(!factionWar.activeBattle)
				   waypointTimer = 1000;

			   if(factionWar.activeBattle)
			   {
				   switch(moved)
				   {
				       case 0:
				           me->GetMotionMaster()->MovePoint(1, waypoints[1].x, waypoints[1].y, waypoints[1].z);
					       break;

				       case 1:
						   me->GetMotionMaster()->MovePoint(2, waypoints[2].x, waypoints[2].y, waypoints[2].z);
						   break;

				       case 2:
						   me->GetMotionMaster()->MovePoint(3, waypoints[3].x, waypoints[3].y, waypoints[3].z);
						   break;

				       case 3:
						   me->GetMotionMaster()->MovePoint(4, waypoints[4].x, waypoints[4].y, waypoints[4].z);
						   break;

				       case 4:
						   me->GetMotionMaster()->MovePoint(5, waypoints[5].x, waypoints[5].y, waypoints[5].z);
						   break;

				       case 5:
						   me->GetMotionMaster()->MovePoint(6, waypoints[6].x, waypoints[6].y, waypoints[6].z);
						   break;

				       case 6:
						   me->GetMotionMaster()->MovePoint(7, waypoints[7].x, waypoints[7].y, waypoints[7].z);
						   break;

				       case 7:
						   me->GetMotionMaster()->MovePoint(8, waypoints[8].x, waypoints[8].y, waypoints[8].z);
						   break;

				       case 8:
						   me->GetMotionMaster()->MovePoint(9, waypoints[9].x, waypoints[9].y, waypoints[9].z);
						   break;

				       case 9:
						   me->GetMotionMaster()->MovePoint(10, waypoints[10].x, waypoints[10].y, waypoints[10].z);
						   break;

				       case 10:
						   me->GetMotionMaster()->MovePoint(11, waypoints[11].x, waypoints[11].y, waypoints[11].z);
						   break;

				       case 11:
						   me->GetMotionMaster()->MovePoint(12, waypoints[12].x, waypoints[12].y, waypoints[12].z);
						   break;

				       case 12:
						   me->GetMotionMaster()->MovePoint(13, waypoints[13].x, waypoints[13].y, waypoints[13].z);
						   break;

				       case 13:
						   me->GetMotionMaster()->MovePoint(14, waypoints[14].x, waypoints[14].y, waypoints[14].z);
						   break;

				       case 14:
						   me->GetMotionMaster()->MovePoint(15, waypoints[15].x, waypoints[15].y, waypoints[15].z);
						   break;

				       case 15:
						   me->GetMotionMaster()->MovePoint(16, waypoints[16].x, waypoints[16].y, waypoints[16].z);
						   break;

				       case 16:
						   me->GetMotionMaster()->MovePoint(17, waypoints[17].x, waypoints[17].y, waypoints[17].z);
						   break;

				       case 17:
						   me->GetMotionMaster()->MovePoint(18, waypoints[18].x, waypoints[18].y, waypoints[18].z);
						   break;

				       case 18:
						   me->GetMotionMaster()->MovePoint(19, waypoints[19].x, waypoints[19].y, waypoints[19].z);
						   break;

				       case 19:
						   me->GetMotionMaster()->MovePoint(20, waypoints[20].x, waypoints[20].y, waypoints[20].z);
						   break;

				       case 20:
						   me->GetMotionMaster()->MovePoint(21, waypoints[21].x, waypoints[21].y, waypoints[21].z);
						   break;

				       case 21:
						   me->GetMotionMaster()->MovePoint(22, waypoints[22].x, waypoints[22].y, waypoints[22].z);
						   break;
				   }
			   }
			   DoMeleeAttackIfReady();
		   }
	   };
	   
	   CreatureAI * GetAI(Creature * pCreature) const
	   {
		   return new npcs_splintertreeAI(pCreature);
	   }
};


void AddSC_faction_wars()
{
	new npc_main_commander();
	new npc_start_battle();
	new npc_upgrade_dealer();
	new npc_food_stocker();
	new npcs_astranaar();
	new npcs_splintertree();
}