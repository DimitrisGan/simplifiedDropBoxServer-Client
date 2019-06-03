//
// Created by dimitrisgan on 31/5/2019.
//

#include <assert.h>
#include "criticalSection.h"
#include "socketManipulation.h"



CS::CS(circularBuffer *circBuffer,  myString inputDir) : circBuffer(circBuffer), inputDir(inputDir) {

    pthread_mutex_init(&this->client_list_mtx, NULL);

}


CS::~CS() {
    if (pthread_mutex_destroy(&this->client_list_mtx)) { /* Destroy mutex */
        perror_exit("pthread_mutex_destroy");
    }
}



void* worker_function(void* shared){

    thread_protocol thr;
//    int test= ((CS *)shared))

    while (true/*quitThread == 0*/) {


        info cbuff_item;


        cout << "CIRCLED BUFFER SIZE IS : "<< ((CS *)shared)->circBuffer->size()<<endl;


        cbuff_item = ((CS *)shared)->circBuffer->obtain();


        cout <<"to cbuff_item exei times "<<cbuff_item<<endl;





        if (cbuff_item.isNewClient()){


            myString newClientsDir; newClientsDir  = createNewDirName(cbuff_item.ip ,cbuff_item.port);
            myString newClientsDirPath;newClientsDirPath = createPathForNewDir(  ((CS *)shared)->inputDir ,newClientsDir);

            ///todo mhpws thelei na tsekarw an yparxei hdh exist(newClientsDirPath)
            createDirectory(newClientsDirPath.getMyStr());



            cout <<"WORKER::new Dir for new Client created in Path = "<<newClientsDirPath<<endl;


            linkedList <info> newItems2place_list;


            thr.send_GET_FILE_LIST_and_recv_FILE_LIST(cbuff_item.ip , cbuff_item.port,
                                                      newItems2place_list);

            //add the prefix ip_port/.. in all files
            myString prefix = newClientsDirPath; prefix+="/";

            cout<<newItems2place_list<<endl;
            for (auto &item : newItems2place_list) {

//                addPrefix2list_of_file(item.pathName);
                myString realPath = prefix; realPath+=item.pathName;
                item.setPathName(realPath);


                if (! fileExist(realPath.getMyStr())){
                    item.setVersion(0);
                }


                ((CS *)shared)->circBuffer->place(item);

            }

        }

        if (cbuff_item.isFilePath()){

            thr.send_GET_FILE_and_recv(cbuff_item,((CS *)shared)->inputDir);

        }





//    pthread_exit(nullptr);
    }
    cout << "thread exits!!!!!!!!\n";

    pthread_exit(0);

}

myString createNewDirName(uint32_t ipB, uint16_t portB) {
    myString ipStr; ipStr = convertBinaryIpToString(ipB);
    myString portStr ; portStr = convertBInaryPortToString(portB);
    myString newClientsDirName = ipStr; newClientsDirName+="_";newClientsDirName+= portStr ;

    return newClientsDirName;
}

myString createPathForNewDir(myString inDir, myString nameNewDir) {
    myString newClientsDirPath;newClientsDirPath=inDir;
    newClientsDirPath+="/";
    newClientsDirPath += nameNewDir;


    return newClientsDirPath;
}



// This assumes buffer is at least x bytes long,
// and that the socket is blocking.
void ReadXBytes(int socket, unsigned int x, void* buffer ,const char* error_m)
{
    int bytesRead = 0;
    int result;
    while (bytesRead < x)
    {
        result = static_cast<int>(read(socket, buffer + bytesRead , x - bytesRead));

        if (result < 1 )
        {
            // Throw your error.
            perror_exit(error_m);

        }

        bytesRead += result;
    }
}

void getAllHigherPaths(myString path2break, linkedList<myString> &retAllPaths_list) {

    myString subPath("");/*subPath = path2break.getMyStr()[0];*/
    for (int i = 0; i < path2break.size(); ++i) {

        if (path2break.getMyStr()[i] != '/' ){
            myString ch(path2break.getMyStr()[i]);
            subPath+= ch;
        }
        if (path2break.getMyStr()[i] == '/' ){
            myString ch(path2break.getMyStr()[i]);

            retAllPaths_list.insert_last(subPath);
            subPath+=ch;
        }
    }
}


void
thread_protocol::send_GET_FILE_LIST_and_recv_FILE_LIST(uint32_t ipB, uint16_t portB, linkedList<info> &retNewPaths_list) {
    myString getFileList("1_GET_FILE_LIST");

    myString ipStr; ipStr = convertBinaryIpToString(ipB);

    int sock = create_socket_and_connect(ipStr, portB);


    cout<< "INFO_CLIENT-WORKER!::Send GET_FILE_LIST from send_GET_FILE_LIST_and_recv_FILE_LIST  to other client\n";


    if (write(sock, getFileList.getMyStr() ,getFileList.size()) < 0)
        perror_exit("write i-th file_path_name in FILE_LIST");

    myString FILE_LIST("FILE_LIST");
    char ass[9];
    if (read(sock, ass, FILE_LIST.size()) < 0) //todo needs while () defensive programming
        perror_exit("read i-th ipB in CLIENTS_LIST");

    assert(FILE_LIST == ass);

    int n;
    if (read(sock, &n, sizeof(n)) < 0) //todo needs while () defensive programming
        perror_exit("read i-th ipB in CLIENTS_LIST");


    for (int i = 0; i < n ; ++i) {
        char buf[128];
//        if (read(sock, buf, 128) < 0) //todo needs while () defensive programming
//            perror_exit("read i-th ipB in CLIENTS_LIST");
        ReadXBytes(sock,128,buf,"read i-th ipB in CLIENTS_LIST");
        myString pathName(buf);

        unsigned version;
        if (read(sock, &version, sizeof(unsigned)) < 0) //todo needs while () defensive programming
            perror_exit("read i-th ipB in CLIENTS_LIST");



        info info2add(ipB,portB,pathName,version);

        retNewPaths_list.insert_last(info2add);
        //todo na ftiaksw buffer pou na ta xwnei mesa + na paizw me ta versions =0 an den uparxei hdh

        //TODO
//        if (fileExist(this.)) //todo tha to valw sth worker synarthsh kai apo kei tha tsekarw an uparxei hdh gia na allaksw to version
    }


    close(sock);
}

void thread_protocol::send_GET_FILE_and_recv(info item,myString inDir) {

    cout << "seg #1\n";
    myString getFileList("GET_FILE");

    myString prefix;prefix = createNewDirName(item.ip,item.port);

    myString path2send  = item.pathName;
    path2send.removeSubstr(inDir);path2send.removeSubstr("/");path2send.removeSubstr(prefix);
    myString ipStr; ipStr = convertBinaryIpToString(item.ip);




    int sock = create_socket_and_connect(ipStr, item.port);



    if (write(sock, getFileList.getMyStr() ,getFileList.size()) < 0)
        perror_exit("write GET_FILE in GET_FILE");


    if (write(sock, path2send.getMyStr() ,128) < 0)
        perror_exit("write file_path_name in GET_FILE");


    if (! fileExist(item.pathName.getMyStr()) ) { //if the file doesn't exist send version = 0 to obtain the file


        unsigned zero =0;
        if (write(sock, &zero, sizeof(item.version)) < 0)
            perror_exit("write version in GET_FILE");
    }

    cout << "seg #2\n";

    linkedList <myString> allPaths;
    getAllHigherPaths( item.pathName,allPaths);

    cout << "EDWWWWWWWWWW \t "<<allPaths<<endl;


    //-------------------- RECEIVE --------------------
    //todo FILE_SIZE
    //todo FILE_NOT_FOUND
    //todo FILE_UP_TO_DATE

    char buf[1];
    myString instruction("");
    bool flagFILE_UP_TO_DATE    = false;
    bool flagFILE_SIZE          = false;
    bool flagFILE_NOT_FOUND     = false;

    while(read(sock, buf, 1) > 0) {  /* Receive 1 char */
//        printf("diavasa %d bytes\n", ++diavasa);

        instruction += buf;

//        cout << "To instruction exei timh = " << instruction << endl;

        if (instruction == "FILE_UP_TO_DATE") {
            flagFILE_UP_TO_DATE = true;
            break;
        }

        if (instruction == "FILE_SIZE") {
            flagFILE_SIZE = true;
//            cout << instruction;
            break;
        }

        if (instruction == "FILE_NOT_FOUND") {
            flagFILE_NOT_FOUND = true;
//            cout << instruction;
            break;
        }


    }

    if (flagFILE_UP_TO_DATE){
        cout<< "INFO_CLIENT-WORKER::Received FILE_UP_TO_DATE for file: "<<item.pathName<<endl;
    }

    if (flagFILE_NOT_FOUND){
        cout<< "INFO_CLIENT-WORKER::Received FILE_NOT_FOUND for file: "<<item.pathName<<endl;
    }

    if (flagFILE_SIZE){

        unsigned versionGiven;
        if (read(sock, &versionGiven, sizeof(versionGiven)) < 0) //todo needs while () defensive programming
            perror_exit("read version in GET_FILE");

        unsigned size;
        if (read(sock, &size, sizeof(unsigned)) < 0) //todo needs while () defensive programming
            perror_exit("read size in GET_FILE");



        if (versionGiven == 1 && size == 0){ //is a dir
            createDirectory(item.pathName.getMyStr());
        }

        else{ //is a file

            char contentBuf[size+1];
            memset(contentBuf, 0, sizeof(contentBuf));

            ReadXBytes(sock, size, contentBuf ,"read content in GET_FILE");

//            printf ("Content is : %s \n",contentBuf);


//            cout << "Content buffer pou diavasa einia = "<<contentBuf<<endl;
            //create the file
            FILE *fp;
            /*writes in received file*/
            fp = fopen(item.pathName.getMyStr(), "w"); //creates OR appends the file in mirror dir
            fprintf(fp, "%s", contentBuf); //writes the content of the file
            fclose(fp);




        }






    }



    close(sock);

}
