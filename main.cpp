#include <tuple>
#include <utility>
#include <iostream>

enum MatchType
{
    Unknown = 0,
    Equal,
    NotEqual,
    Great,
    GreatEqual,
    Less,
    LessEqual,
    MAX,
};

MatchType func1()
{
    MatchType ret;
    return MatchType::LessEqual ;
}
int main()
{
    MatchType ret = func1();

    std::cout << ret << std::endl;
}