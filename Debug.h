// Debug.h
#pragma once
#include <iostream>
#include <sstream>

#ifdef _DEBUG
#define DEBUG_PRINT(x) { std::stringstream ss; ss << "JPS: " << x << std::endl; OutputDebugStringA(ss.str().c_str()); }
#else
#define DEBUG_PRINT(x)
#endif