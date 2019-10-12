#pragma once

class Player;

// Command pattern
class IInputCommand
{
public:
	virtual ~IInputCommand() {}
	virtual void Execute(Player& player) = 0;
	virtual void Undo(Player& player) = 0;
};

class MoveForwardCommand : public IInputCommand
{
public:
	virtual ~MoveForwardCommand() override {};
	virtual void Execute(Player& player) override;
	virtual void Undo(Player& player) override;
};

class MoveBackwardCommand : public IInputCommand
{
public:
	virtual ~MoveBackwardCommand() override {};
	virtual void Execute(Player& player) override;
	virtual void Undo(Player& player) override;
};

class TurnRightCommand : public IInputCommand
{
public:
	virtual ~TurnRightCommand() override {};
	virtual void Execute(Player& player) override;
	virtual void Undo(Player& player) override;
};

class TurnLeftCommand : public IInputCommand
{
public:
	virtual ~TurnLeftCommand() override {};
	virtual void Execute(Player& player) override;
	virtual void Undo(Player& player) override;
};