

#include "ScriptPCH.h"

class System_OnKill : public PlayerScript
{
	public:
		System_OnKill() : PlayerScript("System_OnKill") {}

	void OnPVPKill(Player * Killer, Player * Victim)
	{
		uint64 KillerGUID = Killer->GetGUID();
		uint64 VictimGUID = Victim->GetGUID();

		struct KillStreak_Info
		{
			uint32 killstreak;
			uint64 lastkill;
		};

		static std::map<uint64, KillStreak_Info> KillStreakData;

		if( KillerGUID == VictimGUID || KillStreakData[KillerGUID].lastkill == VictimGUID )
			return;

		KillStreakData[KillerGUID].killstreak++;
		KillStreakData[KillerGUID].lastkill = VictimGUID;
		KillStreakData[VictimGUID].killstreak = 0;
		KillStreakData[VictimGUID].lastkill = 0;

		if( KillStreakData[KillerGUID].killstreak % 2 == 0 ) // send killstreak message every 10 kills
		{
			char msg[250];
			snprintf(msg, 250, "[PvP System]: %s is on a killstreak of %u!", Killer->GetName(), KillStreakData[KillerGUID].killstreak);
			sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
			Killer->AddAura(24378, Killer); // gives berserk on 10 kills as buf
		}
		
		if( KillStreakData[KillerGUID].killstreak % 10 == 0 ) // reward every 20 kills
			Killer->AddItem(38186, 10); // gives 10 plus ethereal credits
	}
};

void AddSC_PvP_System()
{
    new System_OnKill;
}