#include "codegen_shbin.hpp"

enum { ConstBool = 0, ConstIVec4 = 1, ConstFVec4 = 2};
enum { VertexShader = 0, GeometryShader = 1 };

typedef struct {
    int magic = 'D' | 'V' << 8 | 'L' << 16 | 'B' << 24;
    int dvleCount; 
    int dvleOffset;
} __attribute__((packed)) DVLB;

typedef struct {
    int magic = 'D' | 'V' << 8 | 'L' << 16 | 'P' << 24;
    int unknown;
    int blobOffset;
    int blobSize;
    int descOffset;
    int descEntryCount;
    int unknown1;
    int unknown2;
    int symOffset;
    int fileSymSize;
} __attribute__((packed)) DVLP;

typedef struct {
    int magic = 'D' | 'V' << 8 | 'L' << 16 | 'E' << 24;
    int unknown;
    char shaderType;
    bool mergeOutmaps = true;
    int mainOffset;
    int endMainOffset;
    short inputBitmask;
    short outputBitmask;
    char geoShaderType;
    char startingConstantRegister;
    char vertexCount;
    int constantTableOffset;
    int constantTableEntryCount;
    int labelTableOffset;
    int labelTableEntryCount;
    int outputRegisterTableOffset;
    int outputRegisterTableCount;
    int uniformTableOffset;
    int uniformTableEntryCount;
    int symbolTableOffset;
    int symbolTableSize;
} __attribute__((packed)) DVLE;

typedef struct {
    short labelId;
    short unknown;
    int programBlobOffset;
    int labelLocationSize;
    int labelSymbolOfset;
} __attribute__((packed)) LableEntry;

typedef struct {
    char constantType;
    char padding;
    char constantRegister;
    char padding1;
    int constantValue[4];
} __attribute__((packed)) ConstantEntry;

typedef struct {
    short outputType;
    short registerID;
    short outputComponentMask;
    short unknown;
} __attribute__((packed)) OutputEntry;

typedef struct {
    int dvleSymbolOffset;
    int registerStartIndex;
    int registerEndIndex;
} __attribute((packed)) UniformEntry;