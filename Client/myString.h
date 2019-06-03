//
// Created by dimitrisgan on 26/2/2019.
//

#ifndef MYSTRING_H
#define MYSTRING_H


#include <iostream>
#include "ErrorsCodes.h"
#include <string.h>

using namespace std;


class myString{
private:
    char* myStr;
    unsigned len;
public:
    myString(){myStr= nullptr;len =0;}

    explicit myString(const char *sptr);
    myString(unsigned num);
    myString (char c);
    myString( myString &right); //move operator maybe not needed because of not existing simple constructor
//    myString( myString right); //move operator maybe not needed because of not existing simple constructor

    int to_int();

    virtual ~myString();

    char *getMyStr() const;

    void setMyStr(const char *myStr);

    unsigned size() const;

    bool substrExist( const myString &substr) const ;

    void addExtension( char* ext){
        *this += ext;
    }


    void addExtension( myString ext){
        *this += ext;
    }



    void removeSubstr(const myString &sub) {
        size_t len = sub.size();
        if (len > 0) {
            char *p = this->myStr;
            if ((p = strstr(p, sub.getMyStr())) != nullptr) {
                memmove(p, p + len, strlen(p + len) + 1);
            }
        }
        this->len = static_cast<unsigned int>(strlen(this->myStr)); //update len

    }




    void removeSubstr(const char *sub) {
        size_t len = strlen(sub);
        if (len > 0) {
            char *p = this->myStr;
            if ((p = strstr(p, sub)) != nullptr) {
                memmove(p, p + len, strlen(p + len) + 1);
            }
        }
        this->len = static_cast<unsigned int>(strlen(this->myStr)); //update len
    }




//    bool substrExist( myString substr){
//
//        int i, j=0, k;
//        for(i=0; i<this->len; i++)
//        {
//            if(this->myStr[i] == substr.getMyStr()[j])
//            {
//                for(k=i, j=0; this->len && substr.size(); j++, k++)
//                    if(this->myStr[k]!=substr.getMyStr()[j])
//                        break;
//                if(j== substr.size()){
//                    printf("Substring");
//                    return true;}
//            }
//        }
//        printf("Not a substring");
//        return false;
//    }

    /*operators overloaded*/
    bool operator==(const myString &rhs) const; //compared with myString
    bool operator==(const char *rhs);           //compared with char *
    bool operator!=(const myString &rhs) const;
    bool operator!=(const char *rhs);           //compared with char *


//    myString& operator=(myString right);   // for assign with myString
    myString& operator=(const myString &right) ;   // for assign with myString
    myString& operator=(const char *right); // for assign with char*

    friend ostream & operator << (ostream &out, const myString &c);
    friend std::istream& operator>> (std::istream& is, myString& s);


    char operator[] (unsigned j);


    myString& operator+= (const myString& s);
    myString& operator+= (const char* s);

    myString operator+(myString& s);
    myString operator+(const myString& s);
    myString operator+(const char* s);


    };


#endif //MYSTRING_H
