/*-- More Knowledge --*/

#strict 3

global func AllKnowledge(playerId)
{
  // Structures
  SetPlrKnowledge(playerId, RTEG); // RTG
  return _inherited(playerId);
}
