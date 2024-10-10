#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <string>
#include <vector>

struct TableEntry {
    int entryType;
    int typeSpecifier;
    int typeQualifier;
    std::string identifer;
};

class SymbolTable {
    public:
    void OpenScope();
    void CloseScope();

    int EntryIndex(std::string id);

    // Table storage
    std::vector<TableEntry> entries;
    std::vector<SymbolTable> scopes;

    TableEntry* Lookup(std::string id);
    TableEntry* Insert(std::string id, int type);
};
#endif /* SYMBOL_HPP */