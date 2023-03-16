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
#include <memory>
#include "boost/asio.hpp"
#include "boost/asio/ts/buffer.hpp"
#include "boost/asio/ts/internet.hpp"
