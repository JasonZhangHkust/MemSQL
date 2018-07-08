#ifndef MEMSQL_TABLEBASE_H
#define MEMSQL_TABLEBASE_H

#include <list>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <map>
#include <set>

namespace MemSQL
{

    enum FieldType{ INT = 0, STRING = 1, LONG = 2,};
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

    class MatchTypeHelper
    {
        static std::unordered_map<std::string, MatchType> mStr2Type;
    public:
        static MatchType Parse(std::string& str)
        {
            if(mStr2Type.find(str) != mStr2Type.end())
            {
                return mStr2Type[str];
            }

            return MatchType::Unknown;
        }
    };

    typedef  std::shared_ptr<std::map<std::string, std::string> > pRecord;
}
;

#endif //MEMSQL_TABLEBASE_H
