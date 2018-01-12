/*
 * Copyright (C) 2018 Vincent Sallaberry
 * vmultilangdemo <https://github.com/vsallaberry/vmultilangdemo>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * -------------------------------------------------------------------------
 * main
 */
#include <cstdio>
#ifndef _NOJAVA
#include <gcj/cni.h>
#endif

#include "version.h"

extern "C" const char *const* vmultilangdemo_get_source();
extern "C" int m();
extern "C" int cpp_call_for_c(int);
extern "C" int cpp_cni_call_for_c(int);

int main(int argc, const char *const* argv) {
    fprintf(stdout, "Copyright (C) 2018 Vincent Sallaberry.\n"
                    "This is free software; see the source for copying conditions.  There is NO\n"
                    "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\n");
    fprintf(stdout, "%s (%s v%s built on %s, %s from git-rev %s)\n",
            *argv, BUILD_APPNAME, APP_VERSION, __DATE__, __TIME__, BUILD_GITREV);
    fprintf(stdout, "  prefix     : %s\n"
                    "  srcpath    : %s\n"
                    "  git_full   : %s\n"
                    "  CC         : %s\n"
                    "  CXX        : %s\n"
                    "  GCJ        : %s\n"
                    "  CCLD       : %s\n"
                    "\n",
                    BUILD_PREFIX, BUILD_SRCPATH, BUILD_FULLGITREV,
                    BUILD_CC_CMD, BUILD_CXX_CMD, BUILD_GCJ_CMD, BUILD_CCLD_CMD);
    if (argc > 1) {
#     ifdef APP_INCLUDE_SOURCE
        for (const char *const* line = vmultilangdemo_get_source(); *line; line++) {
            fprintf(stderr, "%s", *line);
        }
#     endif
    }
    //m();
    cpp_call_for_c(0);
#   ifndef _NOJAVA
    fprintf(stdout, "Starting Java...\n");
    JvCreateJavaVM(NULL);
    JvAttachCurrentThread(NULL, NULL);
    cpp_cni_call_for_c(0);
    JvDetachCurrentThread();
#   endif
    return 0;
}
