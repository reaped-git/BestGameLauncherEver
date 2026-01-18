#pragma once

#include "Player.hpp"
#include "GameBoard.hpp"
#include <vector>
#include <algorithm>
#include <random>

namespace Battleship {
	class AIPlayer : public Player
	{
	public:
		static const int MAX_ATEMPTS = 100;

		// публичные: переопределение типом
		using TargetsType = std::vector<MoveType>;
		using MovesType = std::vector<MoveType>;

	public:
		// конструкторы и деконструктор
		AIPlayer(std::string name, int boardSize);
		~AIPlayer() override = default;

		// публичные методы
		void PlaceShips() override;
		MoveType MakeMove() override;
		void UpdateAIState(Ship::ShotResult result, MoveType coord);
		bool PlaceShipAlternative(int size, std::mt19937& gen);

	private:
		// приватные переменные
		MoveType m_lastHit;
		TargetsType m_potentialTargets;
		MovesType m_allPossibleMoves;
		GameBoard::ShipSizesType shipSizes;
	};
}