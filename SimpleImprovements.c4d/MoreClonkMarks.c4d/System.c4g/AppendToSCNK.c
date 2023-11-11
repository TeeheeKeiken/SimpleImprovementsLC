/*-- Clonk Mars Spaceclonk --*/

#strict 3
#appendto SCNK

protected func FxO2Timer()
{
	var previousO2 = O2;

	_inherited();

	if (LowO2())
	{
		ShowO2WarningMessages();
	}

	if (previousO2 > 0 && O2 <= 0)
	{
		Sound("Warning_lowoxygen2", true, nil, 0, GetOwner() + 1);
	}

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

private func GetWarningColor(int current, int expected)
{
	var red = 255 << 16;
	var green = (255 * current / expected) << 8;
	var blue = 0;
	return red + green + blue;
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

private func ShowO2WarningMessages()
{
	var o2Color = GetWarningColor(O2, 30);
	var playerColor = GetColorDw(this);

	CustomMessage(Format("<c %x>o2</c>: <c %x>%d%%</c>", playerColor, o2Color, O2), this, NO_OWNER);

	var cursor = GetCursor(GetOwner());
	if (cursor != this)
	{
		var message = Format(
			"$LowOnOxygen$",
			playerColor,
			GetName(this));

		var portrait = Format("Portrait:%i::%x::%s", GetID(this), playerColor, GetPortrait(this, false, false));

		CustomMessage(
			message,
			nil,
			GetOwner(),
			0,
			25,
			0xffffff,
			SFMD,
			portrait,
			MSG_Top | MSG_HCenter | MSG_YRel);
	}
}
