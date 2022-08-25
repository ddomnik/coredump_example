# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-master/components/bootloader/subproject"
  "C:/Users/ddufner/Documents/esp/coredump/build/bootloader"
  "C:/Users/ddufner/Documents/esp/coredump/build/bootloader-prefix"
  "C:/Users/ddufner/Documents/esp/coredump/build/bootloader-prefix/tmp"
  "C:/Users/ddufner/Documents/esp/coredump/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/ddufner/Documents/esp/coredump/build/bootloader-prefix/src"
  "C:/Users/ddufner/Documents/esp/coredump/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/ddufner/Documents/esp/coredump/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
