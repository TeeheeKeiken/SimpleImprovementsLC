/*-- Smarter Construction --*/

#strict 3
#appendto CLNK

protected func BuildNeedsMaterial(id itemId, int amount)
{
	var commandName = GetCommand(this);
	var target = GetCommand(this, 1);

	var availableInventorySlotsCount = GetAvailableInventorySlotCount();

	if (availableInventorySlotsCount <= 0)
	{
		return _inherited(itemId, amount);
	}

	var objectComponents = {};
	var targetId = GetID(target);
	var counter = 0;
	var componentId;
	while (componentId = GetComponent(nil, counter++, nil, targetId))
	{
		var neededAmount = GetComponent(componentId, nil, nil, targetId);
		var amountAtConstructionSite = GetComponent(componentId, nil, target);

		if (amountAtConstructionSite != nil)
		{
			neededAmount -= amountAtConstructionSite;
		}

		objectComponents[componentId] = neededAmount;
	}

	// Workaround for normal build behaviour, consider refactoring this if possible.
	objectComponents[itemId] -= 1;

	AddCommand(this, "Call", this, target, nil, nil, nil, "CommandEvaluateBuild");
	for (var componentId, neededAmount in objectComponents)
	{
		if (availableInventorySlotsCount <= 0)
		{
			break;
		}

		for (var i = 0; i < neededAmount; i++)
		{
			if (availableInventorySlotsCount <= 0)
			{
				break;
			}

			availableInventorySlotsCount--;
			AddCommand(this, "Call", this, componentId, 1, nil, nil, "CommandAcquireObjects");
		}
	}

	Message(GetNeededMatStr(target), this);
}

public func CommandAcquireObjects(object source, id itemId, int amount)
{
	if (GetAvailableInventorySlotCount() <= 0 && FindContents(itemId, this) != nil)
	{
		return false;
	}

	var items = FindObjects(
		Find_Not(Find_Container(this)),
		Find_ID(itemId),
		Find_Distance(200),
		Sort_Distance());

	var itemCount = GetLength(items);
	if (itemCount <= 0)
	{
		return false;
	}

	if (itemCount < amount)
	{
		amount = itemCount;
	}

	for (var i = 0; i < amount; i++)
	{
		AddCommand(this, "Get", items[0]);
	}

	return true;
}

public func CommandEvaluateBuild(object source, object target)
{
	if (GetCommand(this, 0, 1) != "Build")
	{
		AddCommand(this, "Build", target);
	}
}

private func GetAvailableInventorySlotCount()
{
	return MaxContentsCount() - ObjectCount2(Find_Container(this));
}
