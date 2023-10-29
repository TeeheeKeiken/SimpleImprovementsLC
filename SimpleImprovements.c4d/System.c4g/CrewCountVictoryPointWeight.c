/*-- Crew Count Victory Point Weight --*/

#strict 3
#appendto VICP

local CrewCountWeight;

public func Initialize()
{
	CrewCountWeight = 5;

	return inherited();
}

public func CalculateVictoryPointsForPlayer(int playerId)
{
	var playerVictoryPoints = inherited(playerId);

	var playerCrewCount = GetCrewCount(playerId);
	playerVictoryPoints += playerCrewCount / CrewCountWeight;
	return playerVictoryPoints;
}
