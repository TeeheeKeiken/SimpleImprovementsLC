/*-- Pandoras Strike Build Drag & Drop --*/
/*-- TODO: Move to separate object folder --*/

#strict 3
#appendto WIEF nowarn

local _constructionId;

protected func ControlCommand(commandName, targetObject, targetX, targetY, targetObject2, data)
{
	// Handle drag & drop construction
	if (commandName == "Construct")
	{
		var selectedId = CastC4ID(data);
		if (selectedId->~RejectConstruction(targetX - GetX(), targetY - GetY(), this()))
		{
			return true;
		}
		else
		{
			_constructionId = selectedId;
			AddCommand(this, "Call", this, targetX, targetY, nil, nil, "ConstructAtLocation");
			AddCommand(this, "MoveTo", nil, targetX, targetY);
			return true;
		}
	}

	return _inherited(commandName, targetObject, targetX, targetY, targetObject2, data);
}

private func TryGetMatchingContainedConstructionItem(id structureId)
{
	if (structureId->~IsGandraGebaeude())
	{
		return FindContents(BARU);
	}
	else if (structureId->~IsIltoGebaeude())
	{
		return FindContents(BAUS);
	}
	else if (structureId->~IsOrkGebaeude())
	{
		return FindContents(KNBA);
	}
	else if (structureId->~IsSkisGebaeude())
	{
		return FindContents(BAKO);
	}
	else if (structureId->~IsSkisWurmGebaeude())
	{
		return FindContents(NALA);
	}

	return nil;
}

private func ConstructAtLocation(object source, int targetX, int targetY)
{
	var thisX = GetX();
	var thisY = GetY();
	var offsetX = targetX - thisX;
	var offsetY = targetY - thisY;

	if (Distance(thisX, thisY, targetX, targetY) <= 20)
	{
		var constructionItem = TryGetMatchingContainedConstructionItem(_constructionId);

		if (constructionItem != nil)
		{
			RemoveObject(constructionItem);
			var construction = CreateConstruction(_constructionId, offsetX, offsetY, GetOwner(), 1, true, true);
			_constructionId = nil;
			AddCommand(this, "Build", construction);
			return true;
		}
	}

	// TODO: Show error message?
	return false;
}
