/*

   Copyright (c) 2020, Iqbal Rifai
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
   EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
   DAMAGE.

 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define RED "\033[1m\033[31m"
#define WHITE "\033[1m\033[37m"
#define RESET "\x1B[0m"

void printErrorMessage(const char* format, const char* arg0) {
    printf(RED "error:" RESET WHITE " ");
    printf(format, arg0);
    printf(RESET);
    exit(EXIT_FAILURE);
}

void printHelpMessage(const char* arg0) {
    printf("usage:  %s <operation> [...]\n", arg0);
    printf("operations:\n");
    printf("    %s {-h}\n", arg0);
    printf("    %s {-R}  <package(s)>\n", arg0);
    printf("    %s {-S}  [package(s)]\n", arg0);
}

void searchAndInstallPackages(int argc, char** argv) {
    if (argc < 3) {
        printErrorMessage("No packages specified! Use %s -S <packages>!\n", argv[0]);
    }

    for (int i = 2; i < argc; i++) {
        char* result = malloc(500);
        printf(WHITE "Search Packages....\n");
        sleep(5);
        snprintf(result, 500, "cd /tmp && rm -rf %s && git clone https://aur.archlinux.org/%s.git && cd %s && makepkg -si",
                 argv[i], argv[i], argv[i]);
        system(result);
        free(result);
    }
}

void removePackages(int argc, char** argv) {
    if (argc < 3) {
        printErrorMessage("No packages specified! Use %s -R <packages>!\n", argv[0]);
    }

    char* names = malloc(1000);
    snprintf(names, 1000, "sudo pacman -R ");

    for (int i = 2; i < argc; i++) {
        strcat(names, argv[i]);
        strcat(names, " ");
    }

    system(names);
    free(names);
}

int main(int argc, char** argv) {
    int rootkh = getuid();

    if (rootkh == 0) {
        printErrorMessage("Running %s as root is not allowed as it can cause permanent, catastrophic damage to your system.\n", argv[0]);
    }

    if (argc < 2) {
        printErrorMessage("No operation specified (use -h for help)\n", argv[0]);
    }

    if (strcmp(argv[1], "-h") == 0) {
        printHelpMessage(argv[0]);
    } else if (strcmp(argv[1], "-S") == 0) {
        searchAndInstallPackages(argc, argv);
    } else if (strcmp(argv[1], "-R") == 0) {
        removePackages(argc, argv);
    } else {
        printErrorMessage("Command not found!\n", argv[0]);
    }

    return EXIT_SUCCESS;
}
