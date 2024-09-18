#include "codegen_shbin.hpp"

// Both Pica Shader Types
enum { VertexShader = 0, GeometryShader = 1};

typedef struct {
    int magic = 'D' | 'V' << 8 | 'L' << 16 | 'B' << 24; // Magic "DVLB"
    int dvleCount; // One because, we only compiled one shader at a time
    int dvleOffset; // Also stays at one, because of that
} __attribute__((packed)) DVLB;

typedef struct {
    int magic = 'D' | 'V' << 8 | 'L' << 16 | 'P' << 24;
    int unknown; // Use that for padding, I guess
    int blobOffset; // Offset to the compiled shader binary blob
    int blobSize; // Size of the compiled shader binary blob
    int descOffset; // Operand descriptor table offset
    int descEntryCount; // Count of operand descriptor table entries
    int unknown1; // More padding and...
    int unknown2; // ... and more padding
    int symOffset; // Offset of filename symbol table
    int fileSymSize; // Size of filename symbol table
} __attribute__((packed)) DVLP;

typedef struct {
    int magic = 'D' | 'V' << 8 | 'L' << 16 | 'E' << 24; // Magic
    int unknown; // Padding for now
    char shaderType; // Type of shader
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