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

    MemSQL::Table myTable(vMyDefine);

}