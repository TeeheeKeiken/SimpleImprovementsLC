/*-- RTG --*/

#strict 3

#include PWRG // Generates power
#include B_50
#include DACT // Damage control

local _energyProduction;

protected func Initialize()
{
	_energyProduction = 10;

	_inherited();
}

// Maximum amount of power that can be stored in this power generator.
public func GetCapacity() 
{
	return 150;
}

// Returns the generator's priority, consumers preferably drain from generators with the highest priority.
public func GetGeneratorPriority()
{
	return 1;
}

private func ProducePower()
{
	DoPower(_energyProduction);
	DebugLog("Energy: %d", GetPower());
}

/* Controls */

protected func ControlUp()
{
	[$TxtShowEnergy$|Image=CXEC]
	ShowEnergyMessage();
}

protected func ContextShowEnergy()
{
	[$TxtShowEnergy$|Image=CXEC]
	ShowEnergyMessage();
}

private func ShowEnergyMessage()
{
	Message("$TxtShowEnergyMsg$", this, _energyProduction);
}
