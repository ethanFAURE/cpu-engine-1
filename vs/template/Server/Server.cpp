#include <iostream>
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

//1888 pour le port
//Local host 127.0.0.1
//s short
//l long
//bind sert à ouvrire un port explicitement 
//Port 1888
//146.190.62.39 adresse ip du sit web
//recv pour recevoir (le receve est blockant faut le faire en thread)
//recvfrom message + ip et Port
//sendto message + ip et Port
//penser a envoyer des ping pour garder la connexion active

#define BUFFER_SIZE 1024

PCSTR ip;
u_short port;
sockaddr_in addr;
CRITICAL_SECTION g_CriticalSection;
SOCKET sock;

bool sended = false;
bool isConnected = false;
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

    port = 1888;

    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

bool LauchServer(SOCKET sock, sockaddr_in addr)
{
    if (sock == INVALID_SOCKET)
        return SOCKET_ERROR;

    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) != 0)
        Display("Bind failed: "), Display(std::to_string(WSAGetLastError()));
    else
        Display("Bind successful");

    return true;
}

bool SendData(SOCKET sock, sockaddr_in addr, char* buffer)
{
    if (sendto(sock, buffer, BUFFER_SIZE, 0, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        Display("send failed: "), Display(std::to_string(WSAGetLastError()));
        return SOCKET_ERROR;
    }
    else
        Display("Data sent");
}

bool ReceveData(SOCKET sock, char* buffer)
{
    sockaddr_in from;
    int addrLen = sizeof(from);
    int ret = recvfrom(sock, buffer, BUFFER_SIZE, 0, (sockaddr*)&from, &addrLen);
    if (ret == SOCKET_ERROR) {
        Display("Receive error: " + std::to_string(WSAGetLastError()));
        return false;
    }
    if (ret <= 0) {
        Display("No data received");
        return false;
    }

    char ipstr[INET_ADDRSTRLEN] = { 0 };
    inet_ntop(AF_INET, &from.sin_addr, ipstr, sizeof(ipstr));
    Display(std::string("Received ") + std::to_string(ret) + " bytes from " + ipstr + ":" + std::to_string(ntohs(from.sin_port)));

    std::cout.write(buffer, ret);
    std::cout << std::endl;

    return true;
}

int ThreadFunction()
{
    Init();

    char buffer[BUFFER_SIZE] = "reponse server";

    bool isConnected = LauchServer(sock, addr);

    if (!isConnected) {
        Cleanup(sock);
        return SOCKET_ERROR;
    }

    while (true)
    {
        if (isDataReceived == false) {
            isDataReceived = ReceveData(sock, buffer);
        }

        if (isDataReceived == true) {
            isDataSent = SendData(sock, addr, buffer);
        }

        isDataReceived = false;
        isDataSent = false;
    }

    Cleanup(sock);
    return 1;
}

DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter) {
    ThreadFunction();
    return 0;
}

int main()
{
    InitializeCriticalSection(&g_CriticalSection);

    HANDLE thread = CreateThread(NULL, 0, &ThreadProc, NULL, NULL, NULL);

    WaitForSingleObject(thread, INFINITE);

    return 0;
}