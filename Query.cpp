#include "Query.h"
#include "TableField.h"
#include "Table.h"

namespace MemSQL
{
	void Query::Reset()
	{
		done = false;
		iLimit = -1;
		vConditions.clear();
		vRet.clear();
	}

	Query& Query::Where(std::string& sFieldName, std::string& sMatchType, std::string& sValue)
	{
		MatchType eMatchType = MatchTypeHelper::Parse(sMatchType);

		if (eMatchType == MatchType::Unknown)
		{
			throw "Unknow match type";
		}

		if (!pTable.get())
		{
			throw "Empty Table";
		}
		Condition tCon;
		tCon.matchType = eMatchType;
		tCon.pTableField = (*pTable).GetFieldByName(sFieldName);
		tCon.sValue = sValue;

		vConditions.push_back(tCon);
		return *this;
	}

	Query& Query::Limit(int count)
	{
		if (count < 0)
		{
			throw "Count should > 0";
		}
		this->iLimit = count;

		return *this;
	}

	Query& Query::SortBy(std::function<bool(std::string&)> fCallBack)
	{
		fSorter = fCallBack;
		return  *this;
	}

	void Query::Add(pRecord ptr)
	{
		mMergeDataByData.insert(ptr);
	}

	void Query::Do()
	{
		if (done || !pTable.get())
			return;

		if (pTable->GetFieldCount() > 0)
		{
			if (vConditions.empty())
			{
				tableFieldPtr ptr = pTable->GetFieldByIndex(0);
				ptr->All(this);
			}
			else
			{
				for (auto& tCon : vConditions)
				{
					tCon.pTableField->Match(this, tCon.matchType, tCon.sValue);
				}
			}
		}

		done = true;
	}

	std::vector<std::map<std::string, std::string> > Query::Result()
	{
		Do();
		std::vector<std::map<std::string, std::string> > vResult;

		for (auto& tData : vRet)
		{
			vResult.push_back(tData);
		}

		if (fSorter)
		{
			//std::sort(vResult.begin(), vResult.end(), fSorter);
		}

		if (iLimit != -1 && iLimit < vResult.size())
		{
			vResult.erase(vResult.begin(), vResult.begin() + iLimit);
		}

		return vResult;
	}

}