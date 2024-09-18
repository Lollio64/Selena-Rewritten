#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <vector>

struct TableEntry {
    int entryType;
    std::string identifer;
    int typeSpecifier;
    int typeQualifier;
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
#endif /* SYMBOL_H */