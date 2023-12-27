#include <iostream>
#include <fstream>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");
bool isSpecificWord(int index, const char *string, const char *word)
{
    // verifies if string at specific index contains word
    // ex: 1 aBool Bool returns true
    int i = 0;
    while (word[i])
    {
        if (string[index + i] != word[i])
            return false;
        i++;
    }
    return true;
}
int digitWord(int index, const char *string)
{
    // if there is a word that is a digit at index then it returns it's number
    // otherwise returns -1
    // ex: 2 ABthree returns 3
    char zero[6] = "zero";
    char one[6] = "one";
    char two[6] = "two";
    char three[6] = "three";
    char four[6] = "four";
    char five[6] = "five";
    char six[6] = "six";
    char seven[6] = "seven";
    char eight[6] = "eight";
    char nine[6] = "nine";
    if (isSpecificWord(index, string, zero))
        return 0;
    if (isSpecificWord(index, string, one))
        return 1;
    if (isSpecificWord(index, string, two))
        return 2;
    if (isSpecificWord(index, string, three))
        return 3;
    if (isSpecificWord(index, string, four))
        return 4;
    if (isSpecificWord(index, string, five))
        return 5;
    if (isSpecificWord(index, string, six))
        return 6;
    if (isSpecificWord(index, string, seven))
        return 7;
    if (isSpecificWord(index, string, eight))
        return 8;
    if (isSpecificWord(index, string, nine))
        return 9;
    return -1;
}
int main()
{
    char row[200];
    int i, sum = 0;
    while (fin.getline(row, 200))
    {
        i = 0;
        int firstDigit, lastDigit;
        bool pickedFirstDigit = false;
        while (row[i] != '\0')
        {
            int digit = digitWord(i, row);
            if (digit == -1)
                if (((row[i] >= '0') && (row[i] <= '9')))
                    digit = row[i] - '0';
            if (digit != -1)
            {
                if (!pickedFirstDigit)
                {
                    firstDigit = digit;
                    pickedFirstDigit = true;
                }
                lastDigit = digit;
            }
            i++;
        }
        sum = sum + firstDigit * 10 + lastDigit;
    }
    fout << sum;
}