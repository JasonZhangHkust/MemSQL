#include "TableField.h"
#include "Query.h"

namespace MemSQL
{
	FieldType TableField::GetTableFieldType()
	{
		return eFieldType;
	}

	std::string TableField::GetTableFieldName()
	{
		return sFieldName;
	}
	void TableField::Add(const std::string& data, const pRecord ptr)
	{
		//Find Key Then append record to the end
		if (mEqualMapper.find(data) != mEqualMapper.end())
		{
			mEqualMapper.at(data).push_back(ptr);
		}
		else
		{
			//Not Find then add a new List;
			recordList lRecordList = { ptr };
			mEqualMapper.insert(std::make_pair(data, lRecordList));
		}
	}

	//Create Index
	void TableField::AddIndexData(MatchType eMatchType, std::string sData, recordList& lRecordList)
	{
		if (mEtcMapper.find(sData) != mEtcMapper.end())
		{
			mEtcMapper.at(sData).matchTypeList[eMatchType] = lRecordList;
		}
		else
		{
			unequalData ud;
			ud.matchTypeList.insert({ eMatchType, lRecordList });
			mEtcMapper.insert(std::make_pair(sData, ud));
		}
	}

	int TableField::KeyCount()
	{
		return mEqualMapper.size();
	}

	bool TableField::GetByKey(std::string& sKey, FieldType keyType, recordList& lRecordList)
	{
		bool bRet = false;
		if (mEqualMapper.find(sKey) != mEqualMapper.end())
		{
			lRecordList = mEqualMapper.at(sKey);
			bRet = true;
		}

		return bRet;
	}

	void TableField::AddListToResult(Query* q, recordList& lRecordList)
	{
		for (auto& record : lRecordList)
		{
			q->Add(record);
		}
	}

	void TableField::All(Query* q)
	{
		for (auto& kv : mEqualMapper)
		{
			AddListToResult(q, kv.second);
		}
	}

	void TableField::Match(Query* q, MatchType t, const std::string& data)
	{
		switch (t)
		{
		case MatchType::Equal:
		{
			recordList lRecordList;
			if (GetByKey(data, this.fieldType, lRecordList))
			{
				AddListToResult(q, lRecordList);
			}
			break;
		}
		case MatchType::NotEqual:
		{
			if (!MatchByIndex(q, t, data))
			{
				for (auto& kv : mEqualMapper)
				{
					if (kv.first != data)
					{
						AddListToResult(q, kv.second);
					}
				}
			}
			break;
		}
		default:
		{
			// 使用索引过的数据
			if (MatchByIndex(q, t, data))
			{
				return;
			}

			// 暴力匹配
			for (auto& kv : mEqualMapper)
			{
				if (Compare(t, kv.first, data))
				{
					AddListToResult(q, kv.second);
				}
			}
			break;
		}
		}
	}

	static bool TableField::Compare(MatchType t, std::string& tabData, std::string& userExpect)
	{
		bool bRet = false;
		switch (t)
		{
		case MatchType::Less:
			bRet = tabDataT < userExpectT;
			break;
		case MatchType::LessEqual:
			bRet = tabDataT <= userExpectT;
			break;
		case MatchType::Great:
			bRet = tabDataT > userExpectT;
			break;
		case MatchType::GreatEqual:
			bRet = tabDataT >= userExpectT;
			break;
		default:
			bRet = false;
		}
		return bRet;
	}

	bool TableField::MatchByIndex(Query* q, MatchType t, std::string& tabData)
	{
		if (mEtcMapper.empty())
			return false;

		if (mEtcMapper.find(tabData) != mEtcMapper.end())
		{
			unequalData& ud = mEtcMapper.at(tabData);
			if (ud.matchTypeList.find(t) != ud.matchTypeList.end())
			{
				AddListToResult(q, ud.matchTypeList.at(t));
			}
			else
			{
				throw "Match type index not built: " + std::to_string(t);
			}

			AddListToResult(q, ud.matchTypeList.at(t));
			return true;
		}
		return false;
	}
}