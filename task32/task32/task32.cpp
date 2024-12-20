#include <windows.h>
#include <iostream>

HANDLE hEvent;

DWORD WINAPI WaitThread(LPVOID lpParam) {
    std::cout << "thread waiting for the event started." << std::endl;

    DWORD dwWaitResult = WaitForSingleObject(hEvent, INFINITE);

    if (dwWaitResult == WAIT_OBJECT_0) {
        std::cout << "Event receved, thread continues execution" << std::endl;
    }
    else {
        std::cerr << "Error while waiting for the event" << std::endl;
    }

    return 0;
}

DWORD WINAPI SetThread(LPVOID lpParam) {
    std::cout << "Thread setting the event started. Waiting..." << std::endl;

    Sleep(3000);

    std::cout << "Setting the event" << std::endl;

    if (!SetEvent(hEvent)) {
        std::cerr << "Error while setting the event" << std::endl;
    }

    return 0;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (hEvent == NULL) {
        std::cerr << "Failed to create the event." << std::endl;
        return 1;
    }

    std::cout << "Event created." << std::endl;

    HANDLE hWaitThread = CreateThread(NULL, 0, WaitThread, NULL, 0, NULL);
    if (hWaitThread == NULL) {
        std::cerr << "Failed to create the waiting thread." << std::endl;
        CloseHandle(hEvent);
        return 1;
    }

    HANDLE hSetThread = CreateThread(NULL, 0, SetThread, NULL, 0, NULL);
    if (hSetThread == NULL) {
        std::cerr << "Failed to create the setting thread." << std::endl;
        CloseHandle(hWaitThread);
        CloseHandle(hEvent);
        return 1;
    }

    WaitForSingleObject(hWaitThread, INFINITE);
    WaitForSingleObject(hSetThread, INFINITE);

    CloseHandle(hWaitThread);
    CloseHandle(hSetThread);
    CloseHandle(hEvent);

    std::cout << "Program finished execution." << std::endl;

    return 0;
}
