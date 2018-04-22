#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
//#include "classHeader.h"
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

#define windowX 1200
#define windowY 600
#define titleX 450
#define titleY 7
#define menuX 1020
#define menuY 150
#define ONOFFX 1020
#define ONOFFY 270
#define messboxX 220.0
#define messboxY 100.0
#define repboxX 220
#define repboxY 350
#define boxLeng 700
#define boxWidth 200
#define speakerX 1020
#define speakerY 390
#define bubbleX 40
#define bubbleY 150
#define botX 40
#define botY 400
#define taskbarX 1200
#define taskbarY 5
#define projectbyX 1020
#define projectbyY 200
#define nisheshX 1020
#define nisheshY 270
#define shreyX 1020
#define shreyY  320
#define rohitX 1020
#define rohitY  370
#define navinX 1020
#define navinY  420

using namespace std;
using namespace sf;

class CResponse;
class messageHandler;
class audioControl;
class homeScreen;


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
    void getInput(std::string str);
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
    std::string printResponse();

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

    friend class messageHandler;

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
    //friend void messageHandler::Enter(RenderWindow &window, Event &event , Bot &b);
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




string tempInput , tempOutput;
//for class prototype
//chatbox, messaging, text display related
class messageHandler
{
    RenderWindow &window;
    bool flag=false;

public:
    string textInput;
    string textToSend;
    string textToRecieve;
    messageHandler(RenderWindow &w):window(w){}
    bool isAreaClicked(int left,int top,int width,int height , RenderWindow &window);
    string fromAI(Bot &b);
    void toAI(std::string textToSend , Bot &b);
    void setMessagingBox(int setx, int sey, int mLength, int mBreadth);
    void setReplyingBox(int setx, int sey, int rLength, int rBreadth);

    void textDisplay(int x,int y, string str, Color color, int s=10)
    {
        Font font;
        font.loadFromFile("LHANDW.ttf");
        Text text;
        text.setFont(font);
        text.setColor(color);
        text.setCharacterSize(s);
        text.setPosition(x,y);
        text.setString(str);
        window.draw(text);
    }
    void Enter(RenderWindow &window, Event &event , Bot &b)
    {
        //messageHandler chat(window);
        static int commentLine=0;    //number of line in messagebox
        static int replyLine=0;      //number of line in reply box
        static string comment[5];   //array of words of messagebox
        static string reply[5];     //array of words of replybox

        if (Mouse::isButtonPressed(Mouse::Left))
                {

                    if(isAreaClicked(messboxX,messboxY,boxLeng,boxWidth,window))
                    {
                        //if necessary , codes to highlight
                        flag=true;
                    }
//                    else if(isAreaClicked(speakerX, speakerY, 128, 128, window))
//                    {
//                        k^=1;
//                        running=false;
//                    }
//                    else
//                    {
//                        running=false;
//                        //cout<<"false being pressed"<<endl;
//                    }
                }
                if(flag)
                {
                      if (event.type == Event::TextEntered)
                    {
                        if (event.text.unicode >= 32 && event.text.unicode < 125 && textInput.size()<50)
                       {
                            textInput.push_back((char)event.text.unicode);
                            textDisplay(messboxX+5,messboxY+5,textInput, Color::White,20);

                       }
                       if(event.text.unicode == 8 && textInput.size()>0)  //backspace
                       {
                            textInput.pop_back();
                            setMessagingBox(messboxX,messboxY, boxLeng,boxWidth);
                       }
                       if(event.text.unicode == 13 && textInput.size()>0)  //when enter pressed
                       {
                           //cout<<"here"<<endl;
                            textToSend=textInput;
                            cout << textToSend;
                            toAI(textToSend , b);                   //to AI
                            textToRecieve=fromAI(b);   //from Nishes(from AI)
                            cout << textToRecieve;
                            if(commentLine<5)
                            {
                                setMessagingBox(messboxX,messboxY, boxLeng,boxWidth);
                                comment[commentLine]=textInput;
                                for(int i=0, x=1; i<=commentLine; i++)
                                {
                                    textDisplay(messboxX+5,messboxY+5+x*20, comment[i] ,Color::White, 20);
                                    x++;
                                }
                                textInput="";
                                commentLine++;
                            }
                            else
                            {
                                commentLine=0;
                                comment[commentLine]=textInput;
                                setMessagingBox(messboxX,messboxY, boxLeng,boxWidth);      //clear all texts by black messagebox
                                textDisplay(messboxX+5,messboxY+5+(commentLine+1)*20, textInput ,Color::White, 20);  //since 1st line should be cleared
                                textInput="";
                            }

                            if(replyLine<5)
                            {
                                setReplyingBox(repboxX,repboxY, boxLeng, boxWidth);
                                reply[replyLine]=textToRecieve;
                                for(int i=0, x=1; i<=replyLine; i++)
                                {
                                    textDisplay(repboxX+5,repboxY+5+x*20, reply[i] ,Color::Black, 20);
                                    x++;
                                }
                                textToRecieve="";
                                replyLine++;
                            }
                            else
                            {
                                replyLine=0;
                                reply[replyLine]=textToRecieve;
                                setReplyingBox(repboxX,repboxY, boxLeng, boxWidth);
                                textDisplay(repboxX+5,repboxY+5+(replyLine+1)*20, textToRecieve ,Color::Black, 20);  //since 1st line should be cleared
                                textToRecieve="";
                            }
                       }
                        cout<<textInput<<endl;
                    }
                }
    }
    friend class Bot;
//   friend void Bot::printResponse(messageHandler &obj) const;
//    friend void Bot::respond(messageHandler &obj);
//    friend void Bot::getInput(std::string str , messageHandler &obj);
};


transposeType transposeList[] = {
                                 {" I'M " , " YOU'RE "},
                                 {" AM " , " ARE "},
                                 {" WERE " , " WAS "},
                                 {" I " , " YOU "},
                                 {" YOURS " , " MINE "},
                                 {" YOUR " , " MY "},
                                 {" I'VE " , " YOU'VE "},
                                 {" ME " , " YOU "},
                                 {" AREN'T " , " AM NOT "},
                                 {" WEREN'T " , " WASN'T "},
                                 {" I'D " , " YOU'D "},
                                 {" DAD " , " FATHER "},
                                 {" MOM " , " MOTHER "},
                                 {" MYSELF " , " YOURSELF "}
                                };

size_t transposeListSize = sizeof(transposeList) / sizeof(transposeList[0]);


//for bakcground images and text related classes
void Bot::saveUnknownInput()
{
    std::fstream outfile("unknown.txt" , std::ios::out);
    if(outfile.fail())
    {
        throw std::string("Unable to save Unknown input list.");
    }
    vstring::const_iterator iter = ListOfUnknownInput.begin();
    for( ; iter != ListOfUnknownInput.end() ; ++iter)
    {
        outfile << *iter << std::endl;
    }
    outfile.flush();
    outfile.close();
}

void Bot::saveLog()     //open the file for saving the conversation
{
    time_t ltime;
    time(&ltime);

    logfile.open("log.txt" , std::ios::out | std::ios::app);
    if(logfile.fail())
    {
        throw std::string("Log file cannot be saved.");
    }
    logfile << "\n\nConversation Log - " << ctime(&ltime) <<std::endl;
}

void Bot::saveLog(std::string str)  //function overloading of saveLog function
{
    if(str == "CHATBOT")
    {
        logfile << sResponse << std::endl;
    }
    else if(str == "USER")      //the user input is preceded by '>' sign as in the console
    {
        logfile << ">" << sInput << std::endl;
    }
}

void Bot::loadDatabase()
{
    std::fstream infile("Database.txt" , std::ios::in); //open the database in input mode
    if(infile.fail())
    {
        throw std::string("Unable to load Database.txt");
    }
    CResponse respObj;
    std::vector<CResponse> ListOfRespObj;

    std::string buffer;
    vstring keyList;

    while(std::getline(infile , buffer))    //reads a line from file and store it in buffer
    {
        char symbol = buffer[0];
        buffer.erase(0,1);          //removes K from keywords, R from responses and C from contexts
        switch(symbol)
        {
        case 'K':keyList.push_back(buffer);
                 break;
        case 'C':respObj.addContext(buffer);    //calls addContext function of object respObj
                 break;
        case 'R':respObj.addResp(buffer);
                 break;
        case '%':ListOfRespObj.push_back(respObj);
                 respObj.clear();
                 break;
        case '#':{
                    if(ListOfRespObj.empty())
                    {
                        ListOfRespObj.push_back(respObj);   //adds the respObj object to the vector ListOfRespObj
                    }

                    vstring::const_iterator iter = keyList.begin();
                    for(;iter != keyList.end() ; ++iter)
                    {
                        database[*iter] = ListOfRespObj;    //maps the keyList to the corresponding list of responses in the database
                    }
                    keyList.clear();
                    ListOfRespObj.clear();
                    respObj.clear();
                 }
                 break;
        }
    }
    infile.close();
}

void Bot::starting()        //Start conversation with the user
{
    handleEvent("STARTING**");
    saveLog();               //after starting, the logfile is opened in append mode
    saveLog("CHATBOT");      //the first starting conversation of the chatbot is saved
    selectResponse();
  //  printResponse();
}

void Bot::getInput(std::string str)         //takes input from the user
{
    //std::cout << ">";
    savePrevInput();        //saves previous Input
    sInput = str;
    saveLog("USER");    //user conversation is saved
    respond();          //then the bot respond is taken
    //return sResponse;
    //std::getline(std::cin,sInput);
    //sInput = chat.toAI();
   // preprocessInput();      //removes punctuation marks and unnecessary spaces
}

void Bot::preprocessInput()      //removes punctuation and unnecessary spaces and convert the string to upppercase if it is in lowercase
{
    cleanString(sInput);
    trimRight(sInput , ".");    //removes . from the end of the input
    UpperCase(sInput);
    //insertSpace(sInput);
}

void Bot::preProcessResponse()
{
    if(sResponse.find("*") != std::string::npos)
    {
        findSubject();      //find the value of sSubject.i.e. the actual content of the sInput which we want to keep in our response
        transpose(sSubject);    //transpose some elements in the sSubject
        replace(sResponse , "*" ," " + sSubject);    //replaces * in the response template by the value of sSubject
    }
}

void Bot::extractRespList(std::vector<CResponse> objList) //extracts response list from the ListOfRespObj which contains contexts as well
{
    std::vector<CResponse>::const_iterator iter = objList.begin();
    for(;iter != objList.end() ; ++iter)
    {
        vstring ListOfContext = iter->getContextList();
        if(ListOfContext.empty())       //if the response list is empty then, the response list is directly copied
        {
            ListOfResponse = iter->getRespList();
        }
        else if(std::find(ListOfContext.begin() , ListOfContext.end() , sPrevResponse) != ListOfContext.end()) //checks if the previous response is same as the previous context
        {                                           //if sPrevResponse is found in the context list only then, response list is copied
            ListOfResponse = iter->getRespList();
            break;
        }
    }
}

void Bot::findSubject()
{
    sSubject.erase();
    trimRight(sInput , " ");    //removes space characters at the end of the input given  by the user
    trimLR(sKeyWord , " ");
    size_t pos = sInput.find(sKeyWord);     //sKeyWord is the best keyword from the database.
    if(pos != std::string::npos)
    {
        sSubject = sInput.substr(pos + sKeyWord.length() + 1 , sInput.length() - 1);    //everything else except the sKeyword is saved to sSubject from the sInput
    }
}

bool Bot::botRepeat()
{
    int pos = findRespPos(sResponse);
    if(pos > 0)
    {
        return pos + 1 < ListOfResponse.size();
    }
    return 0;
}

int Bot::findRespPos(std::string str)
{
    int pos = -1;
    std::stack<std::string> s = ResponseLog;
    while(s.empty())
    {
        ++pos;
        if(s.top() == str)
        {
            break;
        }
        s.pop();
    }
    return pos;
}

std::string Bot::getSubPhrase(vstring wordList , size_t start , size_t endPos)
{
    std::string buffer;
    for(int i = start ; i < endPos ; ++i)
    {
        buffer += wordList[i];  //combines the strings from the  wordList into a sentence separated by space character.
        if(i != endPos - 1)
            buffer += " ";
    }
    return buffer;
}

void Bot::preProcessKeyWord(std::string &str , size_t startPos , size_t endPos , size_t iSize)  //if the keyword does not match then, we
{                                                                                               //put _ at the beginning and end and
    if(startPos == 0)                                                                           //starts matching process again
        str.insert(0 , "_");
    if(endPos == iSize - 1)
        str.insert(str.length() , "_");
}

void Bot::transpose(std::string &str)
{
    std::string buffer = " " + str + " ";   //space character at the beginning and the end are kept for the matching process of the
    bool bTransposed = 0;                   //words found in the beginning and the end because the transpose list contains spaces.
    for(int i = 0 ; i < transposeListSize ; ++i)
    {
        std::string left = transposeList[i].left;
        std::string right = transposeList[i].right;
        while(replace(buffer , left , right) != std::string::npos)  //replaces left with right in the buffer
        {
            bTransposed = 1;
        }
    }
    if(!bTransposed)        //if left is not exchanged with right then replace right with left
    {
        for(int i = 0 ; i < transposeListSize ; ++i)
        {
            std::string left = transposeList[i].left;
            std::string right = transposeList[i].right;
            while(replace(buffer , right , left) != std::string::npos);
        }
    }
    trimLR(buffer , " ");
    str = buffer;
}

void Bot::findMatch()       //finds the best response for the current input
{
    ListOfResponse.clear();
    std::string bestKeyWord;                     //string to store the best keyword
    vstring ListOfWord;

    if(sInput.find("**") == std::string::npos)
    {
        trimRight(sInput , ".");
        UpperCase(sInput);
        tokenize(sInput , ListOfWord);          //store the individual strings separated by spaces from the input into ListOfWord
        bestKeyWord = findBestKey(ListOfWord);
        sKeyWord = bestKeyWord;
    }
    else    //if the sInput is set to the value of an event having ** , it does not need to be tokenized and the sInput is directly copied to sKeyWord
    {
        sKeyWord = sInput;
    }
    if(database.find(sKeyWord) != database.end())
    {
        std::vector<CResponse> ListOfRespObj = database[sKeyWord];  //stores the corresponding responses in the ListOfRespObj
        extractRespList(ListOfRespObj);
    }
}

std::string Bot::findBestKey(vstring v)
{
    std::string buffer;
    int iSize = v.size();
    bool bKeyFound = 0;
    for(int j = iSize ; j >= 1 ; --j)
    {
        for(int k = 0 ; (k + j) <= iSize ; ++k)     //this algorithm divides each input into different combination of keywords.
        {                                           //The keyword with the longest length appears first and it is checked first
            buffer = getSubPhrase(v , k , k + j);   //in the database.The keyword with the longest length is the best keyword.
            if(database.find(buffer) != database.end())
            {
                bKeyFound = 1;
            }
            else
            {
                preProcessKeyWord(buffer , k , k + j , iSize);
                if(database.find(buffer) != database.end()) //checks after adding '_' at the beginning and at the end
                {
                    bKeyFound = 1;
                }
            }
            if(bKeyFound)
            {
                return buffer;
            }
        }
    }
    return buffer;
}


std::string Bot::printResponse()        //prints the response selected by the bot
{
//    if(sResponse.length() < 10)
//    {
//        char s[256];
//        strcpy(s , sResponse.c_str());
//        openApplication(s);
//        return;
//    }
    if(sResponse.length() > 0)
    {
        return sResponse;
        //obj.fromAI(sResponse);
    }        std::cout << sResponse << std::endl;

}

void Bot::respond()
{
    savePrevResponse();
    setEvent("BOT UNDERSTAND**");   //during bot understand event, only the event is set handleEvent function is not called
                                    //handleEvent function is called to set sInput to the value of the event.
    if(nullInput())     //checks if sInput is null
    {
        handleEvent("NULL INPUT**");
    }
    else if(nullInputRepitition())
    {
        handleEvent("NULL INPUT REPETITION**");
    }
    else if(userRepeat())
    {
        handleUserRepetition();
    }
    else
    {
        findMatch();
    }

    if(userWantToQuit())
    {
        bQuitProgram = 1;
    }

    if(!botUnderstand())
    {
        handleEvent("BOT DONT UNDERSTAND**");
        updateUnknownInputList();       //If the matched keyword is not found in the database then, that input is saved in a text file
    }                                   //unknown.txt so that we can add corresponding responses later.

    if(ListOfResponse.size() > 0)
    {
        selectResponse();
        saveBotResponse();
        preProcessResponse();

        if(botRepeat())
        {
            handleRepetition();
        }
        saveLog("CHATBOT");     //before printing the response, it is saved in the conversation log first
        printResponse();
    }
}

void Bot::handleRepetition()    //handles repetition made by the program
{
    std::map<int , std::string> ListOfPrevResponse;
    if(ListOfPrevResponse.size() > 1)
    {
        std::stack<std::string> s = ResponseLog;
        vstring::const_iterator iter  = ListOfResponse.begin();
        for(;iter != ListOfResponse.end() ; ++iter)
        {
            int pos = findRespPos(*iter);
            ListOfPrevResponse[pos] = *iter;
        }
    }
    std::map<int , std::string>::const_iterator iter2 = ListOfPrevResponse.end();
    --iter2;
    sResponse = iter2->second;
}

void Bot::handleUserRepetition()    //handles repetition made by the user
{
    if(sameInput())
    {
        handleEvent("REPETITION T1**");
    }
    else if(similarInput())
    {
        handleEvent("REPETITION T2**");
    }
}

void Bot::handleEvent(std::string str)  //handles different types of event
{
    savePrevEvent();
    setEvent(str);

    saveInput();
    //insertSpace(str);
    setInput(str);

    if(!sameEvent())
    {
        findMatch();
    }
    restoreInput();     //input is restored so that it can be saved in the prevInput variable to compare with the new input
}

class homeScreen
{
    RenderWindow &window;
public:
    homeScreen(RenderWindow &w):window(w){}
    void createTaskbar(int , int );
    void createText( int chSize,  int setx, int sety ,Color color , string strname ,string fontstyle="LHANDW.ttf");

};
void homeScreen::createTaskbar( int length,int breadth)
{
    RectangleShape bar(Vector2f(length, breadth));
    bar.setFillColor(Color(197,114,31));
    bar.setPosition(Vector2f(0,70));
    window.draw(bar);
}
void homeScreen::createText( int chSize,  int setx, int sety ,Color color , string strname ,string fontstyle)
{
    Font font;
    if(!font.loadFromFile(fontstyle))
    {
        cout<<"font could not be loaded";
    }
    Text title;
    title.setFont(font);
    title.setPosition(Vector2f(setx, sety));
    title.setCharacterSize(chSize);
    title.setColor(Color(23,67,122));
    title.setString(strname);
    title.setColor(color);
    window.draw(title);
}


class audioControl
{
    RenderWindow &window;
public:
    audioControl(RenderWindow &w): window(w){}
    void createSprite(int setx ,int sety, string imageName);
    void toggleSprite();
    //related to audio true false, then toggle image void
};
void audioControl::createSprite(int setx ,int sety, string imageName)
{
    Sprite loudSprite;
    Texture loudIcon;
    if(!loudIcon.loadFromFile(imageName))
    {
        cout<<"image could not be loaded";
    }
    loudSprite.setTexture(loudIcon);
    loudSprite.setPosition(setx,sety);
    window.draw(loudSprite);
}



void messageHandler::setMessagingBox(int setx, int sey, int mLength, int mBreadth)
{
    RectangleShape messagebox(Vector2f(mLength, mBreadth));
    messagebox.setPosition(Vector2f(setx,sey));
    messagebox.setFillColor(Color(110,110,110));
    messagebox.setOutlineColor(Color(220,220,220));
    messagebox.setOutlineThickness(10);
    window.draw(messagebox);
}
void messageHandler::setReplyingBox(int setx, int sey, int rLength, int rBreadth)
{
    RectangleShape relplybox(Vector2f(rLength, rBreadth));
    relplybox.setPosition(Vector2f(setx,sey));
    relplybox.setFillColor(Color(220,220,220));
    relplybox.setOutlineColor(Color(110,110,110));
    relplybox.setOutlineThickness(10);
    window.draw(relplybox);
}

void messageHandler::toAI(std::string str , Bot &obj)
{
    obj.getInput(str);
}

string messageHandler::fromAI(Bot &obj)
{
    return obj.printResponse();
}

bool messageHandler::isAreaClicked(int left,int top,int width,int height , RenderWindow &window)
{
    Vector2i mousePosition = Mouse::getPosition(window);
    if(mousePosition.x > left && mousePosition.x < (left + width)
        && mousePosition.y > top && mousePosition.y < (top + height))
        return true;
    else
        return false;
}



int main()
{
    //static bool isOutsideClicked=true;
    Bot bot("Chatterbot");

    RenderWindow window(VideoMode(windowX, windowY), "Chat Window");

    homeScreen backPage(window);
    messageHandler chat(window);
    audioControl speech(window);

    window.clear(Color::White);
    bool flag;
    bool toBeCleared=true;

    //backPage.createTaskbar(taskbarX, taskbarY);
    //title
    backPage.createText( 50,titleX,titleY, Color::Blue, "CHAT BOT");

    backPage.createText( 24,projectbyX,projectbyY,Color(255, 23, 111), "Project by:", "MATURASC.ttf");
    backPage.createText( 16,nisheshX,nisheshY,Color(196,23,111), "Nishesh Awale");
    backPage.createText( 16,shreyX,shreyY,Color(196,23,111), "Shrey Niraula");
    backPage.createText( 16,rohitX,rohitY,Color(196,23,111), "Rohit Kauri");
    backPage.createText( 16,navinX,navinY,Color(196,23,111), "Nabin Rai");

    //volume
    //backPage.createText( 24,740,420,Color(20,255,20),"Volume");

    //image
    //speech.createSprite(speakerX, speakerY, "sound.png");

    //chat area
    chat.setMessagingBox( messboxX, messboxY, boxLeng, boxWidth);
    chat.setReplyingBox( repboxX, repboxY, boxLeng, boxWidth);

    //logo
    speech.createSprite(bubbleX,bubbleY,"bubble.png");
    speech.createSprite(botX,botY,"bot.png");


    while (window.isOpen())
    {

        Event event;
            while (window.pollEvent(event) )
            {

                if(event.type==Event::Closed)
                    window.close();

                chat.Enter(window, event, bot);
                //bot.getInput();
            }
    window.display();
    toBeCleared=false;
    }
 return 0;
}

