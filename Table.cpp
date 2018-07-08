#include"Table.h"
#include <iostream>

namespace MemSQL
{

	void Table::AddRecord(std::map<std::string, std::string>& vRecord)
	{
		int iIndex = 0;
		std::shared_ptr<std::map<std::string, std::string> > ptrRecord = std::make_shared<std::map<std::string, std::string> >(std::move(vRecord));
		if (ptrRecord.get() == nullptr)
			return;
		for (auto& sFields : (*ptrRecord))
		{
			tableFieldPtr pTableField = vFields[iIndex];
			if (pTableField.get())
			{
				(*pTableField).Add(sFields.second, ptrRecord);
				std::cout << sFields.first << "|" << sFields.second << std::endl;
			}
				
			++iIndex;
		}
	}

	tableFieldPtr Table::GetFieldByName(std::string& sName)
	{
		auto ptr = mFieldByName.find(sName);
		if (ptr != mFieldByName.end())
		{
			return ptr->second;
		}
		return tableFieldPtr();
	}

	tableFieldPtr Table::GetFieldByIndex(int iIndex)
	{
		tableFieldPtr ptr = nullptr;
		if (iIndex < vFields.size() && iIndex >= 0)
		{
			ptr = vFields.at(iIndex);
		}

		return ptr;
	}

	int Table::GetFieldCount()
	{
		return vFields.size();
	}
	//Begin and End here show the range of the Index;
	//Only support Index on int type;
	void Table::GenFieldIndex(std::string& sName, std::string& sMathType, int iBegin, int iEnd)
	{
		if (GetFieldCount() == 0)
			return;
		if ((*GetFieldByIndex(0)).KeyCount() == 0)
		{
			throw MemSQLException("Require table data to gen index");
		}

		tableFieldPtr pField = GetFieldByName(sName);

		if (!pField.get())
		{
			throw MemSQLException("Field not found: +" + sName);
		}

		MatchType eMatchType = MatchTypeHelper::Parse(sMathType);

		if (eMatchType == MatchType::Unknown)
		{
			throw MemSQLException("Unkown match type: " + sMathType);
		}

		for (int i = iBegin; i <= iEnd; ++i)
		{
			MemSQL::TableField::recordList vIndexList;
			switch (eMatchType)
			{
			case MatchType::NotEqual:
			{
				for (int j = i; j <= iEnd; j++)
				{
					if (j == i)
						continue;
					FieldType eFieldType = (*pField).GetTableFieldType();
					MemSQL::TableField::recordList vRecordList;
					if ((*pField).GetByKey(std::to_string(j), eFieldType, vRecordList));
					vIndexList.insert(vIndexList.end(), vRecordList.begin(), vRecordList.end());
				}
			}
			break;
				case MatchType::Great:
			{

				for (int j = i + 1; j <= iEnd; j++)
				{
					FieldType eFieldType = (*pField).GetTableFieldType();
					MemSQL::TableField::recordList vRecordList;
					if ((*pField).GetByKey(std::to_string(j), eFieldType, vRecordList));
					vIndexList.insert(vIndexList.end(), vRecordList.begin(), vRecordList.end());
				}

			}
			break;
				case MatchType::GreatEqual:
			{

				for (int j = i; j <= iEnd; j++)
				{
					FieldType eFieldType = (*pField).GetTableFieldType();
					MemSQL::TableField::recordList vRecordList;
					if ((*pField).GetByKey(std::to_string(j), eFieldType, vRecordList));
					vIndexList.insert(vIndexList.end(), vRecordList.begin(), vRecordList.end());
				}
			}
			break;
				case MatchType::Less:
			{
				for (int j = iBegin; j < i; j++)
				{
					FieldType eFieldType = (*pField).GetTableFieldType();
					MemSQL::TableField::recordList vRecordList;
					if ((*pField).GetByKey(std::to_string(j), eFieldType, vRecordList));
					vIndexList.insert(vIndexList.end(), vRecordList.begin(), vRecordList.end());
				}
			}
			break;
				case MatchType::LessEqual:
			{
				for (int j = iBegin; j <= i; j++)
				{
					FieldType eFieldType = (*pField).GetTableFieldType();
					MemSQL::TableField::recordList vRecordList;
					if ((*pField).GetByKey(std::to_string(j), eFieldType, vRecordList));
					vIndexList.insert(vIndexList.end(), vRecordList.begin(), vRecordList.end());
				}
			}
			break;
			}

			(*pField).AddIndexData(eMatchType, std::to_string(i), vIndexList);
		}
	}
}