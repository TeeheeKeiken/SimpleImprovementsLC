/*-- Value Victory Point Weight --*/

#strict 3
#appendto VICP

local ValueWeight;

public func Initialize()
{
	ValueWeight = 1000;

	return (inherited());
}

public func CalculateVictoryPointsForPlayer(int playerId)
{
	var playerVictoryPoints = inherited(playerId);

	var playerValue = GetPlrValueGain(playerId);
	playerVictoryPoints += playerValue / ValueWeight;
	return playerVictoryPoints;
}
