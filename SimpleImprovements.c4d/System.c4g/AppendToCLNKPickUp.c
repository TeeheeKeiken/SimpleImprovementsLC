/*-- Pick Up Menu / Swap Menu --*/

#strict 3
#appendto CLNK

local _pickUpIdMap;
local _itemIdSlatedForPickUp;
local _itemSlatedForPickUp;
local _pickUpClosestItemInRange;
local _pickUpRepetitions;

local _previousPickUpMenuItemSelection;

public func RejectCollect(id itemId, object item)
{
	if (_itemIdSlatedForPickUp != nil)
	{
		if (_pickUpClosestItemInRange)
		{
			if (itemId != _itemIdSlatedForPickUp)
			{
				return true;
			}
		}
		else if (_itemSlatedForPickUp != item)
		{
			return true;
		}
	}

	return _inherited(itemId, item);
}

protected func Initialize()
{
	_pickUpRepetitions = 0;

	_inherited();
}

private func GetItemsInRangeMappedById()
{
	var offsetY = GetDefOffset(GetID(), 1);
	var collectionLeft = GetDefCollection(GetID(), 0);
	var collectionTop = GetDefCollection(GetID(), 1) + offsetY;
	var collectionWidth = GetDefCollection(GetID(), 2);
	var collectionHeight = GetDefCollection(GetID(), 3) - offsetY;

	var itemsInRange = FindObjects(
		Find_Exclude(this),
		Find_InRect(collectionLeft, collectionTop, collectionWidth, collectionHeight),
		Find_OCF(OCF_Collectible),
		Find_NoContainer());

	_pickUpIdMap = {};
	DebugLog("Cleared _pickUpIdMap.");
	for (var item in itemsInRange)
	{
		if (item->~RejectEntrance(this))
		{
			continue;
		}
		var items = _pickUpIdMap[GetID(item)] ?? [];
		items[] = item;
		_pickUpIdMap[GetID(item)] = items;
	}

	return GetLength(_pickUpIdMap) > 0;
}

private func IsPickUpPossible()
{
	return !Contained() && GetItemsInRangeMappedById();
}

private func DropItem(id itemId)
{
	ShiftContents(this, false, itemId, false);
	AppendCommand(this, "Drop");
}

private func SwapItem(id itemId)
{
	DropItem(itemId);
	return AppendCommand(this, "Call", this, _pickUpClosestItemInRange, nil, _itemSlatedForPickUp, nil, "CommandPickUpItem");
}

private func ExecuteDropItem(id itemId)
{
	return SwapItem(itemId);
}

private func CreateDropMenu()
{
	var pickupMenu = CreateMenu(_itemIdSlatedForPickUp, this, this, C4MN_Extra_None, "$DropMenuNothingToDrop$", nil, C4MN_Style_Normal, false, DPIS);

	var dropIdMap = {};
	for (var i = 0; i < ContentsCount(); i++)
	{
		var item = Contents(i);
		var id = GetID(item);
		var items = dropIdMap[id] ?? [];
		items[] = item;
		dropIdMap[id] = items;
	}

	for (var id in GetKeys(dropIdMap))
	{
		var items = dropIdMap[id];
		if (id != _itemIdSlatedForPickUp && IsSpecialItem(_itemSlatedForPickUp) == IsSpecialItem(items[0]))
		{
			AddMenuItem("$DropMenuEntryDropPs$", "ExecuteDropItem", id, this, GetLength(items));
		}
	}
}

private func TryGetSingleOtherEquivalentItemId(object pickUpItem, id& singleOtherEquivalentItemId)
{
	var isPickUpItemSpecial = IsSpecialItem(pickUpItem);
	var pickUpItemId = GetID(pickUpItem);

	var item;
	var idMap = { };
	for (var i = 0; item = Contents(i); i++)
	{
		var itemId = GetID(item);
		if (itemId != pickUpItemId &&
			IsSpecialItem(item) == isPickUpItemSpecial)
		{
			idMap[itemId] = itemId;
		}
	}

	var idMapKeysLength = GetLength(GetKeys(idMap));
	singleOtherEquivalentItemId = GetValues(idMap)[0];
	if (idMapKeysLength == 1)
	{
		return true;
	}

	if (idMapKeysLength == 0)
	{
		return true;
	}

	return false;
}

private func DeactivatePickUpRepetitions()
{
	_itemIdSlatedForPickUp = nil;
	_itemSlatedForPickUp = nil;
	_pickUpRepetitions = 0;
	_pickUpClosestItemInRange = false;
}

private func PickUpClosestItem(id itemId, bool skipDropMenu)
{
	var closestItem =
	{
		Ref = nil,
		Distance = nil
	};

	var items = nil;
	var tries = 0;
	while (items == nil)
	{
		var items = _pickUpIdMap[itemId];

		if (items == nil)
		{
			DebugLog("Found no valid items to pick up for %i, retrying. (Try: %d)", itemId, tries + 1);
			RefreshPickUpMenu();
		}

		if (++tries >= 3)
		{
			DebugLog("Abort picking up items: Reached maximum tries.");
			return false;
		}
	}

	for (var item in items)
	{
		var distance = ObjectDistance(item);
		if (closestItem.Distance == nil || closestItem.Distance > distance)
		{
			closestItem.Ref = item;
			closestItem.Distance = distance;
		}
	}

	PickUpItem(closestItem.Ref, skipDropMenu);
	return true;
}

private func PickUpItem(object item, bool skipDropMenu)
{	
	var itemId = GetID(item);
	var singleOtherEquivalentItemId = nil;

	DebugLog("Trying to pick up %v. (Repetition: %d)", item, _pickUpRepetitions);
	if ((MaxContentsCount() == 1 && GetMaxSpecialCount(0, 0) == nil) ||
		!RejectCollect(itemId, item) ||
		TryGetSingleOtherEquivalentItemId(item, singleOtherEquivalentItemId) ||
		skipDropMenu)
	{
		if (singleOtherEquivalentItemId != nil)
		{
			_itemIdSlatedForPickUp = itemId;
			_itemSlatedForPickUp = item;
			_pickUpClosestItemInRange = true;
			return SwapItem(singleOtherEquivalentItemId);
		}
		return AppendCommand(this, "Get", item);
	}
	_itemIdSlatedForPickUp = itemId;
	_itemSlatedForPickUp = item;
	_pickUpClosestItemInRange = true;
	RememberSelectedPickUpMenuItem();
	return CreateDropMenu();
}

private func RememberSelectedPickUpMenuItem()
{
	var selectedMenuItemId = GetMenuSelection();
	_previousPickUpMenuItemSelection =
	{
		Index = GetMenuSelection(),
		ItemId = GetKeys(_pickUpIdMap)[selectedMenuItemId]
	};
}

private func SelectPickUpMenuItem(int oldSelectedMenuItemId, id selectedItemId)
{
	var pickUpIds = GetKeys(_pickUpIdMap);
	var newSelectedMenuItemId = GetIndexOf(selectedItemId, pickUpIds);

	if (newSelectedMenuItemId < 0)
	{
		newSelectedMenuItemId = Min(oldSelectedMenuItemId, GetLength(pickUpIds) - 1);
	}

	SelectMenuItem(newSelectedMenuItemId);
}

private func SelectRememberedPickUpMenuItem()
{
	SelectPickUpMenuItem(_previousPickUpMenuItemSelection.Index, _previousPickUpMenuItemSelection.ItemId);
	_previousPickUpMenuItemSelection = nil;
}

private func RefreshPickUpMenu()
{
	var oldSelectedMenuItemId = GetMenuSelection();
	var selectedItemId = GetKeys(_pickUpIdMap)[oldSelectedMenuItemId];
	GetItemsInRangeMappedById();
	ClearMenuItems();

	for (var items in GetValues(_pickUpIdMap))
	{
		var itemsCount = GetLength(items);
		AddMenuItem("$PickUpMenuEntryPickUpPs$", "ExecutePickUpClosestItem", GetID(items[0]), this, itemsCount, itemsCount);
	}

	SelectPickUpMenuItem(oldSelectedMenuItemId, selectedItemId);
}

private func GetMaxCount(bool isSpecial)
{
	if (isSpecial)
	{
		return GetMaxSpecialCount(0, true);
	}
		
	return MaxContentsCount();
}

private func ExecutePickUpClosestItem(id itemId, int count, bool specialInput)
{
	DebugLog("Got input to pick up %i.", itemId);
	if (specialInput)
	{
		var maxCount = GetMaxCount(IsSpecialItem(_pickUpIdMap[itemId][0]));
		var alreadyHeldCount = ContentsCount(itemId, this);
		_pickUpRepetitions = Min(count, maxCount - alreadyHeldCount);
		DebugLog("Picking up %d.", _pickUpRepetitions);
	}

	return PickUpClosestItem(itemId, specialInput);
}

private func CreatePickUpMenu()
{
	var pickupMenu = CreateMenu(CXIV, this, this, C4MN_Extra_None, "$PickUpMenuNothingToPickUp$", nil, C4MN_Style_Normal, true, PUIS);
	RefreshPickUpMenu();
}

protected func ContextPickUp()
{
	[$ContextPickUp$|Image=CXIV|Condition=IsPickUpPossible]

	CreatePickUpMenu();

	return true;
}

private func CommandPickUpItem(object source, bool pickUpClosestItem, _, object itemSlatedForPickUp)
{
	if (pickUpClosestItem)
	{
		GetItemsInRangeMappedById();
		PickUpClosestItem(_itemIdSlatedForPickUp);
		if (GetMenu() != PUIS && IsPickUpPossible() && _previousPickUpMenuItemSelection != nil)
		{
			CreatePickUpMenu();
			SelectRememberedPickUpMenuItem();
		}
	}
	else
	{
		PickUpItem(itemSlatedForPickUp, true);
	}
}

protected func Collection2()
{
	_inherited();

	if (_pickUpRepetitions > 0)
	{
		var singleOtherEquivalentItemId;
		if (_pickUpIdMap[_itemIdSlatedForPickUp] == nil ||
			!TryGetSingleOtherEquivalentItemId(_itemSlatedForPickUp, singleOtherEquivalentItemId) &&
			singleOtherEquivalentItemId == nil &&
			RejectCollect(_itemIdSlatedForPickUp, _itemSlatedForPickUp))
		{
			DebugLog("Exhausted all valid pick ups, aborting at %d remaining repititions.", _pickUpRepetitions);
			_pickUpRepetitions = 0;
		}
		else
		{
			DebugLog("Trying to pick up %i. (Repititions: %d)", _itemIdSlatedForPickUp, _pickUpRepetitions);
			if (PickUpClosestItem(_itemIdSlatedForPickUp, true))
			{
				_pickUpRepetitions--;
				DebugLog("Picked up %i. (Remaining repititions: %d)", _itemIdSlatedForPickUp, _pickUpRepetitions);
			}
		}
	}

	if (_pickUpRepetitions <= 0)
	{
		DeactivatePickUpRepetitions();
	}

	if (GetMenu() == PUIS)
	{
		RefreshPickUpMenu();
	}
}

protected func MenuQueryCancel(int selection, object menu)
{
	if (GetMenu() == DPIS)
	{
		CloseMenu();
		if (IsPickUpPossible() && _previousPickUpMenuItemSelection != nil)
		{
			CreatePickUpMenu();
			SelectRememberedPickUpMenuItem();
		}
		return true;
	}

	return _inherited();
}