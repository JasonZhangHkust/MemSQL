#ifndef MEMSQL_QUERY_H
#define MEMSQL_QUERY_H

#include "TableBase.h"
#include "TableField.h"
#include "Table.h"
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

        void Reset()
        {
            done = false;
            iLimit = -1;
            vConditions.clear();
            mResults.clear();
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

            vConditions.push_back(tCon);
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

        Query& SortBy( std::function<bool(std::string&)> fCallBack)
        {
            fSorter = fCallBack;
            return  *this;
        }

        void Add(pRecord)
        {
            mMergeDataByData.insert(pRecord);
        }

        void Do()
        {
            if(done || !pTable.get())
                return;

            if(pTable->GetFieldCount() > 0)
            {
                if(vConditions.size().empty())
                {
                    pTable->GetFieldByIndex(0);
                }
                else
                {
                    for(auto& tCon: vConditions)
                    {
                        tCon.pTableField->Match(this, tCon.matchType, tCon.sValue);
                    }
                }
            }

            done = true;
        }

        std::vector<std::map<std::string, std::string> > Result()
        {
            Do();
            std::vector<std::map<std::string, std::string> > vResult;

            for(auto& data : vRet)
            {
                vResult.push_back(data->second);
            }

            if(fSorter)
            {
                sort(vResult.begin(), vResult.end(), fSorter);
            }

            if(iLimit != -1 && iLimit < vResult.size())
            {
                vResult.erase(vResult.begin(), vResult.begin() + iLimit);
            }

            return vResult;
        }
    };
};





#endif //MEMSQL_QUERY_H
