#include "Utility.h"
#include <iomanip>
#include <sstream>


int Utility::GenerateRandomInteger(int min, int max) {
	//Taken from google searched how to generate random numbers

	//Originally while coding this idle RPG, random number generator all started with rand() function, i learned at university.
	//However as I get into more depth of rand() to use for my console game, it didn't satisfy my needs mainly because it uses a seed
	//which means if i were to run the program again, the rand() will generate the same numbers once again.
	//I can try this one srand( time( NULL ) ) which will make a different seed each time i run the program because it now determined based on current time of the system.
	//the numbers generate between 0 to 32767. The only way to change it is using modulus to limit my numbers and then translate the numbers using the min number:
	//-50 to 10:
	//first I modulus for 60 since the difference between 10 and -50 is 60
	//which i get a number from 0 to 32767 to 0 to 60. 
	//Then I translate by adding the min number of -50 which changes from 0 to 60 to -50 to 10.
	//However the problem arises with multiple players running at the same time, it can easily reproduce the same random number generator result.

	//In which I then abandoned what I learned at university of rand() and srand(time(NULL)) and google searched this fancy random number generator
	//from <random> c++ standard lib.

	//Initialize random number generator
	//Random device used to generate number
	std::random_device dev;
	//Mersenne Twister pseudo random number generator 32 bit numbers with state size of 19937 bits
	std::mt19937 rng(dev());
	//Use a uniform int distribution (equal chance of getting a random number)
	std::uniform_int_distribution<int> integer(min, max);
	//Return a selected integer value.
	return integer(rng);
}

float Utility::ComputeDamageMult(int damage, int defense)
{
	//Taken from basic damage calculation.
	float multiplier = damage / (damage + defense * 1.f);
	return multiplier;
}

std::string  Utility::Replace(std::string text, std::string original, std::string new_text) {
	//How I got this code? I googled search how to replace text. Inspired from c# string Replace()
	std::string d = text;
	size_t pos = text.find(original);
	if (pos != std::string::npos) {
		d = text.replace(pos, original.size(), new_text);
	}
	return d;
}
std::string  Utility::ToString(float value, int setprecision) {
	//How I got this code? I googled search how to convert numbers into string. Inspired from c# object ToString()
	//Originally i could use std::to_string but there are decimal places like .0000 which is not friendly to players
	std::stringstream ostr;
	//Use std::fixed to change decimal places
	ostr << std::setprecision(setprecision) << std::fixed << value;

	return ostr.str().c_str();
}
std::string Utility::ToString(int value)
{
	std::stringstream ostr;
	//Use std::fixed to change decimal places
	ostr << value;

	return ostr.str().c_str();
}
std::vector<std::string> Utility::SplitText(std::string string, std::string delimeter) {
	//How I get this code? I googled search how to split text using a character (delimeter). Inspired from c# string Split()
	//This is extremely difficult to understand. The original code i copied was not exact but I though of a vector to replace some of the code so it stores to
	//a vector instead.
	std::string s = string;
	std::string delimiter = delimeter;
	std::vector<std::string> vector_list;
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		vector_list.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	vector_list.push_back(s);
	return vector_list;
}