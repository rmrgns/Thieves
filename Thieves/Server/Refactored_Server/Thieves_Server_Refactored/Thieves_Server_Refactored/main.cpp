#include "Server.hpp"
#include "define.hpp"
#include <clocale>
#include <iostream>
#include <locale>
#include <ostream>

using namespace std;
const int MAX_THREADS = 8;

int main()
{
	setlocale(LC_ALL, "korean");

	wcout.imbue(locale("korean"));
	Server server;

}