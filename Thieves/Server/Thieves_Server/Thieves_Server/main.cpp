
#include "pch.h"

#include "game_server/ingame_server.h"
using namespace std;
const int MAX_THREAD = 8;

int main() {
	setlocale(LC_ALL, "korean");

	wcout.imbue(locale("korean"));
	InGameServer server;
	server.Init(MAX_THREAD);
	cout << "�ʱ�ȭ �Ϸ�" << endl;

	server.BindListen(SERVER_PORT);
	cout << "���ε� ���� ��" << endl;

	server.Run();
	cout << "���� ��" << endl;
	server.End();

	return 0;
}