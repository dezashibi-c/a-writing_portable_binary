# Portable Binary File Serialization in C: Ensuring Cross-Platform Data Integrity

A simple guide and example code for creating portable binary files in C, addressing architecture-specific challenges like endianness and data alignment.

**Get full source code [here](/portable_binary.c).**

## Introduction

In systems programming, it is often necessary to store data in binary files for efficient storage and retrieval. However, binary data can be architecture-specific, which poses challenges when transferring files between different systems.
oOne simple example of this happens when writing a file using a program compiled using x64 compiler and reading that data using a program compiled using x86 compiler.

This short article explores the methods for creating portable binary files in C, ensuring data consistency across diverse architectures.

## The Challenge of Portability

Binary files store data in a raw, unencoded format that is directly mapped from memory. This direct mapping leads to potential issues:

1. **Endianness**: Different systems use different byte orders for multi-byte data. Big-endian systems store the most significant byte first, while little-endian systems store the least significant byte first.
2. **Data Alignment**: Compilers may introduce padding between structure members to align data in memory, which varies across systems.
3. **Data Type Sizes**: The size of basic data types like `int` and `long` can differ between architectures.

## Ensuring Portability

To achieve portability, we must standardize the binary format, explicitly controlling byte order and structure padding. This involves manually serializing and deserializing each data member, converting them to a fixed byte order, and avoiding direct memory writes.

## Implementation

Below is a simple example demonstrating how to serialize and deserialize a simple structure in a portable manner:

```c
#include <stdio.h>
#include <stdint.h>

// Function to write a 32-bit integer in a portable way
void write_int32(FILE *file, int32_t value) {
    uint8_t bytes[4];
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
    fwrite(bytes, sizeof(bytes), 1, file);
}

// Function to write a float in a portable way
void write_float(FILE *file, float value) {
    uint8_t bytes[4];
    uint32_t as_int = *((uint32_t*)&value);
    bytes[0] = (as_int >> 24) & 0xFF;
    bytes[1] = (as_int >> 16) & 0xFF;
    bytes[2] = (as_int >> 8) & 0xFF;
    bytes[3] = as_int & 0xFF;
    fwrite(bytes, sizeof(bytes), 1, file);
}

// Function to read a 32-bit integer in a portable way
int32_t read_int32(FILE *file) {
    uint8_t bytes[4];
    fread(bytes, sizeof(bytes), 1, file);
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

// Function to read a float in a portable way
float read_float(FILE *file) {
    uint8_t bytes[4];
    fread(bytes, sizeof(bytes), 1, file);
    uint32_t as_int = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
    return *((float*)&as_int);
}

typedef struct {
    int32_t id;
    float value;
} MyStruct;

void write_struct(FILE *file, const MyStruct *s) {
    write_int32(file, s->id);
    write_float(file, s->value);
}

void read_struct(FILE *file, MyStruct *s) {
    s->id = read_int32(file);
    s->value = read_float(file);
}

int main() {
    MyStruct s = {123, 456.789};
    
    FILE *file = fopen("data.bin", "wb");
    if (file != NULL) {
        write_struct(file, &s);
        fclose(file);
    }
    
    MyStruct read_s;
    file = fopen("data.bin", "rb");
    if (file != NULL) {
        read_struct(file, &read_s);
        fclose(file);
    }
    
    printf("Read id: %d, value: %f\n", read_s.id, read_s.value);
    
    return 0;
}
```

## Conclusion

By manually handling the serialization of each data member and ensuring consistent byte order, we can create binary files that are portable across different architectures. This approach mitigates issues related to endianness, data alignment, and data type size discrepancies. This example serves as a foundation for developing more complex portable binary file handling in C, paving the way for robust and architecture-agnostic data storage solutions.

## License

This project is licensed under the Creative Commons Attribution-NonCommercial 4.0 International (CC BY-NC 4.0) License.

You are free to:

- Share — copy and redistribute the material in any medium or format
- Adapt — remix, transform, and build upon the material

Under the following terms:

- Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
- NonCommercial — You may not use the material for commercial purposes.

No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.

For more details, refer to the [LICENSE](./LICENSE) file.

### Author

Navid Dezashibi  
<navid@dezashibi.com>
