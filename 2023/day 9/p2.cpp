#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");
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
    int getPreviousForecastValue()
    {
        int prev = 0;
        for (int i = forecasts.size() - 1; i >= 0; i--)
        {
            // take forecasts in reverse order
            // and get their prev value
            vector<int> &currentForecast = forecasts.at(i);
            int firstForecastNumber = currentForecast.at(0);
            prev = firstForecastNumber - prev;
        }
        return prev;
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
        sum += ft.getPreviousForecastValue();
    }
    return sum;
}
int main()
{
    fout << getForecastBackwardsPredictionSum();
}