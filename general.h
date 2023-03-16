#define ASIO_STANDALONE

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <thread>
#include <queue>
#include <deque>
#include <mutex>
#include "asio.hpp"
#include "asio/ts/buffer.hpp"
#include "asio/ts/internet.hpp"
