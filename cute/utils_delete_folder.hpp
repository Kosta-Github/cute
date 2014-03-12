// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <algorithm>
#include <string>

#if defined(WIN32)
#   include <Windows.h>
#   include <direct.h>
#else // defined(WIN32)
#   include <dirent.h>
#   include <sys/stat.h>
#   include <unistd.h>
#endif // defined(WIN32)

namespace cute {
    namespace detail {

#if defined(WIN32)

        inline bool delete_folder(std::string dir) {
            std::replace(dir.begin(), dir.end(), '/', '\\');
            while(!dir.empty() && (dir.back() == '\\')) { dir.pop_back(); } // remove trailing backslashes

            WIN32_FIND_DATA ffd;
            auto hFind = FindFirstFile((dir + "\\*").c_str(), &ffd); // list all files in that folder
            if (hFind == INVALID_HANDLE_VALUE) { return false; }

            bool success = true;
            do {
                auto sub = std::string(ffd.cFileName);
                if((sub != ".") && (sub != "..")) { // skip "." and ".." folders
                    auto sub_path = dir + "\\" + sub;
                    if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                        success = delete_folder(sub_path) && success;
                    } else {
                        // remove read-only flag
                        success = (SetFileAttributes(sub_path.c_str(), FILE_ATTRIBUTE_NORMAL) != 0) && success;
                        success = (remove(sub_path.c_str()) == 0) && success;
                    }
                }
            } while(FindNextFile(hFind, &ffd) != 0);

            success = (FindClose(hFind) != FALSE) && success;
            success = (_rmdir(dir.c_str()) == 0) && success;

            return success;
        }

#else // defined(WIN32)

        inline bool delete_folder(std::string const& dir) {
            auto dp = opendir(dir.c_str());
            if(!dp) { return false; }

            bool success = true;
            while(auto ep = readdir(dp)) {
                auto sub_path = (dir + "/") + ep->d_name;

                struct stat stat_buf;
                if((stat(sub_path.c_str(), &stat_buf) == 0) && (S_ISDIR(stat_buf.st_mode) != 0)) {
                    success = delete_folder(sub_path) && success;
                } else {
                    success = (unlink(sub_path.c_str()) == 0) && success;
                }
            }

            success = (closedir(dp) == 0) && success;
            success = (rmdir(dir.c_str()) == 0) && success;

            return success;
        }

#endif // defined(WIN32)
        
    } // namespace detail
} // namespace cute
