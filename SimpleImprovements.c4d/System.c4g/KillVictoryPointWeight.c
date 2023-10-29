/*-- Kill Victory Point Weight --*/

#strict 3
#appendto VICP

static ISB_SI_PlayerKillCountMap;

local KillWeight;

public func Initialize()
{
	KillWeight = 5;
	ISB_SI_PlayerKillCountMap = [];

	return (inherited());
}

public func CalculateVictoryPointsForPlayer(int playerId)
{
	var playerVictoryPoints = inherited(playerId);

	var playerKillCount = ISB_SI_PlayerKillCountMap[playerId] ?? 0;
	playerVictoryPoints += playerKillCount / KillWeight;
	return playerVictoryPoints;
}
