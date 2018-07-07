#ifndef MEMSQL_TABLE_H
#define MEMSQL_TABLE_H

#include "TableBase.h"
#include "TableField.h"
namespace MemSQL
{
    class Table
    {
    private:
        typedef std::shared_ptr<TableField> tableFieldPtr;
        std::unordered_map<std::string, pTableField> mFieldByName;
        std::vector<pTableField> vFields;
    public:
        Table(std::map<std::string, FieldType>& tTableDefine)
        {
           for(auto& tColumn : tTableDefine)
           {
               tableFieldPtr pTableField  = std::make_shared<TableField>(tColumn.first, tColumn.second);
               vFields.push_back(pTableField);
               mFieldByName.insert(colomn.first, pTableField);
           }

        }

        void AddRecord(std::map<std::string, std::string> vRecord)
        {
            int iIndex = 0;
            std::shared_ptr ptrRecord = std::make_shared<std::map<std::string, std::string> >(std::move(vRecord));
            if(ptrRecord.get() == nullptr )
                return;
            for(auto& sFields : ptrRecord.)
            {
                tableFieldPtr pTableField = vFields[index];
                if(pTableField.get())
                    (*pTableField).Add(sFields.first, ptrRecord);
                ++index;
            }
        }

        tableFieldPtr GetFieldByName(std::string& sName)
        {
            auto ptr = mFieldByName.find(sName);
            if(ptr != mFieldByName.end())
            {
                return ptr->second;
            }
            return tableFieldPtr();
        }

        tableFieldPtr GetFieldByIndex(int iIndex)
        {

            if(iIndex < vFields.size() && iIndex >=0)
            {
                tTableField = vFields.at(iIndex);
                bRet = true;
            }

            return tableFieldPtr();
        }

        int GetFieldCount()
        {
            return vFields.size();
        }
        //Begin and End here show the range of the Index;
        //Only support Index on int type;
        void GenFieldIndex(std::string& sName, std::string& sMathType, int iBegin, int iEnd)
        {
            if(GetFieldCount() == 0)
                return;
            if((*GetFieldByIndex(0)).KeyCount() == 0)
            {
                throw "Require table data to gen index";
            }

            tableFieldPtr pField = GetFieldByName(sName);

            if(!tableFieldPtr.get())
            {
                throw "Field not found: +" + sName;
            }

            MatchType eMatchType = MatchTypeHelper::Parse(sMathType);

            if(eMatchType == MatchType::Unknown)
            {
                throw "Unkown match type: " + sMathType;
            }

            for(int i = iBegin; i <= iEnd; ++i)
            {
                MemSQL::TableField::recordList vIndexList;
                switch( eMatchType )
                {
                    case MatchType::NotEqual:
                    {
                        for (int j = i;j<= iEnd;j++)
                        {
                            if (j == i)
                                continue;
                            FieldType eFieldType = (*pField).GetTableFieldType();
                            MemSQL::TableField::recordList vRecordList;
                             if((*pField).GetByKey(std::to_string(j), eFieldType, vRecordList));
                            vIndexList.push_back(vIndexList.end(),vRecordList.begin(), vRecordList.end());
                        }
                    }
                        break;
                    case MatchType.Great:
                    {
                        // 大于当前值的所有列表合并
                        for (int j = i + 1; j <= iEnd; j++)
                        {
                            FieldType eFieldType = (*pField).GetTableFieldType();
                            MemSQL::TableField::recordList vRecordList;
                            if((*pField).GetByKey(std::to_string(j), eFieldType, vRecordList));
                            vIndexList.push_back(vIndexList.end(),vRecordList.begin(), vRecordList.end());
                        }

                    }
                        break;
                    case MatchType.GreatEqual:
                    {
                        // 大于等于当前值的所有列表合并
                        for (int j = i; j <= iEnd; j++)
                        {
                            FieldType eFieldType = (*pField).GetTableFieldType();
                            MemSQL::TableField::recordList vRecordList;
                            if((*pField).GetByKey(std::to_string(j), eFieldType, vRecordList));
                            vIndexList.push_back(vIndexList.end(),vRecordList.begin(), vRecordList.end());
                        }
                    }
                        break;
                    case MatchType.Less:
                    {
                        for (int j = begin; j < i; j++)
                        {
                            FieldType eFieldType = (*pField).GetTableFieldType();
                            MemSQL::TableField::recordList vRecordList;
                            if((*pField).GetByKey(std::to_string(j), eFieldType, vRecordList));
                            vIndexList.push_back(vIndexList.end(),vRecordList.begin(), vRecordList.end());
                        }
                    }
                        break;
                    case MatchType.LessEqual:
                    {
                        for (int j = begin; j <= i; j++)
                        {
                            FieldType eFieldType = (*pField).GetTableFieldType();
                            MemSQL::TableField::recordList vRecordList;
                            if((*pField).GetByKey(std::to_string(j), eFieldType, vRecordList));
                            vIndexList.push_back(vIndexList.end(),vRecordList.begin(), vRecordList.end());
                        }
                    }
                        break;
                }

                (*pField).AddIndexData(eMatchType, std::to_string(i), vIndexList);
            }


        }
    };






}



#endif //MEMSQL_TABLE_H
