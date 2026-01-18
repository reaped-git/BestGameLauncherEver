#include "GameBoard.hpp"
#include <algorithm>


namespace Battleship {
	GameBoard::GameBoard(int size)
		: m_size(size)
	{
	}

	bool GameBoard::PlaceShip(const Ship& ship)
	{
		// Проверка на выход за границы
		for (const auto& coord : ship.GetCoordinates())
		{
			if (coord.first < 0 || coord.first >= m_size ||
				coord.second < 0 || coord.second >= m_size)
			{
				return false;
			}
		}

		// Проверка на пересечение с другими кораблями
		for (const auto& existingShip : m_ships)
		{
			for (const auto& existingCoord : existingShip.GetCoordinates())
			{
				for (const auto& newCoord : ship.GetCoordinates())
				{
					// Проверка самой координаты и соседних клеток
					for (int i = -1; i <= 1; i++)
					{
						for (int j = -1; j <= 1; j++)
						{
							if (existingCoord.first + i == newCoord.first &&
								existingCoord.second + j == newCoord.second)
							{
								return false;
							}
						}
					}
				}
			}
		}

		m_ships.push_back(ship);
		return true;
	}

	Ship::ShotResult GameBoard::ReceiveShot(std::pair<int, int> coord)
	{
		// Проверка на повторный выстрел
		if (m_shots.find(coord) != m_shots.end())
		{
			return Ship::ShotResult::eAlreadyShot;
		}

		m_shots.insert(coord);

		// Проверка попадания
		for (auto& ship : m_ships)
		{
			if (ship.TakeHit(coord))
			{
				if (ship.IsSunk())
				{
					return Ship::ShotResult::eSunk;
				}
				return Ship::ShotResult::eHit;
			}
		}

		m_misses.push_back(coord);
		return Ship::ShotResult::eMiss;
	}

	bool GameBoard::IsAllShipsSunk() const
	{
		for (const auto& ship : m_ships)
		{
			if (!ship.IsSunk())
			{
				return false;
			}
		}
		return true;
	}

	GameBoard::BoardStateType GameBoard::GetVisibleState(bool forOwner) const
	{
		BoardStateType state(m_size, std::vector<char>(m_size, '.'));

		// Всегда показываем промахи
		for (const auto& miss : m_misses)
		{
			state[miss.first][miss.second] = 'O';
		}

		// Всегда показываем попадания
		for (const auto& ship : m_ships)
		{
			for (const auto& coord : ship.GetCoordinates())
			{
				if (m_shots.find(coord) != m_shots.end())
				{
					state[coord.first][coord.second] = 'X';
				}
			}
		}

		// Показываем корабли ТОЛЬКО если это поле владельца
		if (forOwner)
		{
			for (const auto& ship : m_ships)
			{
				for (const auto& coord : ship.GetCoordinates())
				{
					// Показываем неподбитые части кораблей
					if (m_shots.find(coord) == m_shots.end())
					{
						state[coord.first][coord.second] = 'S';
					}
				}
			}
		}

		return state;
	}

	// Кастомные размеры кораблей в зависимости от размера поля
	GameBoard::ShipSizesType GameBoard::MakeShipSizes(std::array < std::pair<int, int>, 4> shipConfig)
	{
		// Формируем вектор размеров кораблей
		ShipSizesType sizes;
		for (const auto& pair : shipConfig)
		{
			int shipLength = pair.first;
			int shipCount = pair.second;

			for (int i = 0; i < shipCount; i++)
			{
				sizes.push_back(shipLength);
			}
		}

		return sizes;
	}
}