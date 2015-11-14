#pragma once

#ifdef TEST_DLL_EXPORTS
#define TEST_DLL_API __declspec(dllexport)
#else
#define TEST_DLL_API __declspec(dllimport)
#endif

TEST_DLL_API void test_dll();
