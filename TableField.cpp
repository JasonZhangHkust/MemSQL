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

	void TableField::Add(const std::string& sData, const pRecord ptr)
	{
		//Find Key Then append record to the end
		if (mEqualMapper.find(sData) != mEqualMapper.end())
		{
			mEqualMapper.at(sData).push_back(ptr);
		}
		else
		{
			//Not Find then add a new List;
			recordList lRecordList = { ptr };
			mEqualMapper.insert(std::make_pair(sData, lRecordList));
		}
		std::cout << "EqualMapper" << mEqualMapper.size() << std::endl;
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

	bool TableField::GetByKey(const std::string& sKey, FieldType keyType, recordList& lRecordList)
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
		std::cout << "Record Size"<< lRecordList.size() << std::endl;
		for (auto& record : lRecordList)
		{
			q->Add(record);
		}
	}

	void TableField::All(Query* q)
	{
		std::cout << "EqualMapper Size|" << mEqualMapper.size() << std::endl;

		for (auto kv : mEqualMapper)
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
			if (GetByKey(data, this->eFieldType, lRecordList))
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
			// ʹ��������������
			if (MatchByIndex(q, t, data))
			{
				return;
			}

			// ����ƥ��
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

	bool TableField::Compare(MatchType t, const std::string& tabData, const std::string& userExpect)
	{
		bool bRet = false;
		switch (t)
		{
		case MatchType::Less:
			bRet = tabData < userExpect;
			break;
		case MatchType::LessEqual:
			bRet = tabData <= userExpect;
			break;
		case MatchType::Great:
			bRet = tabData > userExpect;
			break;
		case MatchType::GreatEqual:
			bRet = tabData >= userExpect;
			break;
		default:
			bRet = false;
		}
		return bRet;
	}

	bool TableField::MatchByIndex(Query* q, MatchType t, const std::string& tabData)
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