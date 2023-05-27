#include "utils/fs.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace mine {

namespace utils {

namespace fs {

/**
 * Read a file and return its contents as a string
 *
 * @param path const char*
 * @return std::string
 */
std::string readFile(const char *path) {
    std::fstream fs{path};
    if (fs.fail()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        exit(1);
    }

    std::stringstream ss;
    ss << fs.rdbuf();

    return ss.str();
}
} // namespace fs
} // namespace utils
} // namespace mine
