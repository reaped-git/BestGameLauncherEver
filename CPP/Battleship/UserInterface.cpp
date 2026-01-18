#include "UserInterface.hpp"
#include "BattleshipGame.hpp"
#include <iostream>

namespace Battleship {
	UserInterface::UserInterface(GameManager* manager)
		: m_gameManager(manager)
	{
	}

	void UserInterface::DisplayBoards(Player* player)
	{
		std::cout << "========================================\n";
		std::cout << "         ТЕКУЩЕЕ СОСТОЯНИЕ\n";
		std::cout << "========================================\n";
		std::cout << "Игрок: " << player->GetName() << "\n\n";

		// Поле игрока - показываем корабли (forOwner = true)
		std::cout << "=== ВАШЕ ПОЛЕ ===\n";
		auto myState = player->GetMyBoard().GetVisibleState(true);

		// Вывод номеров столбцов
		std::cout << "  ";
		for (int j = 0; j < player->GetMyBoard().GetSize(); j++)
		{
			std::cout << j << " ";
		}
		std::cout << "\n";

		for (int i = 0; i < player->GetMyBoard().GetSize(); i++)
		{
			std::cout << i << " ";
			for (int j = 0; j < player->GetMyBoard().GetSize(); j++)
			{
				std::cout << myState[i][j] << " ";
			}
			std::cout << "\n";
		}

		// Поле противника - НЕ показываем корабли (forOwner = false)
		std::cout << "\n=== ПОЛЕ ПРОТИВНИКА ===\n";
		auto enemyState = player->GetEnemyBoard()->GetVisibleState(false);

		// Вывод номеров столбцов
		std::cout << "  ";
		for (int j = 0; j < player->GetEnemyBoard()->GetSize(); j++)
		{
			std::cout << j << " ";
		}
		std::cout << "\n";

		for (int i = 0; i < player->GetEnemyBoard()->GetSize(); i++)
		{
			std::cout << i << " ";
			for (int j = 0; j < player->GetEnemyBoard()->GetSize(); j++)
			{
				std::cout << enemyState[i][j] << " ";
			}
			std::cout << "\n";
		}

		// Легенда
		DisplayLegend();
		std::cout << "========================================\n";
	}

	void UserInterface::DisplayMessage(const std::string& message)
	{
		std::cout << message << "\n";
	}

	void UserInterface::ShowGameOver(const std::string& winnerName)
	{
		std::cout << "\n========================================\n";
		std::cout << "           ИГРА ОКОНЧЕНА!\n";
		std::cout << "========================================\n";
		std::cout << "*** " << winnerName << " ПОБЕДИЛ В ИГРЕ! ***\n";

		// Показываем все корабли противника в конце игры
		if (m_gameManager)
		{
			Player* currentPlayer = m_gameManager->GetCurrentPlayer();
			if (currentPlayer)
			{
				std::cout << "\n=== РАСКРЫТОЕ ПОЛЕ ПРОТИВНИКА ===\n";

				// Используем forOwner = true чтобы показать все корабли противника
				auto revealedState = currentPlayer->GetEnemyBoard()->GetVisibleState(true);

				// Вывод номеров столбцов
				std::cout << "  ";
				for (int j = 0; j < currentPlayer->GetEnemyBoard()->GetSize(); j++)
				{
					std::cout << j << " ";
				}
				std::cout << "\n";

				for (int i = 0; i < currentPlayer->GetEnemyBoard()->GetSize(); i++)
				{
					std::cout << i << " ";
					for (int j = 0; j < currentPlayer->GetEnemyBoard()->GetSize(); j++)
					{
						std::cout << revealedState[i][j] << " ";
					}
					std::cout << "\n";
				}
			}
		}
		std::cout << "========================================\n";
	}

	void UserInterface::DisplayLegend()
	{
		std::cout << "\n--- ЛЕГЕНДА ---\n";
		std::cout << "S - ваш корабль\n";
		std::cout << "X - попадание\n";
		std::cout << "O - промах\n";
		std::cout << ". - неизвестная клетка\n";
		std::cout << "----------------\n";
	}
}