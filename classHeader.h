//This part provides an interface for the chatbot
#ifndef chatterbot
#define chatterbot

#include "strings.h"
//#include "upperAndClean.h"
#include <iostream>
#include <ctime>        //for time(NULL) function
#include <conio.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stack>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>

class CResponse;

typedef std::map<std::string , std::vector<CResponse> > mapString;  //mapString maps a string to a vector of CResponse in the same
                                                                    //as array maps an index number to an object.
typedef struct
{
    char *left;
    char *right;
}transposeType;

class Bot
{
public:
    Bot(std::string str):sBotName(str),bQuitProgram(0),sInput("null")   //constructor to initialize the botName
    {                                                                   //sBotName represents BotName is of type string
        seedRandomNumberGenerator();
        loadDatabase();                                 //bQuitProgram represents boolean
    }
    ~Bot(){};

    void starting();
    void getInput();
    void respond();

    bool quit() const
    {
        return bQuitProgram;
    }

    void findMatch();
    void handleRepetition();            //to check if the bot is repeating itself
    void handleUserRepetition();          //to check if the user is repeating the questions
    void handleEvent(std::string str);
    void seedRandomNumberGenerator()
    {
        srand((unsigned)time(NULL));    //time(NULL) is a function which checks how many seconds have elapsed from 1970,January 1
    }                                   //srand initializes the random number generator with the value provided with parentheses
    void openApplication(char s1[256]) const
    {
        char s2[256];
        sprintf(s2,"START %s",s1);
        const char* prgm=s2;
        system(prgm);
    }
    void selectResponse()               //to select a response from a list of responses
    {
        shuffle(ListOfResponse , ListOfResponse.size());
        sResponse = ListOfResponse[0];
    }
    void savePrevInput()                //saves the current input into a variable before getting some new inputs
    {
        sPrevInput = sInput;
    }
    void savePrevResponse()             //saves the current response of the bot into a variable before the bot started to search another response
    {
        sPrevResponse = sResponse;
    }
    void savePrevEvent()                //saves the current event
    {
        sPrevEvent = sEvent;
    }
    void setEvent(std::string str)
    {
        sEvent = str;
    }
    void saveInput()                    //make a backup of the current input
    {
        sInputBackup = sInput;
    }
    void setInput(std::string str)      //set the current input
    {
        sInput = str;
    }
    void restoreInput()                 //restore the value of the current input stored in the backup
    {
        sInput = sInputBackup;
    }
    void printResponse() const          //prints the response selected by the bot
    {
        if(sResponse.length() < 10)
        {
            char s[256];
            strcpy(s , sResponse.c_str());
            openApplication(s);
            return;
        }
        if(sResponse.length() > 0)
        {
            std::cout << sResponse << std::endl;
        }
    }
    void preprocessInput();

    void preProcessResponse();

    void findSubject();

    bool botRepeat();              //checks if the bot is repeating

    bool userRepeat() const             //checks if the user is repeating
    {
        return (sPrevInput.length()>0 &&
                ((sInput == sPrevInput) ||
                 (sInput.find(sPrevInput) != std::string::npos)||      //search sPrevInput in the string sInput and returns a size_type
                 (sPrevInput.find(sInput) != std::string::npos)));     //which is the index where it is found and return npos if it is
                                                                        //not found. npos has value of -1.
    }
    bool botUnderstand() const          //checks if the bot understand the user input
    {
        return ListOfResponse.size() > 0;
    }
    bool nullInput() const              //checks if the current input is null
    {
        return (sInput.length() == 0 && sPrevInput.length() != 0);
    }
    bool nullInputRepitition() const    //checks if the user is repeating a null input
    {
        return (sInput.length() == 0 && sPrevInput.length() == 0);
    }
    bool userWantToQuit() const         //checks if the user wants to quit
    {
        return sInput.find("BYE") != std::string::npos;
    }
    bool sameEvent() const              //checks if the current event is same as the previous one
    {
        return (sEvent.length()>0 && sEvent == sPrevEvent);
    }
    bool noResponse() const             //checks if the program has no response for the current input
    {
        return ListOfResponse.size() == 0;
    }
    bool sameInput() const              //checks if the current input is same as the previous one
    {
        return (sInput.length() > 0 && sInput == sPrevInput);
    }
    std::string getSubPhrase(vstring wordList , size_t start , size_t end);
    std::string findBestKey(vstring v);

    void preProcessKeyWord(std::string &str , size_t startPos , size_t endPos , size_t iSize);

    bool similarInput() const           //checks if the current input is similar to the previous one
    {                                   //substring is also considered as a similar input
        return (sInput.length() > 0 &&
                (sInput.find(sPrevInput) != std::string::npos ||
                sPrevInput.find(sInput) != std::string::npos));
    }

    void transpose(std::string &str);

    //void addXmlTag(std::string &str);

    void loadDatabase();
    void extractRespList(std::vector<CResponse> objList);
    void saveBotResponse()
    {
        if(!sResponse.empty())
            ResponseLog.push(sResponse);
    }
    int findRespPos(std::string str);

    void saveUnknownInput();

    void updateUnknownInputList()
    {
        ListOfUnknownInput.push_back(sInput);
    }

    void saveLog();
    void saveLog(std::string str);

private:
    std::string sBotName;
    std::string sUserName;
    std::string sInput;
    std::string sResponse;
    std::string sPrevInput;
    std::string sPrevResponse;
    std::string sEvent;
    std::string sPrevEvent;
    std::string sInputBackup;
    std::string sSubject;
    std::string sKeyWord;
    std::string sContext;
    std::string sPrevContext;
    bool bQuitProgram;

    vstring ListOfResponse;   //vstring represents a vector of string
    vstring ListOfUnknownInput;
    std::stack<std::string> ResponseLog;

    mapString database;     //database object maps a string to a vector of CResponse

    std::fstream logfile;
};

class CResponse
{
public:
    CResponse(){}
    ~CResponse(){}

    void addContext(std::string str)
    {
        ListOfContext.push_back(str);
    }

    void addResp(std::string str)
    {
        ListOfResp.push_back(str);
    }

    vstring getContextList() const
    {
        return ListOfContext;
    }

    vstring getRespList() const
    {
        return ListOfResp;
    }

    void clear()
    {
        ListOfResp.clear();
        ListOfContext.clear();
    }

private:
    vstring ListOfContext;
    vstring ListOfResp;
};

#endif
