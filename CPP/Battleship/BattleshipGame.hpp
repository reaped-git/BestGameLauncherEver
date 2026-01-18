#pragma once

#include "Player.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"
#include "UserInterface.hpp"
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <cstdio>
#include <iostream>
#include <limits>

namespace Battleship {
	// Предварительное объявление
	class UserInterface;

	class GameManager
	{
	public:
		// конструкторы и деконструктор
		GameManager(int boardSize);
		~GameManager();

		// публичные методы
		void SetupGame();
		void StartGame();
		void RunGameLoop();
		void SwitchTurn();
		void DisplayGameState();

		// геттеры
		Player* GetCurrentPlayer() const { return m_currentPlayer; }
		Player* GetPlayer1() const { return m_player1; }
		Player* GetPlayer2() const { return m_player2; }

	private:
		// приватные переменные
		Player* m_player1;
		Player* m_player2;
		Player* m_currentPlayer;
		bool m_gameOver;
		UserInterface* m_userInterface;
	};

}