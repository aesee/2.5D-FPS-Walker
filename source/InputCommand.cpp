#include "InputCommand.h"
#include "Player.h"

void MoveForwardCommand::Execute(Player& player)
{
	player.SetMoveStatus(1);
}

void MoveForwardCommand::Undo(Player& player)
{
	player.SetMoveStatus(0);
}

void MoveBackwardCommand::Execute(Player& player)
{
	player.SetMoveStatus(-1);
}

void MoveBackwardCommand::Undo(Player& player)
{
	player.SetMoveStatus(0);
}

void TurnRightCommand::Execute(Player& player)
{
	player.SetTurnStatus(1);
}

void TurnRightCommand::Undo(Player& player)
{
	player.SetTurnStatus(0);
}

void TurnLeftCommand::Execute(Player& player)
{
	player.SetTurnStatus(-1);
}

void TurnLeftCommand::Undo(Player& player)
{
	player.SetTurnStatus(0);
}
