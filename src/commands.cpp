#include "commands.hpp"

void command_resolver(int argc, char *argv[])
{
    Options options{};
    int argument = 0;

    while ((argument = getopt(argc, argv, "m:t:vhrs")) != -1)
    {
        switch (argument)
        {
        case 'm':
            options.rights = std::stoi(optarg, (std::size_t *)0, 8);
#if DEBUG
            print_if_debug("set rights to " + (std::ostringstream() << std::oct << options.rights).str());
#endif
            break;
        case 't':
            options.max_threads = std::stoi(optarg);
            break;
        case 'v':
            version_handler();
            break;
        case 'h':
            help_handler();
            break;
        case 'r':
#if DEBUG
            print_if_debug("set to recursive");
#endif
            options.recursive = true;
            break;
        case 's':
#if DEBUG
            print_if_debug("set to verbose");
#endif
            options.verbose = false;
            break;
        }
    }
#if DEBUG
    print_if_debug(std::format("using {} threads", options.max_threads));
#endif
    for (; optind < argc;)
    {
        int start = optind;
        int current_working_threads = 0;
        std::vector<std::thread> threads;
        for (; optind < argc && optind - options.max_threads < start; optind++)
        {
#if DEBUG
            print_if_debug(std::format("spawned thread {}", current_working_threads));
#endif
            current_working_threads += 1;
            const std::string directory = static_cast<std::string>(argv[optind]);
            threads.push_back(std::thread(std::ref(create_directory), directory, std::ref(options)));
        }

        for (int thread_index = 0; thread_index < threads.size(); thread_index++)
        {
            threads[thread_index].join();
#if DEBUG
            print_if_debug(std::format("finished thread {}", thread_index));
#endif
        }
#if DEBUG
        print_if_debug(std::format("finished threads: {}", current_working_threads));
#endif
    }
}

bool check_if_directory_exists(const std::string &directory)
{
    DIR *dir = opendir(directory.c_str());
    return bool(dir);
}
void recursive_directory_create(const std::string &directory, Options &options)
{
    std::string accumulator = "";

    for (auto c : directory)
    {
        if (c == '/')
        {
            if (!check_if_directory_exists(accumulator))
            {
                mkdir(accumulator.c_str(), options.rights);
#if DEBUG
                print_if_debug(std::format("recursively created directory '{}'", accumulator));
#else
                if (options.verbose)
                    print(std::format("recursively created directory '{}'", accumulator));
#endif
            }
            else
            {
#if DEBUG
                print_if_debug(std::format("cannot recursively create directory '{}': directory exists", accumulator));
#endif
            }
        }
        accumulator += c;
    }
}
void create_directory(const std::string &directory, Options &options)
{
    if (check_if_directory_exists(directory))
    {
        print(std::format("cannot create directory '{}': directory exists", directory));
        return;
    }
    if (options.recursive)
    {
        recursive_directory_create(directory, options);
    }
    if (mkdir(directory.c_str(), options.rights))
    {
#if DEBUG
        print_if_debug(std::format("cannot create directory '{}': {}", directory, std::strerror(errno)));
#else
        print(std::format("cannot create directory '{}': {}", directory, std::strerror(errno)));
#endif
        return;
    }
#if DEBUG
    print_if_debug(std::format("created directory '{}'", directory));
#else
    if (options.verbose)
    {
        print(std::format("created directory '{}'", directory));
    }

#endif
}

void version_handler()
{
#if DEBUG
    print_if_debug("version handler invoked");
#endif

    const std::string version = std::format("{}.{}.{}", MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION);
    const std::string version_text =
        "fmd " +
        version + "\n" +
        "Copyright (C) 2024 Free Software Foundation, Inc.\n"
        "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n"
        "This is free software: you are free to change and redistribute it.\n"
        "There is NO WARRANTY, to the extent permitted by law.\n"
        "\nWritten by Egor Efremov.\n";

    std::cout << version_text;
    exit(0);
}

void help_handler()
{
#if DEBUG
    print_if_debug("help handler invoked");
#endif

    const std::string help_text =
        "Usage: fmd [OPTION]... DIRECTORY...\n"
        "Create the DIRECTORY(ies), if they do not already exist.\n\n"
        "  -m=MODE      set file mode (as in chmod), not a=rwx - umask\n"
        "  -t=THREADS   set maximum number of usable threads\n"
        "  -r           make parent directories recursively\n"
        "  -s           suppress messages\n"
        "  -h           display this help and exit\n"
        "  -v           output version information and exit\n";
    std::cout << help_text;
    exit(0);
}

#if DEBUG
template <typename T>
void print_if_debug(const T &printable)
{
    if (DEBUG)
        std::cout << "fmd debug: " << printable << "\n";
}
#endif

template <typename T>
void print(const T &printable)
{
    std::cout << "fmd: " << printable << "\n";
}