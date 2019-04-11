#include "MirrorEncryption.h"



MirrorEncryption::MirrorEncryption()
{
	_decryptedMessage = "";
}


MirrorEncryption::~MirrorEncryption()
{
}

void MirrorEncryption::run()
{
	init();

	load_encryption("mirror.txt");

	emplace_encryption();

	_encryptedMessage = "TpOnr";

	print_grid();

	decrypt(_encryptedMessage);
}

void MirrorEncryption::init()
{
	_letters.push_back("abcdefghijklm");
	_sLetters.push_back("nopqrstuvwxyz");

	std::memset(_grid, ' ', sizeof(_grid));

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		if (i == 0)
		{
			for (int j = 0; j < (ARRAY_SIZE - 1); j++)
			{
				_grid[i][j + 1] = _letters[0][j];
			}
		}

		if (i == 14)
		{
			for (int j = 0; j < (ARRAY_SIZE - 1); j++)
			{
				_grid[i][j + 1] = std::toupper(_sLetters[0][j]);
			}
		}
	}

	int counter = 0;

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		for (int j = 0; j < ARRAY_SIZE; j++)
		{
			if ((j == 0) && (i > 0) && (i != (ARRAY_SIZE - 1)))
			{
				_grid[i][j] = std::toupper(_letters[0][counter - 1]);
			}
		}
		counter++;
	}

	counter = 0;
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		for (int j = 0; j < ARRAY_SIZE; j++)
		{
			if ((j == 14) && (i > 0) && (i != (ARRAY_SIZE - 1)))
			{
				_grid[i][j] = _sLetters[0][counter - 1];
			}
		}
		counter++;
	}
}

void MirrorEncryption::load_encryption(const std::string & fileName)
{
	std::ifstream file(fileName);

	if (file.fail())
	{
		std::cerr << "Error loading file" << std::endl;
		_getch();
		return;
	}

	std::string line;

	while (getline(file, line))
	{
		_encryption.push_back(line);
	}

	file.close();
}

void MirrorEncryption::emplace_encryption()
{
	for (int i = 0; i < _encryption.size(); i++)
	{
		for (int j = 0; j < _encryption[i].size(); j++)
		{
			_grid[i + 1][j + 1] = _encryption[i][j];
		}
	}

	std::memcpy(_gridC, _grid, sizeof(char) * ARRAY_SIZE * ARRAY_SIZE);
}

void MirrorEncryption::decrypt(std::string encryptedMessage)
{
	for (int i = 0; i < encryptedMessage.length(); i++)
	{
		send_ping(encryptedMessage[i]);
	}
}

void MirrorEncryption::send_ping(char start)
{
	int x, y;

	ping p;

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		for (int j = 0; j < ARRAY_SIZE; j++)
		{
			if (_grid[i][j] == start)
			{
				x = i;
				y = j;
				p._x = i;
				p._y = j;
				p._start = _grid[i][j];
			}
		}
	}

	if (x == 14)
		p._dir = 0;
	if (x == 0)
		p._dir = 2;
	if (y == 14)
		p._dir = 3;
	if (y == 0)
		p._dir = 1;

	/*
	dir = 0 == MOVE UP
	dir = 2 == MOVE DOWN
	dir = 1 == MOVE RIGHT
	dir = 3 == MOVE LEFT
	*/

	bool done = false;
	while (!done)
	{
		char t = get_next_move(p._x, p._y, p._dir);

		int prevX = p._x;
		int prevY = p._y;

		switch (t)
		{
		case '\\':
			move_ping(p, MoveType::BACKSLASH);
			break;
		case '/':
			move_ping(p, MoveType::FORWARDSLASH);
			break;
		case ' ':
			move_ping(p);
			break;
		default:
			_decryptedMessage += t;
			_grid[p._x][p._y] = _gridC[p._x][p._y];
			done = true;
			break;
		}

		if (!done)
		{
			_grid[prevX][prevY] = _gridC[prevX][prevY];
			_grid[p._x][p._y] = '*';
		}

		print_grid();
		Sleep(100);
	}
}

void MirrorEncryption::move_ping(ping & p, MoveType type)
{
	switch (type)
	{
	case MoveType::BACKSLASH:
	{
		switch (p._dir)
		{
		case 0:
		{
			p._x -= 1;
			p._dir = 3;
			break;
		}
		case 1:
		{
			p._y += 1;
			p._dir = 2;
			break;
		}
		case 2:
		{
			p._x += 1;
			p._dir = 1;
			break;
		}
		case 3:
		{
			p._y -= 1;
			p._dir = 0;
			break;
		}
		default:
			break;
		}
		break;
	}
	case MoveType::FORWARDSLASH:
	{
		switch (p._dir)
		{
		case 0:
		{
			p._x -= 1;
			p._dir = 1;
			break;
		}
		case 1:
		{
			p._y += 1;
			p._dir = 0;
			break;
		}
		case 2:
		{
			p._x += 1;
			p._dir = 3;
			break;
		}
		case 3:
		{
			p._y -= 1;
			p._dir = 2;
			break;
		}
		default:
			break;
		}
		break;
	}
	case MoveType::DEFAULT:
	{
		switch (p._dir)
		{
		case 0:
		{
			p._x -= 1;
			break;
		}
		case 1:
		{
			p._y += 1;
			break;
		}
		case 2:
		{
			p._x += 1;
			break;
		}
		case 3:
		{
			p._y -= 1;
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}

char MirrorEncryption::get_next_move(int x, int y, int dir)
{
	char result = ' ';

	switch (dir)
	{
	case 0:
	{
		result = _grid[x - 1][y];
		break;
	}
	case 1:
	{
		result = _grid[x][y + 1];
		break;
	}
	case 2:
	{
		result = _grid[x + 1][y];
		break;
	}
	case 3:
	{
		result = _grid[x][y - 1];
		break;
	}
	default:
		break;
	}

	return result;
}

void MirrorEncryption::print_grid()
{
	system("cls");
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		for (int j = 0; j < ARRAY_SIZE; j++)
		{
			if (_grid[i][j] == '\\' || _grid[i][j] == '/')
			{
				std::cout << "|";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
				std::cout << _grid[i][j];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
			}
			else if (_grid[i][j] != '*')
			{
				std::cout << "|" << _grid[i][j];
			}
			else
			{
				std::cout << "|";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
				std::cout << _grid[i][j];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
			}

			if (j == (ARRAY_SIZE - 1))
				std::cout << "|";

		}

		if (i == 7)
			std::cout << "     Encrypted Message:    " << _encryptedMessage;
		else if (i == 8)
			std::cout << "     Decrypted Message:    " << _decryptedMessage;

		if (i != 14)
			printf("\n-------------------------------\n");
		else
			printf("\n-------------------------------");
	}
}
