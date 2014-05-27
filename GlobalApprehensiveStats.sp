#include <sourcemod>

#define STRING_SIZE 64

public Plugin:myinfo =
{
	name = "Global Apprehensive Stats",
	author = "",
	description = "Counter-Strike: Global Offensive stats",
	version = "",
	url = ""
}

new String:victimWeapon[STRING_SIZE]

public OnPluginStart()
{
	HookEvent("player_death", OnPlayerDeath)
	HookEvent("player_hurt", OnPlayerHurt)
}

GetClientFromField(Handle:event, const String:field[])
{
	return GetClientOfUserId(GetEventInt(event, field))
}

public OnPlayerHurt(Handle:event, const String:name[], bool:dontBroadcast)
{
	new victim = GetClientFromField(event, "userid")
	GetClientWeapon(victim, victimWeapon, sizeof(victimWeapon))
}

public OnPlayerDeath(Handle:event, const String:name[], bool:dontBroadcast)
{
	new attacker = GetClientFromField(event, "attacker")
	decl String:attackerName[MAX_NAME_LENGTH]
	GetClientName(attacker, attackerName, sizeof(attackerName))
	decl String:attackerSteamId[STRING_SIZE]
	GetClientAuthString(attacker, attackerSteamId, sizeof(attackerSteamId))
	new assister = GetClientFromField(event, "assister")
	new victim = GetClientFromField(event, "userid")
	decl String:victimName[MAX_NAME_LENGTH]
	GetClientName(victim, victimName, sizeof(victimName))
	decl String:victimSteamId[STRING_SIZE]
	GetClientAuthString(victim, victimSteamId, sizeof(victimSteamId))
	new bool:headshot = GetEventBool(event, "headshot")
	decl String:attackerWeapon[STRING_SIZE]
	GetEventString(event, "weapon", attackerWeapon, sizeof(attackerWeapon))
	PrintToChatAll("%s (%s, %s) killed %s (%s, %s)", attackerName, attackerSteamId, attackerWeapon, victimName, victimSteamId, victimWeapon)
	PrintToChatAll("Assister: %d", assister)
}