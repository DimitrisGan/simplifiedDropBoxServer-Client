//
// Created by dimitrisgan on 26/2/2019.
//

#include <cstring>
#include <stdlib.h>
#include "myString.h"
#include "assistantFunctions.h"


int myString::to_int(){
    int number;
    if (isNumber(this->myStr)){ //check if given id is number
        return atoi(this->myStr);
    }
    cout << "myString::to_int() = this->myStr = "<< this->myStr<<endl;
    std::cerr << "INVALID USE OF to_int() FUNCTION  " <<endl;
    exit(10);
}


bool myString::substrExist( const myString &substr) const {

    return strstr(this->myStr, substr.myStr) != nullptr; //if strstr() retunrs null means that the substr is not contained in string
}                                                        //Thus return false otherwise return true




char& myString::operator[] (unsigned j)
{
    if (j >= this->len) {
        std::cerr << "INVALID INDEX IN MYSTRING IN [] OPERATOR " <<endl;
        exit(INVALID_INDEX_IN_MYSTRING);
    }
    return this->myStr[j];
}


myString myString::operator+(myString& s) {

    return *this += s ;
}

myString myString::operator+(const myString &s) {
    return *this += s ;
}



myString myString::operator+(const char* s) {
    return *this += s ;
}



myString &myString::operator+=(const char *s) {
    myString rhs(s);
    return *this += rhs;
}


myString& myString::operator+= (const myString& s)
{
    unsigned newLength = this->len + s.size();
    char*    newStr = new char[newLength+1];

    strcpy(newStr, this->myStr);
    strcat(newStr, s.getMyStr());

//        for (unsigned j=0; j < this->len; j++)
//            newStr[j] = myStr[j];
//
//        for (unsigned i=0; i < s.size(); i++)
//            newStr[this->len+i] = s[i];

    delete [] this->myStr;
    this->len = newLength;
    this->myStr= newStr;
    return *this;
}

//**************************************************
// Constructor to initialize the str member        *
// with a string constant.                         *
//**************************************************

myString::myString(const char *sptr)  {

    this->len = static_cast<int>(strlen(sptr));
    this->myStr= new char[len + 1];
    strcpy(this->myStr, sptr);
}



myString::myString (char c)
{
    this->len = 1;
    this->myStr   = new char(c);
}


myString::myString(unsigned num) {
    unsigned totalDigits=0;unsigned num2 = num;
    while(num2!=0){
        //4
        num2 = num2/10;
        totalDigits ++;
    }
    this->len = totalDigits;
    this->myStr   = new char[len+1];


    // convert num to string [buf]
    sprintf( myStr, "%d", num );

}




//**************************************************
// Destructor to delete/free the allocate memory   *
//**************************************************

myString::~myString() {

    if (len != 0)
        delete [] this->myStr;
    this->myStr= nullptr;
//    len=0;

}

//***************************************************************
// Overloaded == , != operators.                                *
// Called when the operand on the right is a MyString           *
// object.                                                      *
// == :Returns true if right.str is the same as str.            *
// != :Returns true if not right.str is the different from str. *
//***************************************************************

bool myString::operator==(const myString &rhs) const {
    return strcmp(myStr, rhs.getMyStr()) == 0;
}

bool myString::operator!=(const myString &rhs) const {
    return strcmp(myStr, rhs.getMyStr()) != 0;
}

//***************************************************************
// Overloaded == , != operators.                                *
// Called when the operand on the right is a string.            *
// == :Returns true if right.str is the same as str.            *
// != :Returns true if not right.str is the different from str. *
//***************************************************************


bool myString::operator==(const char *rhs)
{
    return !strcmp(myStr, rhs);
}

bool myString::operator!=(const char *rhs)
{

    return strcmp(myStr, rhs) != 0;
}


//*************************************************
// Copy constructor.                              *
//*************************************************

myString::myString(myString &right)
{
//    if (len != 0)
//        delete [] myStr;
    myStr = new char[right.size() + 1];
    strcpy(myStr, right.getMyStr());
    len = right.size();

}



//************************************************
// Overloaded = operator. Called when operand    *
// on the right is another MyString object.      *
// Returns the calling object.                   *
//************************************************

//myString& myString::operator=(myString right)
//{
////    if (this->size() != 0)
////        delete [] myStr;
//    myStr = new char[right.size() + 1];
//    strcpy(myStr, right.getMyStr());
//    len  = right.size();
//    return *this;
//}


myString &myString::operator=(const myString &right)  {

    if (len != 0)
        delete [] myStr;
    this->myStr = new char[right.size() + 1];
    strcpy(myStr, right.getMyStr());
    len  = right.size();
    return *this;
}






//***********************************************
// Overloaded = operator. Called when operand   *
// on the right is a string.                    *
// Returns the calling object.                  *
//***********************************************

myString& myString::operator=(const char *right)
{
    if (len != 0)
        delete [] myStr;
    int lenRight = static_cast<int>(strlen(right));
    myStr = new char[lenRight + 1];
    strcpy(myStr, right);
    len = static_cast<unsigned int>(strlen(right));
    return *this;
}


//===============================================================



char *myString::getMyStr() const {
    return myStr;
}

unsigned myString::size() const {
    return  len;
}

void myString::setMyStr(const char *myStr) {

    if (myStr != nullptr) {
        this->myStr = new char[len + 1];
        strcpy(this->myStr, myStr);
    }
    else{
        this->myStr = nullptr;
    }
    this->len = strlen(myStr);

}

//    ostream & operator << (ostream &out, myString &c)
//    {
//        for (int i = 0; i < c.size(); ++i) {
//
//        }
//        out << c.getMyStr();
//        return out;
//    }

ostream& operator<<(ostream &os, const myString &string) {
    if (string.myStr) {
        os << string.myStr;
    }
    return os;
}


std::istream& operator>> (std::istream& is, myString& s)
{
    char* c = new char[100];
    is >> c;
    s.setMyStr(c);
    delete[] c;

    return is;
}
