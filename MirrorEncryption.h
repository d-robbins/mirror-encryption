#ifndef MIRRORENCRYPTION_H
#define MIRRORENCRYPTION_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <conio.h>
#include <Windows.h>

enum MoveType { DEFAULT, FORWARDSLASH, BACKSLASH };

struct ping
{
	int _dir;
	int _x, _y;
	char _draw;
	char _start;
};

class MirrorEncryption
{
public:
	MirrorEncryption();
	~MirrorEncryption();

	void run();

private:
	void init();

	void load_encryption(const std::string & fileName);

	void emplace_encryption();

	void decrypt(std::string encryptedMessage);

	void print_grid();

	void send_ping(char start);

	char get_next_move(int x, int y, int dir);

	void move_ping(ping & p, MoveType type = MoveType::DEFAULT);

	static const size_t ARRAY_SIZE = 15;

	char _grid[ARRAY_SIZE][ARRAY_SIZE];
	char _gridC[ARRAY_SIZE][ARRAY_SIZE];

	std::string _encryptedMessage;
	std::string _decryptedMessage;

	std::vector < std::string > _letters;
	std::vector < std::string > _sLetters;
	std::vector < std::string > _encryption;
};


#endif // !MIRRORENCRYPTION_H