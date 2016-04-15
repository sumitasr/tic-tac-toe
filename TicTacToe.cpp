#include<iostream>
#include "TicTacToe.h"

#include <random>

using namespace std;

ComputerLevel* ComputerLevel::Create(int level)
{
	switch(level)
	{
		case 1:
			return new Beginer();
		case 2:
			return new Smart();
		default:
			return new Smart();
	}
}

Player* Player::Create(char player, char mark)
{
	if (player == 'H')
		return new Human(mark);
	else if (player == 'C')
		return new Computer(mark);
}


Computer::Computer(char mark) :Player("Computer", mark)
{
	setupLevel();
}

void Computer::setupLevel()
{
	int level = 2;
	cout << "Comuter level : \n 1. Beginer \n 2. Smart: \n Your Choice: - ";
	cin >> level;
	if (level < 1 || level > 2)
		cout<<"wrong input, taking default as Smart";

	m_level = ComputerLevel::Create(level);
}

Computer::~Computer()
{
	delete m_level;
}

int Computer::Play(Game* game)
{ 
	int selectedBox = m_level->selectBox(game);
	cout <<"\nComputer Selected "<<selectedBox<< " Box";
	return selectedBox;
}

int Beginer::selectBox(Game* game)
{
	//choose a random box.
	int selectedBox = rand()%9 + 1;
	
	while(!game->Validate(selectedBox))
	{
		selectedBox = rand()%9 + 1;
	}

	return selectedBox;
}

int Smart::selectBox(Game* game)
{ 
	int i = 1;
	char player1mark = game->getPlayer1Mark();
	char player2mark = game->getPlayer2Mark();

	//1. Check if Computer is winning in this move
	for (i = 1; i < 10; ++i)
	{
		if(game->Validate(i))
		{
			game->setMark(i, player2mark);
			if(game->IsGameover())
				return i;
			else
				game->setMark(i, i + '0');
		}
	}

	// 2. Check if other player is winning in next move, if so, stop him.
	for (i = 1; i < 10; ++i)
	{
		if(game->Validate(i))
		{
			game->setMark(i, player1mark);
			if(game->IsGameover())
				return i;
			else
				game->setMark(i, i + '0');
		}
	}

	//   3. check if a corner 1, 3, 7, 9 is free
	     if(game->Validate(1)) return 1;
		 if(game->Validate(3)) return 3;
		 if(game->Validate(7)) return 7;
		 if(game->Validate(9)) return 9;

    //   4. check if center is free.
		 if(game->Validate(5)) return 5;

	//   5. choose some other box
		 if(game->Validate(2)) return 2;
		 if(game->Validate(4)) return 4;
		 if(game->Validate(6)) return 6;
		 if(game->Validate(8)) return 8;
}

int Human::Play(Game* game)
{
	int selectedBox;
	cout << "Select a unfilled box from 1-9: ";
	cin >> selectedBox;

	while (!(game->Validate(selectedBox)))
	{
		cout << "Invalid Move, Enter new Option: ";
		cin >> selectedBox;
	}
	return selectedBox;
}

Game::~Game()
{
	delete player2;
}

bool Game::setupGame()
{
	char OtherplayerType;
	cout << "\nPlayer-1 : You"; 
	cout << "\nPlayer-2 : Human(Enter 'H')/Computer(Enter 'C') - ";
	cin >> OtherplayerType;
	if (OtherplayerType != 'H' && OtherplayerType != 'C')
	{
		cout << "Wrong choice of player to play with.\n You need to enter 'H' for Human and 'C' for Computer\n";
		return false;
	}

	player1 = Player::Create('H', PLAYER1_MARK);
	player2 = Player::Create(OtherplayerType, PLAYER2_MARK);
	
	int nextPlayer;
	cout << "\nSelect Player for First Chance : \n 1. Player-1(You) \n 2. Other player: \n Your Choice : - ";
	cin >> nextPlayer;
	if(nextPlayer == 1)
	{
		m_nextPlayer = player1;
	}
	else if (nextPlayer == 2)
	{
		m_nextPlayer = player2;
	}
	else
	{
		cout << "\nWrong choice. Going with default. Your turn first.\n";
		m_nextPlayer = player1;
	}

	for (int i = 0; i < 9; ++i)
		gameBoard[i] = '1'+ i;

	return true;
}

void Game::Display()
{
	cout << endl;
	int t = 0;
	while (t < 9)
	{
		if (t == 3 || t == 6)
		{
			cout << "\n\n";
		}
		cout << "  " << gameBoard[t];
		t++;
	}
	cout << endl << endl;
}

void Game::NextMove()
{
	int selectedBox;
	cout<<(m_nextPlayer == player1 ? "Your" : "Player-2(" + m_nextPlayer->getPlayerType() + ")") <<" Turn :\n";
	selectedBox = m_nextPlayer->Play(this);
	gameBoard[selectedBox - 1] = m_nextPlayer->getMark();	
}

void Game::Start()
{
	Display();
	while (!isBoardFull())
	{
		NextMove();
		Display();
		if (IsGameover())
		{
			cout<<(m_nextPlayer == player1 ? "You" : "Player-2(" + m_nextPlayer->getPlayerType() + ")") <<" won :\n";
			cout << "\n###########################################\n";
			break;
		}
		m_nextPlayer = m_nextPlayer == player1 ? player2 : player1;
	}
}

bool Game::Validate(int position)
{
	if ((gameBoard[position - 1] == '0') || (gameBoard[position - 1] == 'X'))
		return false;
	else
		return true;
}

bool Game::isBoardFull()
{
	for(int i = 1; i < 10; ++i)
		if(Validate(i)) return false;

	return true;
}

bool Game::IsGameover()
{
	if (((gameBoard[0] == gameBoard[1]) && (gameBoard[1] == gameBoard[2]))
		|| ((gameBoard[0] == gameBoard[3]) && (gameBoard[3] == gameBoard[6]))
		|| ((gameBoard[0] == gameBoard[4]) && (gameBoard[4] == gameBoard[8]))
		|| ((gameBoard[1] == gameBoard[4]) && (gameBoard[4] == gameBoard[7]))
		|| ((gameBoard[2] == gameBoard[5]) && (gameBoard[5] == gameBoard[8]))
		|| ((gameBoard[2] == gameBoard[4]) && (gameBoard[4] == gameBoard[6]))
		|| ((gameBoard[3] == gameBoard[4]) && (gameBoard[4] == gameBoard[5]))
		|| ((gameBoard[6] == gameBoard[7]) && (gameBoard[7] == gameBoard[8])))
	{
		return true;
	}
	else
		return false;
}

int main()
{
	char response;
	do
	{
		Game newGame;
		if(newGame.setupGame())
			newGame.Start();

		cout << "\n\nWanna play again(Y/y) : ";
		cin >> response;
	}
	while (response == 'Y' || response == 'y');

	getchar();
	return 0;
}
