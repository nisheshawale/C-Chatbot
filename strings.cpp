#include "strings.h"

const std::string delim = "?!.;,*";

void UpperCase(std::string &str)
{
    int len = str.length();
    for(int i=0;i<len;i++)
    {
        if(str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] -= 'a' - 'A';
        }
    }
}

bool checkPunct(char c)     //check for punctuation marks
{
    return ispunct(c);
}

void cleanString(std::string &str)      //function to remove punctuation marks and unnecessary spaces
{
    int len = str.length();
    char prevChar = 0;
    std::string temp = "";
    for(int i = 0;i < str.length();i++)
    {
        if((str[i] == ' ' && prevChar != ' ') || !checkPunct(str[i]))
        {
            temp += str[i];             //stores the value of str[i] in the variable temp
            prevChar = str[i];
        }
        else if(prevChar != ' ' && checkPunct(str[i]))
        {
            temp += ' ';            //replaces punctuation with space if previous character is not a space
            prevChar = ' ';
        }
    }
    str = temp;             //stores the final string to str
}

void copy(char *array[] , vstring &v , size_t arraySize)       //copies the array of strings in a vector
{
    for(int i = 0 ; i < arraySize ; i++)
    {
        if(array[i] != NULL)                //to check whether the array is empty
            v.push_back(array[i]);
        else
            break;
    }
}

size_t replace(std::string &str , std::string substr1 , std::string substr2)    //replaces substr1 in the str with substr2
{
    size_t pos = str.find(substr1);
    if(pos != std::string::npos)
    {
        str.erase(pos , substr1.length());
        str.insert(pos , substr2);
    }
    return pos;
}

void trimRight(std::string &str , std::string delim)
{
    size_t pos = str.find_last_not_of(delim);       //find_last_not_of returns the position from the last where the delim characters are not found
    if(pos != std::string::npos)
    {
        str.erase(pos + 1 , str.length());      //erase the string from pos+1 upto the length of the characters specified by the second argument
    }
}

void trimLeft(std::string &str , std::string delim)
{
    size_t pos = str.find_first_not_of(delim);
    if(pos != std::string::npos)
    {
        str.erase(0 , pos);     //erase string from position 0 upto the length of character specified by pos
    }
}

void trimLR(std::string &str , std::string characters)
{
    trimLeft(str , characters);
    trimRight(str , characters);
}

void tokenize(const std::string str , vstring &v)
{
    std::string buffer;
    for(int i = 0 ; i < str.length() ; ++i)
    {
        if(!ispunct(str[i]) && !isspace(str[i]) && str[i] != '.')
        {
            buffer += str[i];
        }
        else if(!buffer.empty())
        {
            v.push_back(buffer);    //buffer takes the value from str[i] until punctuation, period or space character is encountered
            buffer.erase();         //Then, it is push back to v and the buffer is erased.
        }
    }
    if((v.empty() && !buffer.empty()) || !buffer.empty())   //the condition is true when v is empty and buffer is not empty. It is
    {                                                       //also true when only the buffer is not empty.
        v.push_back(buffer);
    }
}
