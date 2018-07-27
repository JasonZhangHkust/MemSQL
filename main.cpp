#include <tuple>
#include <utility>
#include <iostream>
#include "Table.h"
#include "Query.h"
#include "TableField.h"

std::unordered_map<std::string, MemSQL::MatchType> MemSQL::MatchTypeHelper::mStr2Type =
		{
				{"==", MatchType::Equal},
				{"!=", MatchType::NotEqual},
				{">", MatchType::Great},
				{">=", MatchType::GreatEqual},
				{"<", MatchType::Less},
				{"<=", MatchType::LessEqual},
		};

int main()
{
	std::map<std::string, MemSQL::FieldType> vMyDefine;

	vMyDefine.insert(std::make_pair("id", MemSQL::FieldType::INT));
	vMyDefine.insert(std::make_pair("level", MemSQL::FieldType::INT));
	vMyDefine.insert(std::make_pair("score", MemSQL::FieldType::INT));
    try
    {

        MemSQL::Table myTable(vMyDefine);
        std::shared_ptr<MemSQL::Table> myTablePtr = std::make_shared<MemSQL::Table>(vMyDefine);
        std::map<std::string, std::string> mData = {{"id",    "001"},
                                                    {"level", "55"},
                                                    {"score", "324"}};
        myTablePtr->AddRecord(mData);


        MemSQL::Query myQuery(myTablePtr);
        std::string sId = "id";
        std::string sMatch = "==";
        std::string sValue = "001";
        auto vResult = myQuery.Where(sId, sMatch, sValue).Result();

        std::cout << "Final Record Retrieve|" << vResult.size() << std::endl;

        std::cin.get();
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() <<std::endl;
    }
	return 0;
}