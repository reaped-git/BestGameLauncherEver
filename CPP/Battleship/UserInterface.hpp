#pragma once

#include "Player.hpp"
#include <string>

namespace Battleship {
	// Предварительное объявление
	class GameManager;

	class UserInterface
	{
	public:
		// конструкторы и деконструктор
		UserInterface(GameManager* manager);
		~UserInterface() = default;

		// публичные методы
		void DisplayBoards(Player* player);
		void DisplayMessage(const std::string& message);
		void ShowGameOver(const std::string& winnerName);
		void DisplayLegend();

	private:
		// приватные переменные
		GameManager* m_gameManager;
	};
}
