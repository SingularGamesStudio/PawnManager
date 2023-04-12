#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#include <algorithm>
#include <cstring>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "boost/asio.hpp"
#include "boost/asio/ts/buffer.hpp"
#include "boost/asio/ts/internet.hpp"
