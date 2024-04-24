/*
    Numerical Literals:
        This program will read a list of literals from a
        .txt file and determine whether or not each input
        is a correct literal.

    Copyright 2017 Zachare' Lofton
*/

#include <iostream>
#include <string>
#include <fstream>
#define MAX_LENGTH 5000

using namespace std;

struct LITERAL {
    bool correctLength;
    bool correctLiteral;
    string line;
};

int checkSize(string);
bool checkForOptionalSign_wholeNum(string);
bool checkForOptionalSign_E(string);
bool checkStringFormat(string);
bool checkExponent(string);
bool checkDecimal(string);
bool checkSign(string);
string cutString(string, int, int);
void printToFile(ofstream&, LITERAL []);
void addLineTag(LITERAL[], int);

int main()
{
    ofstream newFile;
    ifstream expectedFile, currentFile;
//    expectedFile.open("PA_testfile_Expected_Output.txt"),
    currentFile.open("PA_testfile.txt");
    newFile.open("PA_testfile_output.txt");

    cout << "Welcome to the Literals Program. This program reads\n"
         << "data from the PA_testfile.txt and determines if each\n"
         << "line is a correct literal and prints it to a new file\n"
         << "'PA_testfile_output.txt' as well as the screen.\n\n";

    int i = 0;
    LITERAL data[MAX_LENGTH];

    while (true)
    {
        string temp;

        if ( currentFile.eof() )
            break;

        getline(currentFile, temp);
        int SIZE = checkSize(temp);

        if (SIZE > MAX_LENGTH)
        {
            data[i].correctLength = false;
            data[i].line = temp;
            temp.clear();
        }
        else
        {
            data[i].correctLength = true;
            data[i].line = temp;
            temp.clear();
        }

        i++;
    }

    for ( int iter = 0; iter < i; iter++)
    {
        bool correct;
        correct = ( checkStringFormat(data[iter].line) &&
                   checkForOptionalSign_wholeNum(data[iter].line) &&
                   checkForOptionalSign_E(data[iter].line));
        data[iter].correctLiteral = correct;
    }

    addLineTag(data, i);
    printToFile(newFile, data);

    for (int iter = 0; iter < i; iter++)
    {
        cout << data[iter].line << endl;
    }

    expectedFile.close();
    currentFile.close();
    newFile.close();

    return 0;
}

//=================================================================
//   Args:
//      - lit: lit is the string representation of a literal.
//
//   This function checks the size of a string.
//=================================================================

int checkSize(string lit)
{
    return lit.size();
}

//=================================================================
//  Args:
//      - lit: string representation of a literal
//
//  Checks lit for an optional sign ('+' or '-') in the first string
//  location.
//=================================================================

bool checkForOptionalSign_wholeNum(string lit)
{
    string limit = ".0123456789+-";
    bool correct = true;
    int iter = 0;

    while (iter < limit.size())
    {
        if (limit[iter] == lit[0])
            return correct;
        else
            iter++;
    }
    return !correct;
}

//=================================================================
// Args:
//      -lit: string representation of a literal.
//      -start: starting index you want to cut.
//      -stop: final index you want to stop at. In this program it
//        will always be the end of the string.
//
//  Cut's string so that you can look at one individual substring.
//=================================================================

string cutString(string lit, int start, int stop)
{
    string cutPortion;

    while (start < stop)
    {
        cutPortion = cutPortion + lit[start];
        start++;
    }

    return cutPortion;
}

//=================================================================
// Args:
//      -lit: string representation of a literal.
//
//  Checks lit for an optional sign ('+' or '-') after the e in the
//  literal.
//=================================================================

bool checkForOptionalSign_E(string lit)
{
    string temp, limit = "-+1234567890";
    int found = lit.find('e'), iter = 0;
    bool correct = true;

    if (found == string::npos)
    {
        found = lit.find('E');
        if (found == string::npos)
            return correct;
    }
    temp = cutString(lit, found, lit.size());

    while (iter < limit.size())
    {
        if (limit[iter] == temp[1])
            return correct;
        else
            iter++;
    }
    return !correct;
}

//=================================================================
//  Args:
//      -file: a reference to a file you will be changing.
//      -data: an array of literals copied from another text file.
//
//  Prints all the literals from data to a new text file.
//=================================================================

void printToFile(ofstream &file, LITERAL data[])
{
    for (int i = 0; i < MAX_LENGTH; i++)
    {
        file << data[i].line << endl;
    }
}

//=================================================================
//  Args:
//      -data: an array of literals
//      -counter: size of the elements placed in the array from the
//        original text file.
//
//  Adds the corresponding tag to each element in data.
//=================================================================

void addLineTag(LITERAL data[], int counter)
{
    string litTag, nonLitTag;
    int BIGSIZE = 0;

    litTag = " is a valid numeric literal";
    nonLitTag = " is not a valid numeric literal";

    for (int i = 0; i < counter; i++)
    {
        int TOTALSPACE = 0;

        if ( BIGSIZE < data[i].line.size() )
        {
            BIGSIZE = data[i].line.size();
            TOTALSPACE = BIGSIZE + 2;
        }
        else
        {
            TOTALSPACE = BIGSIZE - data[i].line.size() + 2;
        }

        char SPACE = ' ';

        if (data[i].correctLength == true && data[i].correctLiteral == true)
        {
            litTag = litTag.insert(0, TOTALSPACE, SPACE);
            data[i].line = data[i].line + litTag;
        }
        else
        {
            nonLitTag = nonLitTag.insert(0, TOTALSPACE, SPACE);
            data[i].line = data[i].line + nonLitTag;
        }

        litTag = " is a valid numeric literal";
        nonLitTag = " is not a valid numeric literal";
    }
}

//=================================================================
//  Args:
//      -lit: string representation of a literal.
//
//  Checks to make sure the format of the string is correct.
//=================================================================

bool checkStringFormat(string lit)
{
    bool correct, correctE, correctD, correctS;
    int startPosE, startPosD, StarPosS;
    startPosD = startPosE = StarPosS = 0;
    correct = correctD = correctE = correctS = true;

    correctD = checkDecimal(lit);
    correctE = checkExponent(lit);
    correctS = checkSign(lit);
    correct = correctD && correctE && correctS;

    return correct;
}

//=================================================================
// Args:
//      -lit: string representation of a literal.
//
//  Checks string to make sure at most 1 e/E exists in the literal.
//=================================================================

bool checkExponent(string lit)
{
    int startPosE = 0;
    bool correctE = true;

    startPosE = lit.find('e', startPosE);

    if ( startPosE == string::npos )
    {
        startPosE = lit.find('E', startPosE);
        if ( startPosE == 0 )
            return !correctE;
    }
    else if ( startPosE == 0 )
        return !correctE;
    else
        startPosE = lit.find('e', startPosE);
        if ( startPosE == string::npos )
            return correctE;
}

//=================================================================
//  Args:
//      -lir: string representation of a literal.
//
//  Checks to make sure at most 1 decimal is present in the string.
//=================================================================

bool checkDecimal(string lit)
{
    bool correct = true;
    int decimal = 0;

    for (int i = 0; i < lit.size(); i++)
    {
        if ( lit[i] == '.' )
            decimal++;
    }

    return decimal >= 2 ? !correct : correct;
}

//=================================================================
//  Args:
//      -lit: string representation of the literal.
//
//  Checks to make sure the correct amount of signs are in each
//  literal, as well as being placed in allowed places.
//=================================================================

bool checkSign(string lit)
{
    bool correct = true;
    int findSign, sign, e, d;
    char SIGNS;

    findSign = sign = e = d = 0;

    for ( int i = 0; i < lit.size(); i++ )
    {
        SIGNS = lit[i];

        switch(SIGNS)
        {
        case 'e':
        case 'E':
            {
                e++;
                if ( e == 2 )
                    return !correct;
                break;
            }
        case '+':
        case ' ':
        case '-':
            {
                if ( i != 0 )
                    return !correct;
                sign++;
                if ( sign == 2  || d > 1 )
                    return !correct;
                break;
            }
        case '.':
            {
                d++;
                if ( d == 2 || e > 0 )
                    return !correct;
                break;
            }
        default:
            break;
        }
    }

    return correct;
}

