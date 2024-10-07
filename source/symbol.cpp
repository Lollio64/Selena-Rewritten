#include "symbol.hpp"
#include <algorithm>

TableEntry* SymbolTable::Insert(std::string id, int type) {
    if(EntryIndex(id) == -1) {
        entries.push_back((TableEntry){type, id});
    }

    return Lookup(id);
}

TableEntry* SymbolTable::Lookup(std::string id) {
    return &entries[EntryIndex(id)];
}

int SymbolTable::EntryIndex(std::string id) {
    for (int i = 0; i < entries.size(); i++) {
        if(entries[i].identifer.compare(id) == 0) {
            return i;
        }
    }   
    return -1;
}

void SymbolTable::OpenScope() {
    scopes.push_back(*this);
    *this = SymbolTable();
}

void SymbolTable::CloseScope() {
    ///scopes.erase(std::remove(scopes.begin(), scopes.end(), scopes.end()), scopes.end());
    *this = scopes.back();
}