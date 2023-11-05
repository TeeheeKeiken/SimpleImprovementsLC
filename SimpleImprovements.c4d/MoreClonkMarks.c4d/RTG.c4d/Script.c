/*-- RTG --*/

#strict 3

#include PWRG // Generates power
#include B_50
#include DACT // Damage control

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
	DoPower(10);
	DebugLog("Energy: %d", GetPower());
}
