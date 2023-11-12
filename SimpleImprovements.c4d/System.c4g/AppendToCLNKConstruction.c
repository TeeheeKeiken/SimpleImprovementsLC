/*-- Smarter Construction --*/

#strict 3
#appendto CLNK

protected func ControlCommand(commandName, targetObject, targetX, targetY, targetObject2, data)
{
	if (ObjectCount(CNMT) <= 0)
	{
		return _inherited(...);
	}

	// Handle drag & drop construction
	if (commandName == "Construct")
	{
		// Prepare components before travelling to the
		// construction site.
		var availableInventorySlotsCount = GetAvailableInventorySlotCount();
		if (availableInventorySlotsCount > 0)
		{
			var targetId = CastC4ID(data);
			var objectComponents = GetMaxRequiredComponents(targetId);

			// Using a construction kit allows other clonks to help
			// out, so we don't want to take as much time preparing
			// components.
			var distance = Distance(GetX(), GetY(), targetX, targetY);
			if (distance > 200)
			{
				distance = 200;
			}
			else if (distance < 50)
			{
				distance = 50;
			}
			
			AddCommand(this, "Construct", nil, targetX, targetY, nil, nil, targetId);
			AcquireComponents(objectComponents, availableInventorySlotsCount, distance);

			return true;
		}
	}

	if (commandName == "Build")
	{
		// Prepare components before travelling to the
		// construction site.
		var availableInventorySlotsCount = GetAvailableInventorySlotCount();
		if (availableInventorySlotsCount > 0)
		{
			var objectComponents = GetRequiredComponents(targetObject);
			AddCommand(this, "Build", targetObject);
			AcquireComponents(objectComponents, availableInventorySlotsCount, 200);

			return true;
		}
	}

	return _inherited(...);
}

protected func BuildNeedsMaterial(id itemId, int amount)
{
	if (ObjectCount(CNMT) <= 0)
	{
		return _inherited(...);
	}

	var commandName = GetCommand(this);
	var target = GetCommand(this, 1);

	var availableInventorySlotsCount = GetAvailableInventorySlotCount();

	if (availableInventorySlotsCount <= 0)
	{
		return _inherited(...);
	}

	var objectComponents = GetRequiredComponents(target);

	// Workaround for normal build behaviour, consider refactoring this if possible.
	objectComponents[itemId] -= 1;

	AddCommand(this, "Call", this, target, nil, nil, nil, "CommandEvaluateBuild");
	AcquireComponents(objectComponents, availableInventorySlotsCount, 200);

	Message(GetNeededMatStr(target), this);
}

public func CommandAcquireObjects(object source, id itemId, int distance)
{
	if (GetAvailableInventorySlotCount() <= 0 && FindContents(itemId, this) != nil)
	{
		return false;
	}

	var items = FindObjects(
		Find_Not(Find_Container(this)),
		Find_ID(itemId),
		Find_Distance(distance),
		Sort_Distance());

	var itemCount = GetLength(items);
	if (itemCount <= 0)
	{
		return false;
	}

	AddCommand(this, "Get", items[0]);

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

private func GetRequiredComponents(object target)
{
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

	return objectComponents;
}

private func GetMaxRequiredComponents(id targetId)
{
	var objectComponents = {};
	var counter = 0;
	var componentId;
	while (componentId = GetComponent(nil, counter++, nil, targetId))
	{
		var neededAmount = GetComponent(componentId, nil, nil, targetId);
		objectComponents[componentId] = neededAmount;
	}

	return objectComponents;
}

private func AcquireComponents(map objectComponents, int availableInventorySlotsCount, int distance)
{
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
			AddCommand(this, "Call", this, componentId, distance, nil, nil, "CommandAcquireObjects");
		}
	}
}
