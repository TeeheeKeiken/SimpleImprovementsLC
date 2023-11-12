/*-- Clonk Mars Power Generator --*/

#strict 3
#appendto PWRG nowarn

protected func ContextShowStoredEnergy()
{
	[$ContextShowStoredEnergy$|Image=ACCU]
	Message("$ShowStoredEnergyMessage$", this, GetPower(), GetCapacity());
}
