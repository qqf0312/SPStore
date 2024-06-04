// "Copyright [2018] <fisco-dev>"
/*
    This file is part of FISCO-BCOS
    FISCO-BCOS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FISCO-BCOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FISCO-BCOS.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * @file: BuildInfo.h
 * @author: fisco-dev
 * @date: 2018
 * build information template
 */

#pragma once
#define SCHAIN_COMMIT_HASH "b8b62664d1b1f0ad0489bc4b3833bf730deee492"
#define SCHAIN_CLEAN_REPO "0"
#define SCHAIN_BUILD_TYPE "Debug"
#define SCHAIN_BUILD_OS "Linux"
#define SCHAIN_BUILD_COMPILER "g++"
#define SCHAIN_BUILD_PLATFORM "Linux/g++"
#define SCHAIN_BUILD_NUMBER "20200602"
#define SCHAIN_VERSION_SUFFIX ""
#define SCHAIN_BUILD_TIME "20200602 17:01:05"
#define SCHAIN_BUILD_BRANCH "master"

#ifdef FISCO_GM
#define SCHAIN_PROJECT_VERSION "2.3.0 gm"
#else
#define SCHAIN_PROJECT_VERSION "2.3.0"
#endif
