#pragma once

#include "Player.hpp"
#include "GameBoard.hpp"
#include <iostream>

namespace Battleship {
	class HumanPlayer : public Player
	{
	public:
		// конструкторы и деконструктор
		HumanPlayer(std::string name, int boardSize);
		~HumanPlayer() override = default;

		// публичные методы
		void PlaceShips() override;
		MoveType MakeMove() override;

		static const int MAX_ATTEMPTS = 100;
		GameBoard::ShipSizesType shipSizes;

	private:
		// приватные методы
		void DisplayBoardState();
		void ManualPlacement();
		void AutomaticPlacement();
		bool TryPlaceShip(int size, int row, int col, bool horizontal);
		int GetValidatedInput(const std::string& prompt, int minValue, int maxValue);
	};
}