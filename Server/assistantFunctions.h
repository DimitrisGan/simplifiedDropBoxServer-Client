//
// Created by dimitrisgan on 26/2/2019.
//

#ifndef SERVER_ASSISTANTFUNCTIONS_H
#define SERVER_ASSISTANTFUNCTIONS_H

#include "ErrorsCodes.h"
#include "mylinkedList.h"
#include "myString.h"

#include <stdio.h>
#include <sys/wait.h>	     /* sockets */
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <netdb.h>	         /* gethostbyaddr */
#include <unistd.h>	         /* fork */
#include <stdlib.h>	         /* exit */
#include <ctype.h>	         /* toupper */
#include <signal.h>          /* signal */
#include <arpa/inet.h>


#include <cstring>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include<fcntl.h>


struct ArgumentsKeeper{

    myString portNumber;

    ArgumentsKeeper();

    ArgumentsKeeper(const ArgumentsKeeper &right); //move operator maybe not needed because of not existing simple constructor


};



void print_ip(unsigned int ip);

void perror_exit(const char *message);

int remove_directory(const char *path);



bool removeFile(const char *path);
myString getPath(const myString &dirName, const myString &file );

void createDirectory(char* path );
int is_regular_file(const char *path);
void list_dir(const char *path, linkedList<myString> &listDirList);
void listIdFiles(const char *path, linkedList<myString> &idFilesInDir);
void addFileIdInCommon(const struct ArgumentsKeeper &argmKeeper); //1B

bool fileExist(char* path);

void argmParser(int &argc, char **argv, struct ArgumentsKeeper &argmKeeper);

void split(char *str, const char *delimiter, linkedList<char *> &result2return) ;

void removeFirst(char * str, const char * toRemove); // https://codeforwin.org/2015/12/c-program-to-remove-first-occurrence-of-word-from-string.html

bool isNumber(char* s);

char * getline() ;



//========BELOW are not used=======
char * str_slice(char str[], int slice_from, int slice_to);

void trimNoise(char* str); //noise considered '\n' ,whitespace and '\r'


#endif //SERVER_ASSISTANTFUNCTIONS_H
