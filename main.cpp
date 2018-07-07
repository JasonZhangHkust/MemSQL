#include <tuple>
#include <utility>
#include <iostream>
#include "Table.h"
#include "Query.h"
#include "TableField.h"

int main()
{
    std::map<std::string, MemSQL::FieldType> vTableDefine = {{"id", 0}, {"level",0}, {"score",0}};
    MemSQL::Table(vTableDefine);

}