// TestCounters.h
#pragma once

extern int passed;  // Declare as extern to share across files
extern int failed;


#define CHECK(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "\033[1;31m[FAIL]\033[0m " << message << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
            failed++; \
        } else { \
            std::cout << "\033[1;32m[PASS]\033[0m " << message << "\n"; \
            passed++; \
        } \
    } while(0)

// Utility macro for checking if an expression throws an exception
#define CHECK_THROWS(expression, message) \
    do { \
        bool threw = false; \
        try { expression; } \
        catch (...) { threw = true; } \
        CHECK(threw, message); \
    } while(0)

// Utility macro for checking if an expression does NOT throw an exception
#define CHECK_NOTHROW(expression, message) \
    do { \
        bool threw = false; \
        try { expression; } \
        catch (...) { threw = true; } \
        CHECK(!threw, message); \
    } while(0)
