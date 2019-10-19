#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <fstream>

struct Statistics
{
    std::string word;
    long count;
};

bool CompareStatistics(const Statistics& lhs, const Statistics& rhs)
{
    return lhs.count >= rhs.count;
}

bool isNotLetter(const char& c)
{
    return !((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

std::string prepare(const std::string& s)
{
    std::string result;
    result.resize(s.size());
    std::transform(s.begin(), s.end(), result.begin(), ::tolower);

    // while loop does not work with big texts and throws segmentation fault
    while(isNotLetter(result[result.length() - 1]))
    {
        result.pop_back();
    }

    if(isNotLetter(result[0]))
    {
        result.erase(0, 1);
    }

    return result;
}

int main(int argc, char const *argv[])
{
    std::ifstream file("test.txt");

    if(file.is_open())
    {
        std::map<std::string, long> words;
        std::vector<Statistics> info;

        std::string word;
        while(!file.eof())
        {
            file >> word;
            word = prepare(word);
            words[word]++;
        }
        words[word]--;

        for(auto itWords: words)
        {
            Statistics temp = {itWords.first, itWords.second};
            info.push_back(temp);
        }

        std::sort(info.begin(), info.end(), CompareStatistics);

        for(auto itInfo: info)
        {
            std::cout << itInfo.word << " " << itInfo.count << std::endl;
        }
    }
    else return -1;

    return 0;
}
