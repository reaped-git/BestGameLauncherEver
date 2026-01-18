#include "BattleshipGame.hpp"

namespace Battleship {

	GameManager::GameManager(int boardSize)
		: m_gameOver(false)
		, m_userInterface(new UserInterface(this))
	{
		m_player1 = new HumanPlayer("Игрок 1", boardSize);
		m_player2 = new AIPlayer("Компьютер", boardSize);

		m_currentPlayer = m_player1;

		// Устанавливаем ссылки на поля противников
		m_player1->SetEnemyBoard(&m_player2->GetMyBoard());
		m_player2->SetEnemyBoard(&m_player1->GetMyBoard());
	}

	GameManager::~GameManager()
	{
		delete m_player1;
		delete m_player2;
		delete m_userInterface;
	}

	void GameManager::StartGame() {

		while (true) {
			// Устанавливаем локаль для поддержки русского языка
			setlocale(LC_ALL, "Russian");

			std::cout << "========================================\n";
			std::cout << "           МОРСКОЙ БОЙ\n";
			std::cout << "========================================\n";
			std::cout << "Правила игры:\n";
			std::cout << "- Расставьте корабли на поле 10x10\n";
			std::cout << "- Корабли не могут касаться друг друга\n";
			std::cout << "- Стандартный набор кораблей:\n";
			std::cout << "  1 корабль - 4 клетки\n";
			std::cout << "  2 корабля - 3 клетки\n";
			std::cout << "  3 корабля - 2 клетки\n";
			std::cout << "  4 корабля - 1 клетка\n";
			std::cout << "- Вы можете выбрать ручную или автоматическую расстановку\n";
			std::cout << "========================================\n\n";

			try
			{
				const int BOARD_SIZE = 10;
				GameManager gameManager(BOARD_SIZE);

				// Настройка игры
				gameManager.SetupGame();

				std::cout << "\n========================================\n";
				std::cout << "           НАЧАЛО ИГРЫ!\n";
				std::cout << "========================================\n";
				std::cout << "После каждого хода будут показаны оба поля.\n";
				std::cout << "Нажмите Enter для продолжения...";
				std::cin.ignore();
				std::cin.get();

				// Запуск игрового цикла
				gameManager.RunGameLoop();

				std::cout << "\nЕще раз? (y/n)\n";

				char again;
				std::cin >> again;
				if (again == 'y') continue;
				else break;
			}
			catch (const std::exception& e)
			{
				std::cerr << "Произошла ошибка: " << e.what() << std::endl;
				return;
			}
			catch (...)
			{
				std::cerr << "Произошла неизвестная ошибка!" << std::endl;
				return;
			}
		}
	}

	void GameManager::SetupGame()
	{
		std::cout << "Расстановка кораблей для " << m_player1->GetName() << ":\n";
		m_player1->PlaceShips();

		std::cout << "Расстановка кораблей для " << m_player2->GetName() << ":\n";
		m_player2->PlaceShips();

		std::cout << "Игра начинается!\n";
	}

	void GameManager::RunGameLoop()
	{
		while (!m_gameOver)
		{
			AIPlayer* aiPlayer = dynamic_cast<AIPlayer*>(m_currentPlayer);

			// Показываем состояние после хода
			if (!aiPlayer)
			{
				DisplayGameState();
			}

			// Ход текущего игрока
			Player::MoveType move = m_currentPlayer->MakeMove();

			// Обработка выстрела
			GameBoard* enemyBoard = m_currentPlayer->GetEnemyBoard();
			Ship::ShotResult result = enemyBoard->ReceiveShot(move);

			// Обновление состояния ИИ если нужно
			if (aiPlayer)
			{
				aiPlayer->UpdateAIState(result, move);
			}

			// Отображение результата
			std::cout << m_currentPlayer->GetName() << " стреляет в ("
				<< move.first << ", " << move.second << ") - ";

			switch (result)
			{
			case Ship::ShotResult::eHit:
				std::cout << "ПОПАДАНИЕ!\n";
				break;
			case Ship::ShotResult::eSunk:
				std::cout << "КОРАБЛЬ ПОТОПЛЕН!\n";
				break;
			case Ship::ShotResult::eMiss:
				std::cout << "ПРОМАХ!\n";
				break;
			case Ship::ShotResult::eAlreadyShot:
				std::cout << "Уже стреляли сюда!\n";
				break;
			}

			// Показываем состояние после хода
			if (!aiPlayer)
			{
				DisplayGameState();
			}
			// Проверка окончания игры
			if (enemyBoard->IsAllShipsSunk())
			{
				m_gameOver = true;
				std::cout << "\n=== ИГРА ОКОНЧЕНА ===\n";
				std::cout << m_currentPlayer->GetName() << " ПОБЕДИЛ!\n";
				m_userInterface->ShowGameOver(m_currentPlayer->GetName());
				break;
			}

			// Смена хода если не попадание
			if (result != Ship::ShotResult::eHit && result != Ship::ShotResult::eSunk)
			{
				SwitchTurn();
			}

			// Пауза для удобства восприятия
			if (!aiPlayer)
			{
				std::cout << "Нажмите Enter для продолжения...";
				std::cin.ignore();
				std::cin.get();
			}
		}
	}

	void GameManager::SwitchTurn()
	{
		if (m_currentPlayer == m_player1)
		{
			m_currentPlayer = m_player2;
		}
		else
		{
			m_currentPlayer = m_player1;
		}
	}

	void GameManager::DisplayGameState()
	{
		std::cout << "\n";
		m_userInterface->DisplayBoards(m_currentPlayer);
	}

}