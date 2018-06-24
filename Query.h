#ifndef MEMSQL_QUERY_H
#define MEMSQL_QUERY_H

#include "TableBase.h"
#include "TableField.h"
#include "Table.h"
#include <functional>

namespace MemSQL
{
    struct mergeData
    {
        int iCount; //find the duplicated times in the queried field;
        std::string sData;
    };

    struct Condition
    {
        std::shared_ptr<TableField> pTableField;
        MatchType matchType;
        std::string sValue;
    };

    class Query
    {
        std::unordered_map<std::string, mergeData> mMergeDataByData;
        std::vector<Condition> vConditions;
        std::vector<std::map<std::string, std::string> > vResults;
        int iLimit;
        std::shared_ptr<Table> pTable;
        std::function<bool(std::string&)> fCallback;
        bool done;

    public:
    Query(std::shared_ptr<Table> ptr) : pTable(ptr), iLimit(-1)
    {}

    void Reset()
    {
        done = false;
        iLimit = -1;
        vConditions.clear();
        vResults.clear();
    }

    Query& Where(std::string& sFieldName, std::string& sMatchType, std::string& sValue)
    {
        MatchType eMatchType = MatchTypeHelper::Parse(sMatchType);

        if(eMatchType == MatchType::Unknown)
        {
            throw "Unknow match type";
        }

        if(!pTable.get())
        {
            throw "Empty Table";
        }
        Condition tCon;
        tCon.matchType   = eMatchType;
        tCon.pTableField = (*pTable).GetFieldByName(sFieldName);
        tCon.sValue      = sValue;

        vConditions.push_back(std::move(tCon));
        return *this;
    }

    Query& Limit(int count)
    {
        if (count < 0)
        {
            throw "Count should > 0";
        }
        this->iLimit = count;

        return *this;
    }

    Query& SortBy()
    };



}





#endif //MEMSQL_QUERY_H
