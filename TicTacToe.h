#include<string>

class Game;

class ComputerLevel
{
	public:
	   static ComputerLevel* Create(int level);
	   virtual int selectBox(Game* game){ return 0; }
};

class Beginer : public ComputerLevel
{
public:
	int selectBox(Game* game);
};

class Smart: public ComputerLevel
{
public:
	int selectBox(Game* game);
};

class Player
{
public:
	Player(std::string p, char mark) :m_playerType(p), m_mark(mark){}
	virtual int Play(Game* game){ return 0; }
	static Player* Create(char playerType, char mark);
	std::string& getPlayerType(){ return m_playerType;}
	char getMark(){ return m_mark; }
private:
	std::string m_playerType;
	char m_mark;
};

class Human : public Player
{
public:
	Human(char mark) :Player("Human", mark){}
	int Play(Game* game);
};

class Computer: public Player
{
public:
	Computer(char mark);
	~Computer();
	int Play(Game* game);

private:
	void setupLevel();
	ComputerLevel *m_level;
};

class Game
{
public:
	~Game();
	bool setupGame();
	void Start();
	bool Validate(int position);
	bool IsGameover();
	void setMark(int box, char mark){gameBoard[box - 1] = mark;}

	char getPlayer1Mark() { return PLAYER1_MARK;}
	char getPlayer2Mark() { return PLAYER2_MARK;}
private:
	void Display();
	void NextMove();
	bool isBoardFull();

	char gameBoard[9];
	Player* player1;
	Player* player2;
	Player* m_nextPlayer;

	static const char PLAYER1_MARK = '0';
	static const char PLAYER2_MARK = 'X';
};



