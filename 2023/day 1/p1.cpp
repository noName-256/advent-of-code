#include <iostream>
#include <fstream>

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");
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
            if ((row[i] >= '0') && (row[i] <= '9'))
            {
                if (!pickedFirstDigit)
                {

                    firstDigit = row[i] - '0';
                    pickedFirstDigit = true;
                }
                lastDigit = row[i] - '0';
            }
            i++;
        }
        sum = sum + firstDigit * 10 + lastDigit;
    }
    fout << sum;
}