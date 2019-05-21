//
// Created by dimitrisgan on 26/2/2019.
//

#include <cstring>
#include <fstream>
#include "assistantFunctions.h"
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include<fcntl.h>
# include <unistd.h>
#include <sys/types.h>

# define PERMS 0644 //set access p e r m i s s i o n s




/* Call unlink or rmdir on the path, as appropriate. */
int remove_directory(const char *path)
{
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (d)
    {
        struct dirent *p;
        r = 0;

        while (!r && (p=readdir(d)))
        {
            int r2 = -1;
            char *buf;
            size_t len;

            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
            {
                continue;
            }
            len = path_len + strlen(p->d_name) + 2;
            buf = (char*)malloc(len);
            if (buf)
            {
                struct stat statbuf;
                snprintf(buf, len, "%s/%s", path, p->d_name);
                if (!stat(buf, &statbuf))
                {
                    if (S_ISDIR(statbuf.st_mode))
                    {
                        r2 = remove_directory(buf);
                    }
                    else
                    {
                        r2 = unlink(buf);
                    }
                }
                free(buf);
            }
            r = r2;
        }
        closedir(d);
    }
    if (!r)
    {
        r = rmdir(path);
    }
    return r;
}


void getNewlyAddedIdsList(linkedList<myString> newIdsFilesList ,linkedList<myString>& newIdsList){

    for (auto idFile : newIdsFilesList) {
        idFile.removeSubstr(".id");
        newIdsList.insert_last(idFile);
    }
}




void trackNewIdFiles(linkedList<myString> prevStateFilesList, linkedList<myString> currStateFileList,
                     linkedList<myString> &newFilesList, const myString &filename) {

    for ( auto &file : currStateFileList) {
        if (! prevStateFilesList.exists(file) && file!= filename){ //if ti doesn't exist then push it in the newFilesList
            newFilesList.insert_last(file);
        }
    }
}



bool removeFile(const char *path){
    if (remove(path) == 0) {
        printf("Deleted successfully!\n");
        return false;
    }
    else{
        printf("Unable to delete the file\n");
        return true;
    }
}

void cropBullets(linkedList<myString> &listDirList){

    myString currDir(".");
    myString prevDir("..");

    listDirList.deleteNodeByItem(currDir);
    listDirList.deleteNodeByItem(prevDir);

}

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}


void list_dir(const char *path, linkedList<myString> &listDirList) {
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        myString file(entry->d_name);
        listDirList.insert_last(file);
    }
    closedir(dir);

    cropBullets(listDirList);

}


bool isIdFile(const myString &fileName){
    myString ext(".id");
    return fileName.substrExist(ext);
}

bool isFiFoFile(const myString &fileName){
    myString ext(".fifo");
    return fileName.substrExist(ext);
}

void listIdFiles(const char *path, linkedList<myString> &idFilesInDir) {

    linkedList<myString> listDirList;

    list_dir(path, listDirList);

    for ( auto &fileName : listDirList) {
        if (isIdFile(fileName)){
            idFilesInDir.insert_last(fileName);
        }
    }

}


    myString getPath(const myString &dirName, const myString &file ) {

    myString slash("/");
//    myString dot(".");
    myString path ;
    path =/*slash +*/ dirName ;path+= slash + file  ;
    return path;
}


void addFileIdInCommon(const struct ArgumentsKeeper &argmKeeper){
/*

    myString extension(".id");

    myString filename;myString pathToCreate;
    filename = argmKeeper.id ;
    filename+= extension;
    pathToCreate =  getPath(argmKeeper.commonDir , filename);
    int filedes;

    if (fileExist(pathToCreate.getMyStr())){ //check if file *.id already exists

        std::cerr << "ERROR FILE: "<<pathToCreate<<" ALREADY EXISTS"<<endl;
        exit(FILE_ID_ALREADY_EXIST);

    }

    if (( filedes = open(pathToCreate.getMyStr(), O_WRONLY|O_CREAT|O_TRUNC, PERMS) ) == -1) {
        perror ( " creating " ) ;
        exit (1) ;
    }
    else{
//        printf ( " Managed to get to the file successfully \n" ) ;

    }



    //todo write inside file and put it in the right directory



    pid_t procId = getpid () ;
    cout <<"Client #" <<argmKeeper.id << " has process Id #" << procId<<endl;

    write ( filedes , &procId , sizeof(procId) ) ;

    close(filedes);
*/

}



bool fileExist(char* path){

    if( access( path, F_OK ) != -1 ) {
        // file exists
        return true;
    } else {
        // file doesn't exist
        return false;
    }
}
//===========================================
void createDirectory(char* path ){
//    int mkdir(const char *pathname, mode_t mode); from man 2 mkdir

    int result = mkdir(path, 0777);
    if (result){
        std::cerr << "ERROR IN CREATING THE DIRECTORY :"<< path <<endl;
        exit(1);
    }

}

bool directoryExist(char *pathToDir){
    DIR* dir = opendir(pathToDir);
    if (dir)
    {
        /* Directory exists. */
        closedir(dir);
        return true;
    }
    else if (ENOENT == errno)
    {
        /* Directory does not exist. */
        return  false;
    }
    else
    {
        std::cerr << "ERROR IN OPENING THE FILE :"<< pathToDir <<endl;
        exit(1);
        /* opendir() failed for some other reason. */
    }

}




void handleInputDirectories(const struct ArgumentsKeeper &argmKeeper){

  /*  bool flagInputDir = directoryExist(argmKeeper.inDir.getMyStr());
    bool flagCommon = directoryExist(argmKeeper.commonDir.getMyStr());
    bool flagMirror = directoryExist(argmKeeper.mirrorDir.getMyStr());

    if (!flagInputDir){
        std::cerr << "INPUT DIRECTORY ["<< argmKeeper.inDir <<"] DOESNT EXIST"<<endl;
        exit(INPUT_DIR_NOT_EXIST);
    }

    if (flagMirror){
        std::cerr << "MIRROR DIRECTORY ["<< argmKeeper.mirrorDir <<"] EXIST ALREADY"<<endl;
        std::cerr << "THERE IS MIGHT ANOTHER CLIENT THAT HAS ALREADY CREATED IT AND USES IT"<<endl;
        exit(MIRROR_DIR_ALREADY_EXIST);
    }

    createDirectory(argmKeeper.mirrorDir.getMyStr()); //create mirror directory

    if (flagCommon == 0){ //if common dir doesnt exist [which means is the first client]
        createDirectory(argmKeeper.commonDir.getMyStr()); //create it

    }
*/

}


void argmParser(int &argc, char **argv, struct ArgumentsKeeper &argmKeeper){

    bool flagDirName=false;
    bool flagPortNum=false;
    bool flagWorkerThreads=false;
    bool flagbufferSize=false;
    bool flagServerPort=false;
    bool flagServerIp=false;

    int i=1;
    while (i < argc ) {
        if (argv[i] == nullptr ){break;}

        if(strcmp(argv[i],"-d")==0 && !flagDirName && argv[i + 1] != nullptr){
            argmKeeper.inDir = argv[++i];
            flagDirName = true;
            i++;
            continue;
        }

        if(strcmp(argv[i],"-p")==0 && !flagPortNum && argv[i + 1] != nullptr){
            argmKeeper.portNum = argv[++i];
            flagPortNum = true;
            i++;
            continue;
        }

        if(strcmp(argv[i],"-w")==0 && !flagWorkerThreads && argv[i + 1] != nullptr){
            argmKeeper.workerThreads = argv[++i];
            flagWorkerThreads = true;
            i++;
            continue;
        }

        if(strcmp(argv[i],"-b")==0 && !flagbufferSize && argv[i + 1] != nullptr){
            if (! isNumber(argv[++i])) {fprintf(stderr, "ARGUMENT : %s IS NOT A NUMBER\n", argv[i]);exit(UNKNOWN_CMDARGUMENT);}
            argmKeeper.bufSize = atoi(argv[i]);
            flagbufferSize = true;
            i++;
            continue;
        }

        if(strcmp(argv[i],"-sp")==0 && !flagServerPort && argv[i + 1] != nullptr){
//            if (! isNumber(argv[++i])) {fprintf(stderr, "ARGUMENT : %s IS NOT A NUMBER\n", argv[i]);exit(UNKNOWN_CMDARGUMENT);}
//            argmKeeper.serverPort = atoi(argv[i]);
            argmKeeper.serverPort = argv[++i];
            flagServerPort = true;
            i++;
            continue;
        }

        if(strcmp(argv[i],"-sip")==0 && !flagServerIp && argv[i + 1] != nullptr){
            argmKeeper.serverIp = argv[++i];
            flagServerIp = true;
            i++;
            continue;
        }

        fprintf(stderr, "Unknown argument OR not given argument for flag: %s\n", argv[i]);
        exit(UNKNOWN_CMDARGUMENT);

    }


    if (!flagDirName){
        cout << "Provide directory name : ";
        cin >> argmKeeper.inDir ;

    }
    if (!flagPortNum){

        cout << "Provide port number: ";
        cin >> argmKeeper.portNum;
    }
    if (!flagWorkerThreads){
        cout << "Provide number of worker threads: ";
        cin >> argmKeeper.workerThreads;
    }
    if (!flagbufferSize){
        cout << "Provide buffer Size : ";
        cin >> argmKeeper.bufSize;
    }
    if (!flagServerPort){
        cout << "Provide server port : ";
        cin >> argmKeeper.serverPort;
    }
    if (!flagServerIp ){
        cout << "Provide server ip : ";
        cin >> argmKeeper.serverIp;

    }

}




void split(char *str, const char *delimiter, linkedList<char *> &result2return) {

    char* token ;

    token = strtok(str , delimiter);

    while (token!= nullptr){

        if (token[0] == '\n')
            token++ ;
        if (token[strlen(token) -1] == '\n')
            token[strlen(token)-1]= '\0';
        if (!(token[0] == ' ' && strlen(token) == 1)) //means that is not a whitespace
            result2return.insert_last(token);

        token = strtok(nullptr, delimiter);

    }
}


void removeFirst(char * str, const char * toRemove)
{
    int i, j;
    int len, removeLen;
    int found = 0;

    len = strlen(str);
    removeLen = strlen(toRemove);

    for(i=0; i<len; i++)
    {
        found = 1;
        for(j=0; j<removeLen; j++)
        {
            if(str[i+j] != toRemove[j])
            {
                found = 0;
                break;
            }
        }

        /* If word has been found then remove it by shifting characters  */
        if(found == 1)
        {
            for(j=i; j<=len-removeLen; j++)
            {
                str[j] = str[j + removeLen];
            }

            // Terminate from loop so only first occurrence is removed
            break;
        }
    }
}


bool isNumber(char* s)
{
    for (int i = 0; i < strlen(s); i++)
        if (isdigit(s[i]) == 0)
            return false;

    return true;
}


char * getline() {
    char * line = (char*)malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = (char*)realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}


//========BELOW are not used=======

/**
 * Extracts a selection of string and return a new string or NULL.
 * It supports both negative and positive indexes.
 */
char * str_slice(char str[], int slice_from, int slice_to)
{
    // if a string is empty, returns nothing
    if (str[0] == '\0')
        return NULL;

    char *buffer;
    size_t str_len, buffer_len;

    // for negative indexes "slice_from" must be less "slice_to"
    if (slice_to < 0 && slice_from < slice_to) {
        str_len = strlen(str);

        // if "slice_to" goes beyond permissible limits
        if (abs(slice_to) > str_len - 1)
            return NULL;

        // if "slice_from" goes beyond permissible limits
        if (abs(slice_from) > str_len)
            slice_from = (-1) * str_len;

        buffer_len = slice_to - slice_from;
        str += (str_len + slice_from);

        // for positive indexes "slice_from" must be more "slice_to"
    } else if (slice_from >= 0 && slice_to > slice_from) {
        str_len = strlen(str);

        // if "slice_from" goes beyond permissible limits
        if (slice_from > str_len - 1)
            return NULL;

        buffer_len = slice_to - slice_from;
        str += slice_from;

        // otherwise, returns NULL
    } else
        return NULL;

    buffer =(char*) calloc(buffer_len, sizeof(char));
    strncpy(buffer, str, buffer_len);
    return buffer;
}


//not working
void trimNoise(char* str){ //noise considered '\n' ,whitespace and '\r'
    if (str[0] == '\n' || str[0] == ' '  )
        str++;   //shift one letter to avoid " " or "\n"
    if (str[strlen(str) -1] == '\n')
        str[strlen(str)-1]= '\0';
}



void ArgumentsKeeper::printArgs() {

    cout << "~~~~   Arguments Given    ~~~~~"<<endl;
    cout << "dirName (-d) :\t" << inDir <<endl;
    cout << "portNum (-p) :\t" << portNum <<endl;
    cout << "workerThreads (-w) :\t" << workerThreads <<endl;
    cout << "buffer Size (-b) :\t" << bufSize <<endl;
    cout << "serverPort (-sp) :\t" << serverPort <<endl;
    cout << "serverIp (-sip) :\t" << serverIp <<endl;
}



ArgumentsKeeper::ArgumentsKeeper() {}

ArgumentsKeeper::ArgumentsKeeper(const ArgumentsKeeper &right) {

}

