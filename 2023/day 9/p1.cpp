#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");
struct ForecastTriangle
{
    vector<vector<int>> forecasts;
    bool allForecastValuesNull(const int &forecastIndex)
    {
        for (int &forecastNumber : forecasts[forecastIndex])
            if (forecastNumber != 0)
                return false;
        return true;
    }
    void createNewForecast(const int newForecastIndex)
    {
        // we will create this forecast based on the last forecast's differences
        vector<int> newForecast;
        vector<int> &lastForecast = forecasts[newForecastIndex - 1];
        int i, n, newForecastSize = lastForecast.size() - 1;
        for (i = 0; i < newForecastSize; i++)
        {
            n = lastForecast[i + 1] - lastForecast[i];
            newForecast.push_back(n);
        }
        forecasts.push_back(newForecast);
    }
    int getNextForecastValue()
    {
        int next = 0;
        for (int i = forecasts.size() - 1; i >= 0; i--)
        {
            // take forecasts in reverse order
            // and get their next value
            vector<int> &currentForecast = forecasts.at(i);
            int lastForecastNumber = currentForecast.at(currentForecast.size() - 1);
            next = lastForecastNumber + next;
        }
        return next;
    }
    ForecastTriangle(const vector<int> &initialForecast)
    {
        forecasts.push_back(initialForecast);
        int i = 0;
        while (!allForecastValuesNull(i))
        {
            i++;
            createNewForecast(i);
        }
    }
};
bool getForecast(vector<int> &forecast)
{
    forecast.clear();
    string inputRow;
    if (!getline(fin, inputRow))
        return false;
    stringstream forecastStream(inputRow);
    int n;
    while (forecastStream >> n)
        forecast.push_back(n);
    return true;
}
int getForecastBackwardsPredictionSum()
{
    int sum = 0;
    vector<int> forecast;
    while (getForecast(forecast))
    {
        ForecastTriangle ft(forecast);
        sum += ft.getNextForecastValue();
    }
    return sum;
}
int main()
{
    fout << getForecastBackwardsPredictionSum();
}