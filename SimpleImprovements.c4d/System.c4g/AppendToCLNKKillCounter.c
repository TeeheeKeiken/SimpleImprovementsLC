/*-- Kill Counter --*/

#strict 3
#appendto CLNK

/*-- Pandoras Strike Compatibility --*/
#appendto WIEF nowarn

static ISB_SI_PlayerKillCountMap;

protected func Death()
{
	EvaluateKill();

	return _inherited();
}

private func EvaluateKill()
{
	var killerId = GetKiller();

	if (killerId == NO_OWNER)
	{
		return;
	}

	var ownerId = GetOwner();

	if (ownerId == NO_OWNER || ownerId == killerId)
	{
		return;
	}

	if (!Hostile(killerId, ownerId))
	{
		return;
	}

	var playerKillCount = ISB_SI_PlayerKillCountMap[killerId] ?? 0;
	ISB_SI_PlayerKillCountMap[killerId] = ++playerKillCount;
}
