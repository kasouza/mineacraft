#ifndef KASOUZA_MINECRAFT_INCLUDE_UTILS_INCLUDE_UTILS_FS_HPP
#define KASOUZA_MINECRAFT_INCLUDE_UTILS_INCLUDE_UTILS_FS_HPP

#include <string>

namespace mine {

namespace utils {

namespace fs {

/**
 * Read a file and return its contents as a string
 *
 * @param path const char*
 * @return std::string
 */
std::string readFile(const char *path);

} // namespace fs

} // namespace utils

} // namespace mine

#endif
