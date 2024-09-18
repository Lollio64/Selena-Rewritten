#include "codegen_pica.hpp"

template<int amount>
PicaRegister PicaRegisters::Allocate(char reg) {
    int count = amount;

    switch(reg) {
        case 'v':
        for(int i = 0; i < 16; i++) {
            for(int j = 0; j < 4; j++) {
                if(!inputs[i][j])
                    count--;
                if(!count)
                    break;
            }
        }
        break;
        case 'o':
        break;
        case 'r':
        break;
        case 'c':
        break;
        case 'i':
        break;
        case 'b':
        break;
        case 'a':
        break;
    }
    return 0;
}

    template<int amount>
    void Free(PicaRegister registers);
