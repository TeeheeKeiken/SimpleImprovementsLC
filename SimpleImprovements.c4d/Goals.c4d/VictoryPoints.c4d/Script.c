/*-- Victory Points --*/

#strict 3
#include GOAL

local RequiredVictoryPoints;

protected func Initialize()
{
	var goal;
	if (goal = FindObject(GetID()))
	{
		goal.RequiredVictoryPoints += 1;
		return (RemoveObject());
	}

	// Set required victory points
	RequiredVictoryPoints = 1;
	return (inherited());
}

/*-- Competitive --*/
public func IsFulfilledforPlr(int playerId)
{
	// TODO: Consider teams
	var playerVictoryPoints = CalculateVictoryPointsForPlayer(playerId);

	return (playerVictoryPoints >= RequiredVictoryPoints);
}

/*-- Cooperative --*/
public func IsFulfilled()
{
	var totalVictoryPoints = 0;
	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var playerId = GetPlayerByIndex(i, C4PT_User);

		totalVictoryPoints += CalculateVictoryPointsForPlayer(playerId);
	}

	return (totalVictoryPoints >= RequiredVictoryPoints);
}

protected func Activate(player)
{
	if (IsFulfilled())
	{
		return (MessageWindow(Format("$MsgGoalFulfilled$", RequiredVictoryPoints), player));
	}

	MessageWindow(Format("$MsgGoalUnfulfilled$", , RequiredVictoryPoints), player);
	return true;
}

/*-- Basic implementation --*/
public func CalculateVictoryPointsForPlayer(int playerId)
{
	return 0;
}
