//
// Created by dimitrisgan on 26/2/2019.
//

#ifndef SIMPLIFIEDDROPBOX_ASSISTANTFUNCTIONS_H
#define SIMPLIFIEDDROPBOX_ASSISTANTFUNCTIONS_H

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

//    ArgumentsKeeper(const ArgumentsKeeper &right); //move operator maybe not needed because of not existing simple constructor


};



void print_ip(unsigned int ip);

void checkHostName(int hostname);
void checkHostEntry(struct hostent * hostentry);
void checkIPbuffer(char *IPbuffer);

myString zip_it(myString IP , myString port);

void perror_exit(const char *message);


int remove_directory(const char *path);

void getNewlyAddedIdsList(linkedList<myString> newIdsFilesList ,linkedList<myString>& newIdsList);

void trackNewIdFiles(linkedList<myString> prevStateFilesList, linkedList<myString> currStateFileList,
                     linkedList<myString> &newFilesList, const myString &filename);
bool removeFile(const char *path);
myString getPath(const myString &dirName, const myString &file );

void createDirectory(char* path );
int is_regular_file(const char *path);
void list_dir(const char *path, linkedList<myString> &listDirList);
void listIdFiles(const char *path, linkedList<myString> &idFilesInDir);
void addFileIdInCommon(const struct ArgumentsKeeper &argmKeeper); //1B

void handleInputDirectories(const struct ArgumentsKeeper &argmKeeper); //1A
bool fileExist(char* path);

void argmParser(int &argc, char **argv, struct ArgumentsKeeper &argmKeeper);


void split(char *str, const char *delimiter, linkedList<char *> &result2return) ;


void removeFirst(char * str, const char * toRemove); // https://codeforwin.org/2015/12/c-program-to-remove-first-occurrence-of-word-from-string.html

bool isNumber(char* s);


char * getline() ;



//========BELOW are not used=======
char * str_slice(char str[], int slice_from, int slice_to);

void trimNoise(char* str); //noise considered '\n' ,whitespace and '\r'


#endif //SIMPLIFIEDDROPBOX_ASSISTANTFUNCTIONS_H
