#include <iostream>
#include <string>
#include <unistd.h>
#include <format>
#include <vector>
#include "config.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#if DEBUG
#include <sstream>
#endif

struct Options
{
    bool recursive = false;
    bool verbose = true;
    int max_threads = std::thread::hardware_concurrency();
    int rights = 0644;
};

bool check_if_directory_exists(const std::string &directory);
void command_resolver(int argc, char *argv[]);
void create_directory(const std::string &directory, Options &options);
void version_handler();
void help_handler();
#if DEBUG
template <typename T>
void print_if_debug(const T &printable);
#endif

template <typename T>
void print(const T &printable);