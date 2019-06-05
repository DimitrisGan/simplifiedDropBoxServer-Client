//
// Created by dimitrisgan on 26/2/2019.
//

#ifndef ASSISTANTFUNCTIONS_H
#define ASSISTANTFUNCTIONS_H

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




struct ArgumentsKeeper{
    myString inDir;
    myString portNum;
    myString workerThreads;
    int bufSize;
    myString serverPort;
    myString serverIp;

    void printArgs();

    ArgumentsKeeper();


};

bool is_dir(const char* path);
void list_all_in_dir(myString path, linkedList<myString> &listDirList);
void list_dir(const char *path, linkedList<myString> &listDirList);

int loadContextOfFile(myString path ,myString &context); /*returns the file in bytes*/

void checkHostName(int hostname);
void checkHostEntry(struct hostent * hostentry);

void perror_exit(const char *message);

int remove_directory(const char *path);

void createDirectory(char* path );
int is_regular_file(const char *path);
bool directoryExist(char *pathToDir);

bool fileExist(char* path);

void argmParser(int &argc, char **argv, struct ArgumentsKeeper &argmKeeper);


bool isNumber(char* s);


//========BELOW are not used=======
void print_ip(unsigned int ip);

void checkIPbuffer(char *IPbuffer);

void split(char *str, const char *delimiter, linkedList<char *> &result2return) ;

void removeFirst(char * str, const char * toRemove); // https://codeforwin.org/2015/12/c-program-to-remove-first-occurrence-of-word-from-string.html

char * getline() ;

//==============================
char * str_slice(char str[], int slice_from, int slice_to);

void trimNoise(char* str); //noise considered '\n' ,whitespace and '\r'


#endif //ASSISTANTFUNCTIONS_H
