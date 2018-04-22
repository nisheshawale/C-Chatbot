#ifndef __upperAndClean_H__
#define __upperAndClean_H__ // __upperAndClean_H__
#include "strings.h"

void upperCase(std::string &str)
{
    int len = str.length();
    for(int i=0;i<len;i++)
    {
        if(str[i] <= 'z' && str[i] >= 'a')
        {
            str[i] -= 'z' - 'a';
        }
    }
}

bool checkPunct(char c)     //check for punctuation marks
{
    return ispunct(c);
}

void filterString(std::string str)      //function to remove punctuation marks and unnecessary spaces
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
            temp += ' ';            //replaces punctuation with space
        }
    }
    str = temp;             //stores the final string to str
}

void copy(char *array[] , vstring &v)       //copies the array of strings in a vector
{
    for(int i = 0 ;i < maxResponse;i++)
    {
        if(array[i] != NULL)
            v.push_back(array[i]);
        else
            break;
    }
}

#endif
