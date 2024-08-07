// ***************************************************************************************
//    Project: Portable Binary File Serialization in C: Ensuring Cross-Platform Data Integrity
//    File: portable_binary.c
//    Date: 2024-08-07
//    Author: Navid Dezashibi
//    Contact: navid@dezashibi.com
//    Website: https://www.dezashibi.com | https://github.com/dezashibi
//    License:
//     Please refer to the LICENSE file, repository or website for more information about
//     the licensing of this work. If you have any questions or concerns,
//     please feel free to contact me at the email address provided above.
// ***************************************************************************************
// *  Description: Refer to readme file
// ***************************************************************************************

#include <stdint.h>
#include <stdio.h>

// Function to write a 32-bit integer in a portable way
void write_int32(FILE* file, int32_t value)
{
    uint8_t bytes[4];
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
    fwrite(bytes, sizeof(bytes), 1, file);
}

// Function to write a float in a portable way
void write_float(FILE* file, float value)
{
    uint8_t bytes[4];
    uint32_t as_int = *((uint32_t*)&value);
    bytes[0] = (as_int >> 24) & 0xFF;
    bytes[1] = (as_int >> 16) & 0xFF;
    bytes[2] = (as_int >> 8) & 0xFF;
    bytes[3] = as_int & 0xFF;
    fwrite(bytes, sizeof(bytes), 1, file);
}

// Function to read a 32-bit integer in a portable way
int32_t read_int32(FILE* file)
{
    uint8_t bytes[4];
    fread(bytes, sizeof(bytes), 1, file);
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

// Function to read a float in a portable way
float read_float(FILE* file)
{
    uint8_t bytes[4];
    fread(bytes, sizeof(bytes), 1, file);
    uint32_t as_int = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
    return *((float*)&as_int);
}

typedef struct
{
    int32_t id;
    float value;
} MyStruct;

void write_struct(FILE* file, const MyStruct* s)
{
    write_int32(file, s->id);
    write_float(file, s->value);
}

void read_struct(FILE* file, MyStruct* s)
{
    s->id = read_int32(file);
    s->value = read_float(file);
}

int main()
{
    MyStruct s = {123, 456.789};

    FILE* file = fopen("data.bin", "wb");
    if (file != NULL)
    {
        write_struct(file, &s);
        fclose(file);
    }

    MyStruct read_s;
    file = fopen("data.bin", "rb");
    if (file != NULL)
    {
        read_struct(file, &read_s);
        fclose(file);
    }

    printf("Read id: %d, value: %f\n", read_s.id, read_s.value);

    return 0;
}
