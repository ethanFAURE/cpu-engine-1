#include <iostream>
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

#define BUFFER_SIZE 1024

PCSTR ip;
u_short port;
sockaddr_in addr;
CRITICAL_SECTION g_CriticalSection;
SOCKET sock;

bool sended = false;
bool isDataSent = false;
bool isDataReceived = false;

void Display(std::string message)
{
    std::cout << message << std::endl;
}

void Cleanup(SOCKET sock)
{
    shutdown(sock, SD_SEND);

    closesocket(sock);
    WSACleanup();
}

void InitializeWinsock()
{
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);
}

void Init()
{
    InitializeWinsock();

    ip = "10.10.137.43";
    port = 1888;

    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0)
        Display("Invalid address: "), Display(ip);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

bool SendData(SOCKET sock, char* buffer)
{
    int ret = sendto(sock, buffer, (int)strlen(buffer), 0, (sockaddr*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR) {
        Display("Send failed");
        closesocket(sock);
        return false;
    }
    else {
        Display("Data sent");
        return true;
    }
}

bool ReceveData(SOCKET sock, char* buffer)
{
    socklen_t addrLen = sizeof(addr);
    int ret = recvfrom(sock, buffer, BUFFER_SIZE, 0, (sockaddr*)&addr, &addrLen);
    if (ret <= 0) {
        Display("Receive error");
        closesocket(sock);
        return false;
    }
    else {
        Display("Received data:");
        std::cout.write(buffer, ret);
        std::cout << std::endl;
        return true;
    }
}

int ThreadFunction()
{
    Init();

    Cleanup(sock);
    return 1;
}

DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter) {
    ThreadFunction();
    return 0;
}

int main()
{
    Init();

    Sleep(1000);

    char buffer[BUFFER_SIZE] = { 0 };

    while (true)
    {
        std::string input;
        std::cout << "Client sending: ";
        if (!std::getline(std::cin, input)) break;

        if (input.empty()) continue;
        if (input == "quit" || input == "exit") break;

        if (input.size() >= sizeof(buffer)) {
            Display("Message too long");
            continue;
        }
        strcpy_s(buffer, sizeof(buffer), input.c_str());

        if (!SendData(sock, buffer)) {
        }

        if (!ReceveData(sock, buffer)) {
        }
    }

    Cleanup(sock);
    return 0;
}