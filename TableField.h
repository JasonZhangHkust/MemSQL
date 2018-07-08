#ifndef MEMSQL_TABLEFIELD_H
#define MEMSQL_TABLEFIELD_H

#include "TableBase.h"



namespace MemSQL
{
	class Query;
    //typedef  std::shared_ptr<std::map<std::string, std::string> > pRecord;

    class unequalData
    {
    public:
        //
        std::map<MatchType, std::vector<pRecord> > matchTypeList;
    };

    class TableField
    {
    public:

        typedef std::vector<pRecord> recordList;

    private:
        //Equal Mapper
        // Data -> Record Pointer
        std::unordered_map<std::string, recordList> mEqualMapper;
        //Index Mapper
        // Data -> Record Pointer
        std::unordered_map<std::string, unequalData> mEtcMapper;
        FieldType eFieldType;
        std::string sFieldName;
    public:

        TableField(FieldType fd, std::string sName) : eFieldType(fd), sFieldName(sName)
        {}

		FieldType GetTableFieldType();
		std::string GetTableFieldName();
		void Add(const std::string& data, const pRecord ptr);

        //Create Index
		void AddIndexData(MatchType eMatchType, std::string sData, recordList& lRecordList);

		int KeyCount();

		bool GetByKey(const std::string& sKey, FieldType keyType, recordList& lRecordList);

		void AddListToResult(Query* q, recordList& lRecordList);

		void All(Query* q);

		void Match(Query* q, MatchType t, const std::string& data);

		static bool Compare(MatchType t, const std::string& tabData, const std::string& userExpect);

		bool MatchByIndex(Query* q, MatchType t, const std::string& tabData);
    };
};

#endif //MEMSQL_TABLEFIELD_H
