#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

bool compare(const std::string& lhs, const std::string& rhs)
{
    return lhs.length() == rhs.length() ? lhs < rhs : lhs.length() < rhs.length();
}

int main(int argc, char const *argv[])
{
    std::vector<std::string> numbers;
    int n;
    std::string temp;

    std::cout << "N = ";
    std::cin >> n;

    for(int i = 0; i < n; i++)
    {
        std::cin >> temp;
        numbers.push_back(temp);
    }

    std::sort(numbers.begin(), numbers.end(), compare);

    std::cout << "\nAfter sorting" << std::endl;

    for(auto it: numbers)
    {
        std::cout << it << std::endl;
    }

    return 0;
}
