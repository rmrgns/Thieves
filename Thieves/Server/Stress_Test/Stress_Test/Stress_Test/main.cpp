#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "protocol.h" 

#pragma comment(lib, "ws2_32.lib")

struct ClientSession {
    SOCKET socket;
    int id;
    bool is_logged_in = false;

    char recv_buffer[1024];
    WSABUF wsa_buf;
    WSAOVERLAPPED overlapped;
};

std::vector<ClientSession*> g_sessions;
std::mutex g_sessions_mutex;

std::atomic<int> g_connected_count{ 0 };
std::atomic<int> g_logged_in_count{ 0 };

void TestPacketSenderThread() {
    auto interval = std::chrono::milliseconds(16);

    while (true) {
        auto start_time = std::chrono::steady_clock::now();

        {
            std::lock_guard<std::mutex> lock(g_sessions_mutex);
            for (auto session : g_sessions) {
                if (session->is_logged_in) {
                    cs_packet_test test_packet;
                    test_packet.size = sizeof(cs_packet_test);
                    test_packet.type = CS_PACKET_TEST;
                    test_packet.id = session->id;
                    test_packet.num = 0;

                    send(session->socket, reinterpret_cast<char*>(&test_packet), test_packet.size, 0);
                }
            }
        }

        auto end_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        if (elapsed_time < interval) {
            std::this_thread::sleep_for(interval - elapsed_time);
        }
    }
}

void WorkerThread(HANDLE iocp_handle) {
    DWORD bytes_transferred;
    ULONG_PTR completion_key;
    LPOVERLAPPED overlapped;

    while (true) {
        BOOL result = GetQueuedCompletionStatus(iocp_handle, &bytes_transferred, &completion_key, &overlapped, INFINITE);

        ClientSession* session = reinterpret_cast<ClientSession*>(completion_key);

        if (!result || bytes_transferred == 0) {
            if (session) {
                g_connected_count--;
                if (session->is_logged_in) {
                    g_logged_in_count--;
                }
            }
            continue;
        }

        char packet_type = session->recv_buffer[1];

        if (packet_type == SC_PACKET_SIGN_IN_OK) {
            sc_packet_sign_in_ok* ok_packet = reinterpret_cast<sc_packet_sign_in_ok*>(session->recv_buffer);
            session->id = ok_packet->id;

            if (!session->is_logged_in) {
                session->is_logged_in = true;
                g_logged_in_count++;
            }
        }

        DWORD flags = 0;
        WSARecv(session->socket, &session->wsa_buf, 1, &bytes_transferred, &flags, &session->overlapped, NULL);
    }
}

void MonitorThread() {
    while (true) {
        std::cout << "[Monitor] 현재 연결된 클라이언트: " << g_connected_count.load()
            << " / 로그인 완료: " << g_logged_in_count.load() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {

    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        return -1;
    }

    HANDLE iocp_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    std::thread worker(WorkerThread, iocp_handle);
    worker.detach();

    std::thread sender(TestPacketSenderThread);
    sender.detach();

    std::thread monitor(MonitorThread);
    monitor.detach();

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    while (true) {
        SOCKET client_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

        if (connect(client_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == 0) {
            ClientSession* session = new ClientSession();
            session->socket = client_socket;
            session->wsa_buf.buf = session->recv_buffer;
            session->wsa_buf.len = sizeof(session->recv_buffer);
            ZeroMemory(&session->overlapped, sizeof(WSAOVERLAPPED));

            CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_socket), iocp_handle, reinterpret_cast<ULONG_PTR>(session), 0);

            {
                std::lock_guard<std::mutex> lock(g_sessions_mutex);
                g_sessions.push_back(session);
            }

            g_connected_count++;

            cs_packet_sign_in sign_in_packet;
            sign_in_packet.size = sizeof(cs_packet_sign_in);
            sign_in_packet.type = CS_PACKET_SIGN_IN;
            strcpy_s(sign_in_packet.name, "StressTestUser");
            strcpy_s(sign_in_packet.password, "1234");

            send(client_socket, reinterpret_cast<char*>(&sign_in_packet), sign_in_packet.size, 0);

            DWORD bytes_transferred;
            DWORD flags = 0;
            WSARecv(client_socket, &session->wsa_buf, 1, &bytes_transferred, &flags, &session->overlapped, NULL);
        }
        else {
            closesocket(client_socket);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    WSACleanup();
    return 0;
}