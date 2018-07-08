#ifndef MEMSQL_TABLE_H
#define MEMSQL_TABLE_H

#include "TableBase.h"
#include "TableField.h"
namespace MemSQL
{
    typedef std::shared_ptr<TableField> tableFieldPtr;
    class Table
    {
    private:

        std::unordered_map<std::string, tableFieldPtr> mFieldByName;
        std::vector<tableFieldPtr> vFields;
    public:

		Table(std::map<std::string, FieldType> tTableDefine)
		{
			for (auto& tColumn : tTableDefine)
			{
				tableFieldPtr pTableField = std::make_shared<TableField>(tColumn.second, tColumn.first);
				vFields.push_back(pTableField);
				mFieldByName.insert(std::make_pair(tColumn.first, pTableField));
			}

		}

		void AddRecord(std::map<std::string, std::string>& vRecord);

		tableFieldPtr GetFieldByName(std::string& sName);
		tableFieldPtr GetFieldByIndex(int iIndex);
		int GetFieldCount();

        //Begin and End here show the range of the Index;
        //Only support Index on int type;
		void GenFieldIndex(std::string& sName, std::string& sMathType, int iBegin, int iEnd);
    };






}



#endif //MEMSQL_TABLE_H
