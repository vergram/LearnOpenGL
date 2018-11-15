#pragma once

#include <vector>

enum KeyBoard{
	W = 0,
	A = 1,
	S = 2,
	D = 3,
	ESC = 4,
};

const int KeyBoardSize = 5;

class Input
{
public:
	static float MouseSrollOffset;
	static float MouseMovementX;
	static float MouseMovementY;
	static std::vector<int> currentKeys;
};


