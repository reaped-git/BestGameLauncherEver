#pragma once

#include <vector>
#include <utility>
#include <stdexcept>

namespace Battleship {
	class Ship
	{
	public:
		// публичные: переопределение типом
		using CoordinatesType = std::vector<std::pair<int, int>>;
		using HitsType = std::vector<bool>;

		enum class ShotResult
		{
			eMiss = 0,
			eHit = 1,
			eSunk = 2,
			eAlreadyShot = 3
		};

	public:
		// конструкторы и деконструктор
		Ship(int size, std::pair<int, int> startCoord, bool isHorizontal);
		~Ship() = default;

		// публичные методы
		bool IsSunk() const;
		bool TakeHit(std::pair<int, int> coord);

		// геттеры
		const CoordinatesType& GetCoordinates() const { return m_coordinates; }
		int GetSize() const { return m_size; }
		bool GetIsHorizontal() const { return m_isHorizontal; }

	private:
		// приватные переменные
		int m_size;
		CoordinatesType m_coordinates;
		HitsType m_hits;
		bool m_isHorizontal;
	};
}