#include <tuple>
#include <utility>
#include <iostream>
#include "Table.h"
#include "Query.h"
#include "TableField.h"

int main()
{
	std::map<std::string, MemSQL::FieldType> vMyDefine;

	vMyDefine.insert(std::make_pair("id", MemSQL::FieldType::INT));
	vMyDefine.insert(std::make_pair("level", MemSQL::FieldType::INT));
	vMyDefine.insert(std::make_pair("score", MemSQL::FieldType::INT));

    MemSQL::Table myTable(vMyDefine);

}