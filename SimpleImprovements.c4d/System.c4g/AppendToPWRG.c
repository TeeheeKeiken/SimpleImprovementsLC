/*-- Clonk Mars Power Generator --*/
/*-- TODO: Move to separate object folder --*/

#strict 3
#appendto PWRG nowarn

protected func ContextShowStoredEnergy()
{
	[$ContextShowStoredEnergy$|Image=ACCU]
	Message("$ShowStoredEnergyMessage$", this, GetPower(), GetCapacity());
}
