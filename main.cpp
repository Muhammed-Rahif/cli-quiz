#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <random>
using json = nlohmann::json;
using namespace std;

int roll(int min, int max)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(min, max);

    auto random_integer = uni(rng);
    return random_integer;
}

json getQuestionDataFromResponse(string resString)
{
    json parsedResponse;
    parsedResponse = json::parse(resString);

    json results;
    results = parsedResponse.value("results", json::array());

    json questionData;

    for (json::iterator it = results.begin(); it != results.end(); ++it)
    {
        questionData = *it;
    }

    return questionData;
}

string getSymbolForNum(int num)
{
    if (num == 0)
        return "①";
    else if (num == 1)
        return "②";
    else if (num == 2)
        return "③";
    else
        return "④";
}

void askAQuestion()
{
    // sending api req (category=17)
    cpr::Response response = cpr::Get(cpr::Url{"https://opentdb.com/api.php?amount=1&difficulty=easy&type=multiple"});

    // printing if api req fails or not
    if (response.status_code == 200)
        cout << "\033[1;32m[✔] - Question successfully loaded!\033[0m" << endl;
    else
        cout << "\033[1;31m[✘] - Response failed with status code\033[0m" << response.status_code << endl;

    // inserting a horizontal line
    cout << "\033[1;34m"
         << "-------------------"
         << "\033[0m\n";

    // Parsing the question data from response json
    json questionData = getQuestionDataFromResponse(response.text);

    // Ask the question
    cout << "\033[1;34m[?] - " << questionData.value("question", "Ooops!") << " (" << questionData.value("category", "All") << ")"
         << "\033[0m\n";
    cout << "\033[1;34m"
         << "-------------------"
         << "\033[0m\n";

    int crctAnsIndx = roll(0, 3);

    string correctAns = questionData.value("correct_answer", "Correct answer!");
    json incorrectAnswers = questionData.value("incorrect_answers", json::array());

    int i = 0;
    for (json::iterator incrctAns = incorrectAnswers.begin(); incrctAns != incorrectAnswers.end(); ++incrctAns)
    {
        string ans;

        if (i == crctAnsIndx)
        {
            cout << "\033[1;35m[" + getSymbolForNum(i) + " ] - \"" << correctAns << "\"\033[0m\n";
            i++;
        }

        cout << "\033[1;35m[" + getSymbolForNum(i) + " ] - " << *incrctAns << "\033[0m\n";
        if (crctAnsIndx == 3 && i == 2)
            cout << "\033[1;35m[" + getSymbolForNum(i + 1) + " ] - \"" << correctAns << "\"\033[0m\n";
        i++;
    }

    cout << "\033[1;34m"
         << "-------------------"
         << "\033[0m\n";
    cout << "\033[1;36m[Your answer is (1, 2, 3 or 4):\033[0m\n";

    int userAnsIndx;
    cin >> userAnsIndx;
    userAnsIndx--;

    if (userAnsIndx == crctAnsIndx)
        cout << "\033[1;32m[✔] - Corrct answer (" + correctAns + ")!\033[0m" << endl;
    else
        cout << "\033[1;31m[✘] - Incorrect answer! Correct answer is \'" + correctAns + "\'\033[0m" << endl;
    cout << "\033[1;34m"
         << "-------------------"
         << "\033[0m\n";
}

int main(int argc, char **argv)
{
    while (true)
    {
        askAQuestion();
        cout << "\n\n\n\n\n";
    }

    return 0;
}