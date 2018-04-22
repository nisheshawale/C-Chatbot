#include "classHeader.h"

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
        /*case '%':ListOfRespObj.push_back(respObj);
                 respObj.clear();
                 break;*/
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
    printResponse();
}

void Bot::getInput()         //takes input from the user
{
    std::cout << ">";
    savePrevInput();        //saves previous Input
    std::getline(std::cin,sInput);

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
        /*char ans;
        std::string newKeyword , newResponse , newResponse1 , newResponse2 , newResponse3;

        std::cout << "Do you want me to learn new keyword?(y/n)" << std::endl;
        std::cin >> ans;
        while(ans == 'y')
        {
            std::cout << "So the keyword is:" << std::endl;
            std::cin >> newKeyword;
            std::fstream outfile("Database.txt" , std::ios::out | std::ios::app); //open the database in input mode
            if(outfile.fail())
            {
                throw std::string("Unable to store keyword in the database.");
            }
            outfile << "#\n";
            newKeyword = 'K' + newKeyword;
            UpperCase(newKeyword);
            outfile << newKeyword << '\n';
            std::cout << "Write 3 responses separated with newline character." << std::endl;
            getline(getline(getline(getline(std::cin , newResponse) , newResponse2) , newResponse3) , newResponse1);
            //getline(std::cin , newResponse2);
            //getline(std::cin , newResponse3);
            newResponse1 = 'R' + newResponse1;
            newResponse2 = 'R' + newResponse2;
            newResponse3 = 'R' + newResponse3;
            outfile << newResponse1 << '\n';
            outfile << newResponse2 << '\n';
            outfile << newResponse3 << '\n';
            outfile.close();
            std::cout << "Keyword and responses learned successfully." << std::endl;
            std::cout << "Is there any other keyword that I should learn?(y/n)" << std::endl;
            std::cin >> ans;
        }*/
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

int main()
{
    std::cout << "Chatterbot with classes in C++\n" << std::endl;

    try
    {
        Bot bot("Chatterbot");

        bot.starting();
        while(!bot.quit())
        {
            bot.getInput();
            bot.saveLog("USER");    //user conversation is saved
            bot.respond();          //then the bot respond is taken
        }
        bot.saveUnknownInput();     //save any unknown input given by the user
    }
    catch(std::string str)
    {
        std::cerr << str << std::endl;
    }
    catch(...)
    {
        std::cerr << "The program has stopped due to some unknown exception." << std::endl;
    }
    return 0;
}
