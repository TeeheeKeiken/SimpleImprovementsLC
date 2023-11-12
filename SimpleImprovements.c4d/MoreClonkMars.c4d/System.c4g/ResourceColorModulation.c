/*-- Resource Color Modulation --*/

#strict 3

global func ISB_SI_GetResourceColorModulation(id itemId)
{
	if (itemId == ROCK)
	{
		return GetMaterialObjectColor(Material("Rock"));
	}

	if (itemId == ERTH)
	{
		return GetMaterialObjectColor(Material("Earth"));
	}

	return _inherited(itemId);
}
