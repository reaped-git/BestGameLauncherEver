#include "Ship.hpp"

namespace Battleship {
	Ship::Ship(int size, std::pair<int, int> startCoord, bool isHorizontal)
		: m_size(size)
		, m_isHorizontal(isHorizontal)
		, m_hits(size, false)
	{
		m_coordinates.clear();

		// Генерация всех координат корабля
		for (int i = 0; i < m_size; i++)
		{
			if (m_isHorizontal)
			{
				m_coordinates.push_back({ startCoord.first, startCoord.second + i });
			}
			else
			{
				m_coordinates.push_back({ startCoord.first + i, startCoord.second });
			}
		}
	}

	bool Ship::IsSunk() const
	{
		for (bool hit : m_hits)
		{
			if (!hit) return false;
		}
		return true;
	}

	bool Ship::TakeHit(std::pair<int, int> coord)
	{
		for (int i = 0; i < m_size; i++)
		{
			if (m_coordinates[i] == coord)
			{
				m_hits[i] = true;
				return true;
			}
		}
		return false;
	}
}