#pragma once
#include <random>
#include <string>
class Utility
{
public:

	//For attack and defense calculations, we use a multiplier using users attack and enemy defense
	//attack / (attack + defense)
	static float ComputeDamageMult(int attack, int defense);

	//Generates a random integer with values between (min) and (max) and includes both of those numbers
	static int GenerateRandomInteger(int min, int max);
	//Replace a selected text with nothing - Used to replace %HP and %MP with their respective HP and MP values
	static std::string Replace(std::string text, std::string original, std::string new_text);
	//Converts a float value into string with specified decimal places. 0 means the string is truncated with no decimals.
	static std::string ToString(float value, int setprecision = 0);
	//Converts a int value into string.
	static std::string ToString(int value);
	//Split the text using a delimeter text. Returns an array
	static std::vector<std::string> SplitText(std::string string, std::string delimeter);
};