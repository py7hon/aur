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

int
main(int argc, char** argv)
{
  int i;
  int rootkh = getuid();

  if (rootkh == 0) {
    printf(RED "error:" RESET WHITE " Running %s as root is not allowed as it "
                                    "can cause permanent, catastrophic damage "
                                    "to your system.\n" RESET,
           argv[0]);
    exit(EXIT_FAILURE);
  }

  if (argc < 2) {
    printf(RED "error:" RESET WHITE
               " no operation specified (use -h for help)\n" RESET,
           argv[0]);
    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[1], "-h") == 0) {
    printf("usage:  %s <operation> [...]\n", argv[0]);
    printf("operations:\n");
    printf("    %s {-h}\n", argv[0]);
    printf("    %s {-R}  <package(s)>\n", argv[0]);
    printf("    %s {-S}  [package(s)]\n", argv[0]);

  } else if (strcmp(argv[1], "-S") == 0) {

    if (argc < 3) {
      printf(RED "error:" RESET WHITE
                 " No packages specified! Use %s -S <packages>!\n" RESET,
             argv[0]);
      exit(EXIT_FAILURE);
    }
    for (i = 2; i < argc; i++) {

      char* result = malloc(500);
      printf(WHITE "Search Packages....\n");
      sleep(5);
      strcat(result, "cd /tmp && rm -rf ");
      strcat(result, argv[i]);
      strcat(result, " && git clone https://aur.archlinux.org/");
      strcat(result, argv[i]);
      strcat(result, ".git && cd ");
      strcat(result, argv[i]);
      printf(RESET);
      strcat(result, " && makepkg -si");

      system(result);

      free(result);
    }
  } else if (strcmp(argv[1], "-R") == 0) {

    if (argc < 3) {
      printf(RED "error:" RESET WHITE
                 " No packages specified! Use %s -R <packages>!\n" RESET,
             argv[0]);
      exit(EXIT_FAILURE);
    }

    char* names = malloc(1000);

    strcat(names, "sudo pacman -R ");

    for (i = 2; i < argc; i++) {

      strcat(names, argv[i]);
      strcat(names, " ");
    }

    system(names);

    free(names);

  } else {
    printf(RED "error:" RESET WHITE " Command not found!\n" RESET);
  }
  return EXIT_SUCCESS;
}
