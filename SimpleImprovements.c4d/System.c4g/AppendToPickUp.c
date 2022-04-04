/*-- Pick Up Menu / Swap Menu on collectibles  --*/

#strict 3
#appendto *

private func IsPickUpThisPossible(object caller)
{
	return caller != nil &&
		!Contained(this) &&
		GetOCF() & OCF_Collectible && 
		!this->~RejectEntrance(caller);
}

protected func ContextPickUpThis(object caller)
{
	[$ContextPickUpThis$|Image=CXIV|Condition=IsPickUpThisPossible]

	caller._itemSlatedForPickUp = this;
	caller._itemIdSlatedForPickUp = GetID(this);

	var singleOtherEquivalentItemId;
	var hasOneOtherItemTypeOrLess = caller->~TryGetSingleOtherEquivalentItemId(this, singleOtherEquivalentItemId);
	if (hasOneOtherItemTypeOrLess)
	{
		AddCommand(caller, "Get", this);
	}
	else
	{
		caller->~CreateDropMenu();
	}
}