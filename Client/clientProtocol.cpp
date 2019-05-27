//
// Created by dimitrisgan on 25/5/2019.
//

#include "clientProtocol.h"

Protocol::Protocol(ArgumentsKeeper args) : args(args) {}


int Protocol::send_LOG_ON(int sock) {
    char hostbuffer[256];
    char *IPbuffer;
    struct hostent  *mymachine;
    int hostname;

    // To retrieve hostname
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    cout << "hostname = "<< hostbuffer<<endl;
    checkHostName(hostname);

    // To retrieve host information
    mymachine = gethostbyname(hostbuffer);
    checkHostEntry(mymachine);


    // To convert an Internet network
    // address into ASCII string
    /*
     * IPbuffer = inet_ntoa(*((struct in_addr*)
            mymachine->h_addr_list[0]));


    printf("Hostname: %s\n", hostbuffer);
    printf("Host IP: %s\n", IPbuffer);

     */

    /*char hostname [50]*/ char symbolicip [50];
    struct in_addr **addr_list = nullptr;


    printf ( " Name To Be Resolved : %s \n " , mymachine -> h_name ) ;
    printf ( " Name Length in Bytes : % d \n " , mymachine -> h_length ) ;
    addr_list = ( struct in_addr **) mymachine -> h_addr_list ;
    for ( int i = 0; addr_list [ i ] != NULL ; i ++) {
        strcpy ( symbolicip , inet_ntoa (* addr_list [ i ]) ) ;
        printf ( " %s resolved to %s \n " , mymachine -> h_name , symbolicip ) ;
    }

    cout << " edw koita megale :"<<( struct in_addr **) mymachine -> h_addr_list[0]<<endl;


    myString logOn("LOG_ON");

//    instruction2send+= zip_it(this->args.)



    if (write(sock,logOn.getMyStr(), logOn.size()) < 0)
        perror_exit("write LOG ON");

    uint32_t ipInbinary = htonl(addr_list[0]->s_addr);

    cout << " EDWWWW :"<<ipInbinary<<endl;

    print_ip(addr_list[0]->s_addr);

    if (write(sock, &ipInbinary , sizeof(uint32_t)) < 0)
        perror_exit("write IP");


    uint16_t portBinary = htons(static_cast<uint16_t>(this->args.portNum.to_int()));

    if (write(sock, &portBinary , sizeof(uint16_t)) < 0)
        perror_exit("write PORT");


//    close(sock); //todo mporei na fugei apo dw
    return 0;
}


int Protocol::send_GET_CLIENTS(int sock) {
    myString getClients("GET_CLIENTS");


    if (write(sock,getClients.getMyStr(), getClients.size()) < 0)
        perror_exit("write GET_CLIENTS");


    return 0;
}

int Protocol::send_LOG_OFF(int sock) {

    myString logOff("LOG_OFF");

    if (write(sock,logOff.getMyStr(), logOff.size()) < 0)
        perror_exit("write LOG_OFF");



    return 0;
}

int Protocol::recv_CLIENTS_LIST() {

    //todo prosoxh! na kanw ntoh() otan lavw ip kai port
    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////



int
read_from_others (int filedes ,Protocol &prot )
{
    int diavasa=0;

//    char buffer[MAXMSG];
//    int nbytes;
//    nbytes = read (filedes, buffer, MAXMSG);
//    if (nbytes < 0)
//    {
//        /* Read error. */
//        perror ("read");
//        exit (EXIT_FAILURE);
//    }
//    else if (nbytes == 0)
//        /* End-of-file. */
//        return -1;
//    else
//    {
//        /* Data read. */
//        fprintf (stderr, "Server: got message: `%s'\n", buffer);
//        return 0;
//    }

    cout << "EKANA CONNECT APO SERVER!\n";

    char buf[1];
    myString whitespace(" ");
    myString instruction("");

    bool flagLOG_ON     = false;
    bool flagGET_CLIENTS= false;
    bool flagLOG_OFF    = false;

    while(read(filedes, buf, 1) > 0) {  /* Receive 1 char */
        printf("diavasa %d bytes\n", ++diavasa);

        instruction += buf;

        cout << "To instruction exei timh = " << instruction << endl;

        if (instruction == "LOG_ON") {
            flagLOG_ON = true;
            cout << instruction;
            break;
        }

        if (instruction == "GET_CLIENTS") {
            flagGET_CLIENTS = true;
            cout << instruction;
            break;
        }

        if (instruction == "LOG_OFF") {
            flagLOG_OFF = true;
            cout << instruction;
            break;
        }


    }


    //todo SOOSSSS!!!TO LOG_ON KAI OLA TA ALLA MESSAGES THA TA STELNW SE ALLA SOCKETS
    //TODO SAYTA POU KATHE PROCESS KANIE LISTEN KAI DEXETAI CONNECTION
    //TODO ARA PREPEI NA KANW ESTABLISH KAINOURGIO CONNECTION OPOU SERVER GINETAI CLIENT
    //TODO KAI CLIENT-SERVER

    /* if (flagLOG_ON) {
         prot.recv_LOG_ON(newsock);
 //        prot.broadcast_USER_ON();

     }

 */

    return -1;

}