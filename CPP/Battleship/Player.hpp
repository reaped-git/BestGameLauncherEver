#pragma once

#include <string>
#include <utility>
#include "GameBoard.hpp"

namespace Battleship {
	class Player
	{
	public:
		// публичные: переопределение типом
		using MoveType = std::pair<int, int>;

	public:
		// конструкторы и деконструктор
		Player(std::string name, int boardSize);
		virtual ~Player() = default;

		// публичные методы
		virtual void PlaceShips() = 0;
		virtual MoveType MakeMove() = 0;

		// геттеры и сеттеры
		void SetEnemyBoard(GameBoard* board) { m_enemyBoard = board; }
		GameBoard& GetMyBoard() { return m_myBoard; }
		GameBoard* GetEnemyBoard() { return m_enemyBoard; }
		std::string GetName() const { return m_name; }

	protected:
		// защищенные переменные
		std::string m_name;
		GameBoard m_myBoard;
		GameBoard* m_enemyBoard;
	};
}