#include <iostream>

//#include <sys/socket.h>             /* sockets */
//#include <sys/types.h>              /* sockets */
//#include <netinet/in.h>             /* Internet sockets */
//#include <arpa/inet.h>              /* for hton */


#include <stdio.h>
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <unistd.h>          /* read, write, close */
#include <netdb.h>	         /* gethostbyaddr */
#include <stdlib.h>	         /* exit */
#include <string.h>	         /* strlen */
#include <pthread.h>   /* For threads  */


#include "assistantFunctions.h"
#include "socketManipulation.h"
#include "clientProtocol.h"



using namespace std;

int read_from_others_requests_and_respond(int filedes, Protocol &prot ,CS &shared);


static volatile sig_atomic_t quitFlag = 0;
pthread_mutex_t quitMain_mtx;         /* Mutex for synchronization */




//looks for ctrl-c which has a value of 2
void catchinterrupt(int signo){

    if (signo == SIGINT ) {
        quitFlag=1;

        quitThread =1;




    }

}




int main(int argc, char **argv) {

    static struct sigaction act;
    act.sa_handler=catchinterrupt;
    act.sa_flags = SA_SIGINFO;
    sigfillset(&(act.sa_mask));
    sigaction(SIGINT, &act, NULL);


    ArgumentsKeeper argmKeeper;
    argmParser(argc, argv , argmKeeper);

//    argmKeeper.p`rintArgs();

    circularBuffer *circBuf = new circularBuffer(argmKeeper.bufSize);
    CS shared(circBuf,argmKeeper.inDir);

    linkedList<myString> allFilesInInDir_list;

    list_all_in_dir(argmKeeper.inDir, allFilesInInDir_list);

    Protocol prot(argmKeeper);

    int     i;

    uint16_t serverPort,listenPort;
    serverPort = static_cast<uint16_t>(argmKeeper.serverPort.to_int());
    listenPort = static_cast<uint16_t>(argmKeeper.portNum.to_int());


    //----------------- SEND LOG_ON & GET_CLIENTS TO SERVER   ------------------------
    int sock_writes_to_server_LOG_ON = create_socket_and_connect(argmKeeper.serverIp,serverPort);

    prot.send_LOG_ON(sock_writes_to_server_LOG_ON);

    int sock_writes_to_server_GET_CLIENTS = create_socket_and_connect(argmKeeper.serverIp,serverPort);

    prot.send_GET_CLIENTS(sock_writes_to_server_GET_CLIENTS);


    //----------------- FROM HERE CREATE A NEW SOCKET FOR LISTENING   ------------------------


    int sock_to_listen = make_socket_and_bind(listenPort);


    /* Listen for connections */

    if (listen(sock_to_listen, 5) < 0) perror_exit("listen");
    printf("Listening for connections to port %d\n", listenPort);



    fd_set active_fd_set, read_fd_set;
    struct sockaddr_in other;
    socklen_t size;



    //----------------- THREADS INITIALIZATION  ------------------------

    int num_thr= argmKeeper.workerThreads.to_int();
    pthread_t workerThrsIds_array[num_thr];

    for (i=0 ; i<num_thr ; i++) {
        if (pthread_create(workerThrsIds_array+i, NULL, &worker_function,  &shared)) {/* Create a thread */
            perror_exit("pthread_create");
            cout << workerThrsIds_array[i]<<endl;
        }

    }





/* Initialize the set of active sockets. */

    FD_ZERO (&active_fd_set);
    FD_SET (sock_to_listen, &active_fd_set);


    while (true) {

        if (quitFlag){
            break;
        }


/* Block until input arrives on one or more active sockets. */

        read_fd_set = active_fd_set;
        int select_retval = select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL);

        if (select_retval == EINTR || quitFlag){
            break;
        }


        /* Service all the sockets with input pending. */

        for (i = 0; i < FD_SETSIZE; ++i) {

            if (FD_ISSET (i, &read_fd_set)) {
                if (i == sock_to_listen) {

                    /* Connection request on original socket. */
                    int newsock;
                    size = sizeof(other);
                    newsock = accept(sock_to_listen, (struct sockaddr *) &other, &size);

//                    fprintf(stderr,"Other: connect from host %s, port %d.\n", inet_ntoa(other.sin_addr), ntohs(other.sin_port));
                    FD_SET (newsock, &active_fd_set);
                } else {

                    if (quitFlag){
                        break;
                    }
                    /* Data arriving on an already-connected socket. */

                    if (read_from_others_requests_and_respond(i, prot, shared) < 0) {
                        close(i);
                        FD_CLR (i, &active_fd_set);
                    }
                }
            }
        }


    }

    //----------------- CLEAN UP EVERYTHING LEFT    ------------------------
    for (i = 0; i < FD_SETSIZE; ++i) {
        close(i);
        FD_CLR (i, &active_fd_set);
    }
    FD_ZERO (&active_fd_set);

    close(sock_to_listen);

    /*from here send LOG_OFF and terminate*/

    //----------------- SEND LOG_OFF & TERMINTATE   ------------------------

    int sock_writes_to_server_LOG_OFF = create_socket_and_connect(argmKeeper.serverIp,serverPort);
    prot.send_LOG_OFF(sock_writes_to_server_LOG_OFF);



    //----------------- WAIT TO FINISH ALL THREADS  ------------------------

    if (pthread_mutex_lock(&shared.stop_mtw))  /* Lock mutex */
        perror_exit("pthread_mutex_lock");

    shared.stop = true;

    if (pthread_mutex_unlock(&shared.stop_mtw))  /* Unlock mutex */
        perror_exit("pthread_mutex_unlock");




    for (auto &thrId : workerThrsIds_array) {

        if (pthread_cancel(thrId))
            perror_exit("pthread_cancel");

        pthread_kill(thrId,9);

//        if (pthread_join(thrId, nullptr))
//            perror_exit("pthread_join");

    }

    delete circBuf;circBuf= nullptr;


    return 0;


}




//=================================================================


int read_from_others_requests_and_respond(int filedes, Protocol &prot , CS &shared)
{


    char buf[1];
    myString whitespace(" ");
    myString instruction("");

    /*messages received from server*/
    bool flagUSER_ON     = false;
    bool flagCLIENT_LIST = false;
    bool flagUSER_OFF    = false;

    /*messages received from other clients-workers*/
    bool flagGET_FILE_LIST = false;
    bool flagGET_FILE = false;


    while(read(filedes, buf, 1) > 0) {  /* Receive 1 char */

        instruction += buf;


        if (instruction == "USER_ON") {
            flagUSER_ON = true;
            break;
        }

        if (instruction == "CLIENT_LIST") {
            flagCLIENT_LIST = true;
            break;
        }

        if (instruction == "USER_OFF") {
            flagUSER_OFF = true;
            break;
        }

        if (instruction == "1_GET_FILE_LIST") { //TODO CHANGE IT!!
            flagGET_FILE_LIST = true;
            break;
        }

        if (instruction == "GET_FILE") {
            flagGET_FILE = true;
            break;
        }


    }

//    cout << "INFO::Instruction read = " << instruction << endl;


    if (flagUSER_ON) {
        clientsTuple newClient;
        prot.recv_USER_ON(filedes,newClient);
        prot.add_client(newClient,shared);

//        cout <<"Printing the list after USER_ON: \t";
//        cout << shared.clients_list<<endl;

    }

    if (flagCLIENT_LIST){
        linkedList <clientsTuple> existingClients_list;
        prot.recv_CLIENTS_LIST(filedes ,existingClients_list );
        prot.add_list_of_existing_clients(existingClients_list,shared);

//        cout <<"Printing the list after CLIENT_LIST: \t";
//        cout << shared.clients_list<<endl;

    }

    if (flagUSER_OFF) {
        clientsTuple client2remove;
        prot.recv_USER_OFF(filedes,client2remove);
        prot.remove_client(client2remove,shared);

//        cout <<"Printing the list after USER_OFF: \t";
//        cout << shared.clients_list<<endl;

    }

    if(flagGET_FILE_LIST){

        prot.recv_GET_FILE_LIST(filedes); //we dont care which worker-client sent it
        //I will send from the same socket the info
        prot.respond_with_FILE_LIST(filedes);

    }

    if(flagGET_FILE){

        prot.recv_GET_FILE(filedes);
        prot.respond_to_GET_FILE(filedes);

    }


    return -1;

}
