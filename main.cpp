// main.cpp
#include "framework.h"
#include "World.h"
#include "Constants.h"
#include <iostream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
std::unique_ptr<World> world;
bool isLeftMouseDown = false;
std::optional<bool> currentDrawMode;  // true for adding walls, false for removing

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#ifdef _DEBUG
    // Create console for debug output only in debug mode
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
#endif

    // Register window class
    const wchar_t CLASS_NAME[] = L"Pathfinding Visualizer";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.style = CS_OWNDC;

    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Pathfinding Visualizer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        Constants::WINDOW_WIDTH + 16,
        Constants::WINDOW_HEIGHT + 39,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr) {
        return 0;
    }

    world = std::make_unique<World>();

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_ERASEBKGND:
        return 1;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        world->render(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_KEYDOWN: {
        switch (wParam) {
        case 'A':
            world->findPathAStar();
            InvalidateRect(hwnd, nullptr, FALSE);
            break;
        case 'D':
            world->findPathDijkstra();
            InvalidateRect(hwnd, nullptr, FALSE);
            break;
        case 'J':
            world->findPathJPS();
            InvalidateRect(hwnd, nullptr, FALSE);
            break;
        }
        return 0;
    }

    case WM_LBUTTONDOWN: {
        isLeftMouseDown = true;
        POINTS pt = MAKEPOINTS(lParam);
        int gridX = pt.x / Constants::CELL_SIZE;
        int gridY = pt.y / Constants::CELL_SIZE;
        if (gridX >= 0 && gridX < Constants::GRID_WIDTH &&
            gridY >= 0 && gridY < Constants::GRID_HEIGHT) {
            // Set the draw mode based on the initial cell state
            currentDrawMode = !world->isWall(gridX, gridY);
            world->setWall(gridX, gridY, *currentDrawMode);
#ifdef _DEBUG
            std::cout << "Adding wall at: " << gridX << ", " << gridY << std::endl;
#endif
            InvalidateRect(hwnd, nullptr, FALSE);
        }
        return 0;
    }

    case WM_LBUTTONUP: {
        isLeftMouseDown = false;
        currentDrawMode.reset();  // Reset the draw mode when mouse is released
        return 0;
    }

    case WM_MOUSEMOVE: {
        if (isLeftMouseDown && currentDrawMode.has_value()) {
            POINTS pt = MAKEPOINTS(lParam);
            int gridX = pt.x / Constants::CELL_SIZE;
            int gridY = pt.y / Constants::CELL_SIZE;
            if (gridX >= 0 && gridX < Constants::GRID_WIDTH &&
                gridY >= 0 && gridY < Constants::GRID_HEIGHT) {
                world->setWall(gridX, gridY, *currentDrawMode);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
        }
        return 0;
    }

    case WM_RBUTTONDOWN: {
        POINTS pt = MAKEPOINTS(lParam);
        int gridX = pt.x / Constants::CELL_SIZE;
        int gridY = pt.y / Constants::CELL_SIZE;
        if (gridX >= 0 && gridX < Constants::GRID_WIDTH &&
            gridY >= 0 && gridY < Constants::GRID_HEIGHT) {
            if (GetKeyState(VK_CONTROL) & 0x8000) {
#ifdef _DEBUG
                std::cout << "Setting end point at: " << gridX << ", " << gridY << std::endl;
#endif
                world->setEndPoint(gridX, gridY);
            }
            else {
#ifdef _DEBUG
                std::cout << "Setting start point at: " << gridX << ", " << gridY << std::endl;
#endif
                world->setStartPoint(gridX, gridY);
            }
            InvalidateRect(hwnd, nullptr, FALSE);
        }
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}