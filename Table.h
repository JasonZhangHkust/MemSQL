#ifndef MEMSQL_TABLE_H
#define MEMSQL_TABLE_H

#include "TableBase.h"
#include "TableField.h"
namespace MemSQL
{
    class Table
    {
    private:
        std::unordered_map<std::string, TableFiled> mFieldByName;
        std::list<TableFiled> lFields;
    public:
        Table(std::vector<std::pair<std::string, FieldType> > tTableDefine)
        {
           for(auto& tColumn : tTableDefine)
           {
               switch (colomn.second)
               {
                   case FieldType::INT:
                   {
                       auto& tf = TableField(colomn.second);
                       lFields.push_back(tf);
                       mFieldByName.insert(colomn.first, tf);
                   }
               }
               auto& tf = TableField<>(colomn.second);
               lFields.push_back(tf);
               mFieldByName.insert(colomn.first, tf);
           }
        }
        void AddRecord(std::map<std::string, std::string>& vRecord)
        {
            int iIndex = 0;
            for(auto& sFields : vRecord)
            {
                auto& tRecordField = fields[index];
                auto& key =
            }
        }
    };






}



#endif //MEMSQL_TABLE_H
