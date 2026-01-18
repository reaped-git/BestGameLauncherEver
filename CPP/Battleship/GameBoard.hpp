#pragma once

#include <vector>
#include <set>
#include <utility>
#include <string>
#include <array>
#include "Ship.hpp"

namespace Battleship {
	class GameBoard
	{
	public:
		static const int DEFAULT_BOARD_SIZE = 10;
		static constexpr std::array<std::pair<int, int>, 4> DEFAULT_SHIP_CONFIG = {
			{{ 4, 1 }, { 3, 2 }, { 2, 3 }, { 1, 4 }}
		};

		// публичные: переопределение типом
		using ShipsType = std::vector<Ship>;
		using ShotsType = std::set<std::pair<int, int>>;
		using MissesType = std::vector<std::pair<int, int>>;
		using BoardStateType = std::vector<std::vector<char>>;
		using ShipSizesType = std::vector<int>;

	public:
		// конструкторы и деконструктор
		GameBoard(int size);
		~GameBoard() = default;

		// публичные методы
		bool PlaceShip(const Ship& ship);
		Ship::ShotResult ReceiveShot(std::pair<int, int> coord);
		bool IsAllShipsSunk() const;
		BoardStateType GetVisibleState(bool forOwner) const;
		static ShipSizesType MakeShipSizes(std::array < std::pair<int, int>, 4> shipConfig);

		// геттеры
		int GetSize() const { return m_size; }
		const ShipsType& GetShips() const { return m_ships; }

	private:
		// приватные переменные
		int m_size;
		ShipsType m_ships;
		ShotsType m_shots;
		MissesType m_misses;
	};
}