/*-- Hack for RTG Temperature --*/

#strict 3
#appendto TEMP

protected func FxTemperatureTimer(object pTarget, int iEffectNumber)
{
	if (FindObject2(Find_ID(RTEG), Find_Distance(24, GetX(pTarget), GetY(pTarget))) != nil)
	{
		var iTemp = EffectVar(0, pTarget, iEffectNumber);

		var k = 10;

		var iOuterTemp = 500;
		
		var diff = iOuterTemp - iTemp;
		var iChange =  k * (diff) / 100;
		if(!iChange && diff != 0) {
			if(diff < 0)
				iChange = -1;
			else
				iChange = 1;
		}

		if(Abs(iChange) == 1 && pTarget -> ~HeavySuit()) {
			iChange = 0;
		}

		iTemp += iChange;

		EffectVar(0, pTarget, iEffectNumber) = iTemp;

		var iLimit = MaxTemp * 3 / 4;
		if(iTemp < -iLimit) {
			pTarget -> DoEnergy(-ChangeRange(Abs(iTemp) - iLimit, 0, MaxTemp - iLimit, 1, 10));
		}
		
		if(iTemp > iLimit) {
			pTarget -> DoEnergy(-ChangeRange(Abs(iTemp) - iLimit, 0, MaxTemp - iLimit, 1, 15));
			Schedule("CreateParticle(\"Fire2\", -6 + PRandom(12), -5 + PRandom(15), GetWind() / 2, -PRandomX(10, 20), PRandomX(20, 50), 0, this)", 1, 25, pTarget);
		}

		pTarget -> UpdateHUDValue(HUD_ClonkTemp, iTemp);
		return true;
	}

	return _inherited(pTarget, iEffectNumber);
}
