/*-- Resource Info Collectible --*/

#strict 3

/* Primary Resources */
#appendto METL
#appendto PSTC
#appendto CRYS

/* Secondary Resources */
#appendto ORE1
#appendto ROCK
#appendto ERTH
#appendto ASHS

/* Cells */
#appendto CELL
#appendto FCLL

/* Kits & Co. */
#appendto CNKT
#appendto SPCT
#appendto PIKT
#appendto WIRO

public func ISB_IS_IsResource()
{
	return true;
}
