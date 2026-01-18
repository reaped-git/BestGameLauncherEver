#include "AIPlayer.hpp"
#include <random>
#include <ctime>


namespace Battleship {
	AIPlayer::AIPlayer(std::string name, int boardSize)
		: Player(name, boardSize)
		, m_lastHit({ -1, -1 })
	{
		// Генерируем все возможные ходы
		for (int i = 0; i < boardSize; i++)
		{
			for (int j = 0; j < boardSize; j++)
			{
				m_allPossibleMoves.push_back({ i, j });
			}
		}

		// Перемешиваем ходы
		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(m_allPossibleMoves.begin(), m_allPossibleMoves.end(), gen);

		shipSizes = GameBoard::MakeShipSizes(GameBoard::DEFAULT_SHIP_CONFIG);
	}

	void AIPlayer::PlaceShips()
	{

		std::random_device rd;
		std::mt19937 gen(rd());

		for (int size : shipSizes)
		{
			bool placed = false;
			int attempts = 0;

			while (!placed && attempts < MAX_ATEMPTS) // Ограничение на попытки
			{
				int row = gen() % m_myBoard.GetSize();
				int col = gen() % m_myBoard.GetSize();
				bool horizontal = gen() % 2 == 0;

				Ship ship(size, { row, col }, horizontal);
				placed = m_myBoard.PlaceShip(ship);
				attempts++;
			}

			if (!placed)
			{
				// Если не удалось разместить, пробуем альтернативный алгоритм
				placed = PlaceShipAlternative(size, gen);
			}
		}
	}

	bool AIPlayer::PlaceShipAlternative(int size, std::mt19937& gen)
	{
		// Альтернативный алгоритм размещения корабля
		for (int attempt = 0; attempt < MAX_ATEMPTS; attempt++)
		{
			for (bool horizontal : {true, false})
			{
				for (int row = 0; row < m_myBoard.GetSize(); row++)
				{
					for (int col = 0; col < m_myBoard.GetSize(); col++)
					{
						Ship ship(size, { row, col }, horizontal);
						if (m_myBoard.PlaceShip(ship))
						{
							return true;
						}
					}
				}
			}
		}
		return false;
	}

	Player::MoveType AIPlayer::MakeMove()
	{
		// Если есть потенциальные цели, стреляем в них
		if (!m_potentialTargets.empty())
		{
			MoveType target = m_potentialTargets.back();
			m_potentialTargets.pop_back();
			return target;
		}

		// Иначе стреляем в случайную клетку из доступных
		if (!m_allPossibleMoves.empty())
		{
			MoveType move = m_allPossibleMoves.back();
			m_allPossibleMoves.pop_back();
			return move;
		}

		// Запасной вариант
		return { 0, 0 };
	}

	void AIPlayer::UpdateAIState(Ship::ShotResult result, MoveType coord)
	{
		if (result == Ship::ShotResult::eHit)
		{
			m_lastHit = coord;

			// Добавляем соседние клетки как потенциальные цели
			int directions[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
			for (auto& dir : directions)
			{
				int newRow = coord.first + dir[0];
				int newCol = coord.second + dir[1];

				if (newRow >= 0 && newRow < m_myBoard.GetSize() &&
					newCol >= 0 && newCol < m_myBoard.GetSize())
				{
					MoveType newTarget = { newRow, newCol };

					// Проверяем, что этот ход еще возможен
					auto it = std::find(m_allPossibleMoves.begin(), m_allPossibleMoves.end(), newTarget);
					if (it != m_allPossibleMoves.end())
					{
						m_potentialTargets.push_back(newTarget);
						m_allPossibleMoves.erase(it);
					}
				}
			}
		}
		else if (result == Ship::ShotResult::eSunk)
		{
			// Очищаем потенциальные цели при потоплении корабля
			m_potentialTargets.clear();
			m_lastHit = { -1, -1 };
		}
	}
}