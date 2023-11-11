/*-- Clonk Mars Spaceclonk --*/

#strict 3
#appendto SCNK

protected func FxO2Timer()
{
	_inherited();

	if (!GetAlive(this))
	{
		HandleLowO2Warning();
	}
}

protected func FxO2Stop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
	_inherited(pTarget, iEffectNumber, iReason, fTemp);

	HandleLowO2Warning();
}

private func HandleLowO2Warning()
{
	var amountOfOtherLowO2Clonks = ObjectCount2(
		Find_Exclude(this),
		Find_ID(GetID()),
		Find_OCF(OCF_Alive),
		Find_Owner(GetOwner()),
		Find_Func("LowO2"));

	if (amountOfOtherLowO2Clonks <= 0)
	{
		Sound("Warning_lowoxygen", true, nil, 0, GetOwner() + 1, -1);
	}
	else
	{
		Sound("Warning_lowoxygen", true, nil, 0, GetOwner() + 1, +1);
	}
}
