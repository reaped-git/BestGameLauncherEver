#include "HumanPlayer.hpp"
#include <random>
#include <algorithm>

using namespace std;

namespace Battleship {
	HumanPlayer::HumanPlayer(std::string name, int boardSize)
		: Player(name, boardSize)
	{
		shipSizes = GameBoard::MakeShipSizes(GameBoard::DEFAULT_SHIP_CONFIG);
	}

	void HumanPlayer::PlaceShips()
	{
		std::cout << m_name << ", выберите способ расстановки кораблей:\n";
		std::cout << "1 - Ручная расстановка\n";
		std::cout << "2 - Автоматическая расстановка\n";

		int choice;
		while (true)
		{
			std::cout << "Ваш выбор (1 или 2): ";
			std::cin >> choice;

			if (choice == 1 || choice == 2)
			{
				break;
			}
			std::cout << "Неверный выбор. Попробуйте снова.\n";
		}

		if (choice == 1)
		{
			ManualPlacement();
		}
		else
		{
			AutomaticPlacement();
		}
	}

	void HumanPlayer::ManualPlacement()
	{
		std::cout << "\n=== РУЧНАЯ РАССТАНОВКА КОРАБЛЕЙ ===\n";

		for (int size : shipSizes)
		{
			bool placed = false;
			while (!placed)
			{
				DisplayBoardState();
				std::cout << "Разместите корабль размером " << size << "\n";

				int row, col, orientation;

				// Валидация координат
				row = GetValidatedInput("Введите номер ряда (0-" + std::to_string(m_myBoard.GetSize() - 1) + "): ",
					0, m_myBoard.GetSize() - 1);
				col = GetValidatedInput("Введите номер столбца (0-" + std::to_string(m_myBoard.GetSize() - 1) + "): ",
					0, m_myBoard.GetSize() - 1);

				// Валидация ориентации
				orientation = GetValidatedInput("Ориентация (0 - горизонтально, 1 - вертикально): ", 0, 1);

				placed = TryPlaceShip(size, row, col, orientation == 0);

				if (!placed)
				{
					std::cout << "Невозможно разместить корабль здесь. Попробуйте снова.\n";
				}
			}
		}

		std::cout << "Все корабли успешно расставлены!\n";
		DisplayBoardState();
	}

	void HumanPlayer::AutomaticPlacement()
	{
		std::cout << "\n=== АВТОМАТИЧЕСКАЯ РАССТАНОВКА КОРАБЛЕЙ ===\n";

		std::random_device rd;
		std::mt19937 gen(rd());

		for (int size : shipSizes)
		{
			bool placed = false;
			int attempts = 0;

			while (!placed && attempts < MAX_ATTEMPTS) // Ограничение на попытки
			{
				int row = gen() % m_myBoard.GetSize();
				int col = gen() % m_myBoard.GetSize();
				bool horizontal = gen() % 2 == 0;

				placed = TryPlaceShip(size, row, col, horizontal);
				attempts++;
			}

			if (!placed)
			{
				std::cout << "Не удалось автоматически разместить корабль размером " << size << ". Попробуйте ручную расстановку.\n";
				ManualPlacement();
				return;
			}
		}

		std::cout << "Все корабли успешно расставлены автоматически!\n";
		DisplayBoardState();
	}

	bool HumanPlayer::TryPlaceShip(int size, int row, int col, bool horizontal)
	{
		Ship ship(size, { row, col }, horizontal);
		return m_myBoard.PlaceShip(ship);
	}

	Player::MoveType HumanPlayer::MakeMove()
	{
		std::cout << m_name << ", ваш ход:\n";

		int row = GetValidatedInput("Введите номер ряда (0-" + std::to_string(m_myBoard.GetSize() - 1) + "): ",
			0, m_myBoard.GetSize() - 1);
		int col = GetValidatedInput("Введите номер столбца (0-" + std::to_string(m_myBoard.GetSize() - 1) + "): ",
			0, m_myBoard.GetSize() - 1);

		return { row, col };
	}

	void HumanPlayer::DisplayBoardState()
	{
		// При расстановке показываем корабли (forOwner = true)
		auto state = m_myBoard.GetVisibleState(true);
		std::cout << "Ваше поле:\n";

		// Вывод номеров столбцов
		std::cout << "  ";
		for (int j = 0; j < m_myBoard.GetSize(); j++)
		{
			std::cout << j << " ";
		}
		std::cout << "\n";

		for (int i = 0; i < m_myBoard.GetSize(); i++)
		{
			std::cout << i << " ";
			for (int j = 0; j < m_myBoard.GetSize(); j++)
			{
				std::cout << state[i][j] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}

	int HumanPlayer::GetValidatedInput(const std::string& prompt, int minValue, int maxValue)
	{
		int value;
		while (true)
		{
			std::cout << prompt;
			std::cin >> value;

			if (std::cin.fail())
			{
				std::cin.clear(); // Сбрасываем флаг ошибки
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер
				std::cout << "Ошибка: введите целое число!\n";
			}
			else if (value < minValue || value > maxValue)
			{
				std::cout << "Ошибка: число должно быть в диапазоне от " << minValue << " до " << maxValue << "!\n";
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else
			{
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер
				break;
			}
		}
		return value;
	}
}