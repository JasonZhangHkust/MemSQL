#ifndef MEMSQL_QUERY_H
#define MEMSQL_QUERY_H

#include "TableBase.h"

#include <functional>
#include <utility>
#include <algorithm>

namespace MemSQL
{
    struct Condition
    {
        std::shared_ptr<TableField> pTableField;
        MatchType matchType;
        std::string sValue;
    };

    class Query
    {
    private:

        std::set<pRecord, std::owner_less<pRecord> > mMergeDataByData;
        std::vector<Condition> vConditions;
        std::vector<std::map<std::string, std::string> > vRet;
        int iLimit;
        std::shared_ptr<Table> pTable;
        std::function<bool(std::string&)> fSorter;
        bool done;

    public:

        Query(std::shared_ptr<Table> ptr) : pTable(std::move(ptr)), iLimit(-1)
        {}
		void Reset();
		Query& Where(std::string& sFieldName, std::string& sMatchType, std::string& sValue);
		Query& Limit(int count);
		Query& SortBy(std::function<bool(std::string&)> fCallBack);
		void Add(pRecord);
		void Do();
		std::vector<std::map<std::string, std::string> > Result();
    };
};





#endif //MEMSQL_QUERY_H
