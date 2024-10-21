#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <string>
#include <vector>

struct TableEntry {
    enum {
        Function,
        Variable,
        Parameter,
    };

    int entryType;
    int typeSpecifier;
    int typeQualifier;
    std::string identifer;
};

class SymbolTable {
    public:
    SymbolTable();

    void OpenScope();
    void CloseScope();

    int EntryIndex(std::string id);

    // Scopes and Table entries
    std::vector<TableEntry> entries;
    std::vector<SymbolTable> scopes;

    // Reserved keywords
    std::vector<std::string> reserved;

    TableEntry* Lookup(std::string id);
    TableEntry* Insert(std::string id, int type);
};
#endif /* SYMBOL_HPP */