#include "Server.h"
#include "IOCP.h"
#include "define.h"
#include "protocol.h"
#include <clocale>
#include <iostream>
#include <locale>
#include <ostream>
#include <chrono>
#include <thread>


using namespace std;
const int MAX_THREADS = 8;

int main()
{
	setlocale(LC_ALL, "");
	wcout.imbue(locale(""));

	IOCP server;

	server.Init(MAX_THREADS, SERVER_PORT);
	server.Start();

	// 윈도우 네트워크 시스템은, 비동기 작업을 요청한 스레드(이 메인 스레드를 포함한 많은 스레드 들)가 종료되어 버리면,
	// GQCS가 강제로 끊어지고, GQCS에서 995 에러 (스레드 요청으로 IO 종료했어요 라는 에러)를 뿜어냄.
	// -> 해결하기 위해 그냥 쓰레드를 무한정 살려놓은 방향으로 해결해버리는 방법을 썼음.
	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}