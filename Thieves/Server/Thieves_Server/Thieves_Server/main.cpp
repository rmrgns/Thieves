#include "game_server/ingame_server.h"
#include <clocale>
#include <iostream>
#include <locale>
#include <ostream>
#include "protocol.h"

using namespace std;
const int MAX_THREAD = 8;

int main() {
	setlocale(LC_ALL, "korean");

	wcout.imbue(locale("korean"));
	InGameServer server;
	server.Init(MAX_THREAD);
	cout << "Server Init End" << endl;

	server.BindListen(SERVER_PORT);
	cout << "Server Bind and Listen" << endl;

	server.Run();
	cout << "Start Server" << endl;
	server.End();

	return 0;
}