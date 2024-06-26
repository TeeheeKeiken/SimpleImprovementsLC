/*-- Resource Info --*/

#strict 3
#appendto *

static ISB_SI_PlayerResourceMap;

global func ISB_SI_GetResourceColorModulation(id itemId)
{
	return _inherited(itemId);
}

public func Collection2(object item)
{
	if (item->~ISB_SI_IsResource())
	{
		IncreaseResourceCount(GetOwner(), item);
	}

	return _inherited(item);
}

protected func Ejection(object obj)
{
	if (obj->~ISB_SI_IsResource())
	{
		DecreaseResourceCount(GetOwner(), obj);
	}

	return _inherited(obj);
}

protected func Destruction()
{
	if (this->~ISB_SI_IsResource())
	{
		DecreaseResourceCount(GetOwner(Contained()), this);
	}

	return _inherited();
}

protected func ContentsDestruction(object obj)
{
	if (obj->~ISB_SI_IsResource())
	{
		DecreaseResourceCount(GetOwner(), obj);
	}

	return _inherited(obj);
}

private func IncreaseResourceCount(int playerId, object item)
{
	if (playerId == NO_OWNER)
	{
		return;
	}

	if (ISB_SI_PlayerResourceMap == nil)
	{
		ISB_SI_PlayerResourceMap = {};
	}

	var playerResourceArrays = ISB_SI_PlayerResourceMap[playerId] ?? {};

	var itemId = GetID(item);
	var resourceArray = playerResourceArrays[itemId] ?? {};

	if (GetIndexOf(item, GetValues(resourceArray)) >= 0)
	{
		return;
	}

	resourceArray[item] = 1;
	playerResourceArrays[itemId] = resourceArray;
	ISB_SI_PlayerResourceMap[playerId] = playerResourceArrays;

	UpdateResourceDisplay(playerId);
}

private func DecreaseResourceCount(int playerId, object item)
{
	if (playerId == NO_OWNER || ISB_SI_PlayerResourceMap == nil)
	{
		return;
	}

	var playerResourceArrays = ISB_SI_PlayerResourceMap[playerId];

	if (playerResourceArrays == nil)
	{
		return;
	}

	var itemId = GetID(item);
	var resourceArray = playerResourceArrays[itemId];

	if (resourceArray != nil)
	{
		resourceArray[item] = nil;
		playerResourceArrays[itemId] = resourceArray;
		ISB_SI_PlayerResourceMap[playerId] = playerResourceArrays;
	}

	UpdateResourceDisplay(playerId);
}

private func UpdateResourceDisplay(int playerId)
{
	var playerCount = GetPlayerCount(C4PT_User);
	var friendlyPlayerIds = [];
	for (var i = 0; i < playerCount; i++)
	{
		 var otherPlayerId = GetPlayerByIndex(i, C4PT_User);
		 if (!Hostile(playerId, otherPlayerId, false))
		 {
			 friendlyPlayerIds[] = otherPlayerId;
		 }
	}

	var resourceInfoMap = {};
	for (var friendlyPlayerId in friendlyPlayerIds)
	{
		var playerResourceMaps = ISB_SI_PlayerResourceMap[friendlyPlayerId];

		if (playerResourceMaps != nil)
		{
			for (var itemId, itemMap in playerResourceMaps)
			{
				var count = 0;
				for (var value in GetValues(itemMap))
				{
					if (value != nil)
					{
						count += value;
					}
				}

				var totalCount = resourceInfoMap[itemId] ?? 0;
				resourceInfoMap[itemId] = totalCount + count;
			}
		}
	}
	
	var message = "@";
	var isFirstItem = true;
	for (var itemId, count in resourceInfoMap)
	{
		if (!isFirstItem)
		{
			message ..= " ";
		}

		var format = "%s";
		
		var color = ISB_SI_GetResourceColorModulation(itemId);
		if (count > 0)
		{
			format ..= " %d";
		}
		else
		{
			color = GetBlendedColor(color ?? 0xffffff, 0x646464);
			format ..= " <c 8a8a8a>%d</c>";
		}

		var icon;
		if (color == nil)
		{
			icon = Format("{{%i}}", itemId);
		}
		else
		{
			icon = Format("<c %x>{{%i}}</c>", color, itemId);
		}

		message ..= Format(format, icon, count);
		isFirstItem = false;
	}

	for (var friendlyPlayerId in friendlyPlayerIds)
	{
		CustomMessage(
			message,
			nil,
			friendlyPlayerId,
			-300 - (40 * GetLength(GetKeys(resourceInfoMap))),
			24,
			0xffffff,
			nil,
			"Portrait",
			MSG_Right);
	}
}

private func GetBlendedColor(int colorA, int colorB)
{
	var redA = colorA >> 16;
	var greenA = colorA & 0x00ff00 >> 8;
	var blueA = colorA & 0x0000ff;

	var redB = colorB >> 16;
	var greenB = colorB & 0x00ff00 >> 8;
	var blueB = colorB & 0x0000ff;

	var blendedRed = (redA + redB) / 2;
	var blendedGreen = (greenA + greenB) / 2;
	var blendedBlue = (blueA + blueB) / 2;

	return RGB(blendedRed, blendedGreen, blendedBlue);
}
