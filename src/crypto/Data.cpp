#include "Data.hpp"
#include <string.h>
#include <cstdlib>
#include "Log.h"

Data::Data(unsigned char* bytes, size_t size) {
    if (bytes && size > 0) {
        m_bytes = (unsigned char *)malloc(size);
        memcpy(m_bytes, bytes, size);
        m_size = size;
    }
}

Data::Data(size_t capacity) {
    if (capacity > 0) {
        m_bytes = (unsigned char *)malloc(capacity + 1);
        m_bytes[capacity] = '\0';
        m_size = capacity;
    }
}

Data::~Data() {
    if (m_bytes) {
        free(m_bytes);
    }
}

Data Data::subdata(size_t start, size_t size) {
    if (start + size > m_size) {
        LOG("Invalid data length...\n");
        exit(-1);
    }
    return Data(&m_bytes[start], size);
}

Data Data::append(Data other) {
    if (is_empty()) {
        return other;
    }
    
    Data data(m_size + other.m_size);
    memcpy(data.m_bytes, m_bytes, m_size);
    memcpy(&data.m_bytes[m_size], other.m_bytes, other.m_size);
    return data;
}

Data::Data(const Data& that): Data(0) {
    m_bytes = (unsigned char *)malloc(that.size());
    memcpy(m_bytes, that.bytes(), that.size());
    m_size = that.size();
}

Data& Data::operator=(const Data& that) {
    if (this != &that) {
        if (m_bytes) {
            free(m_bytes);
        }
        
        m_bytes = (unsigned char *)malloc(that.m_size);
        memcpy(m_bytes, that.m_bytes, that.m_size);
        m_size = that.m_size;
    }
    return *this;
}

Data Data::random_bytes(size_t size) {
    unsigned char* bytes = (unsigned char*)malloc(sizeof(char) * size);
    arc4random_buf(bytes, size);
    Data random_data(size);
    memcpy(random_data.m_bytes, bytes, size);
    free(bytes);
    return random_data;
}

Data Data::read_from_file(std::string path) {
    FILE* f = fopen(path.c_str(), "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        int size = (int)ftell(f);
        fseek(f, 0, SEEK_SET);
        
        char* buffer = (char*)malloc(size);
        fread(buffer, 1, size, f);
        Data data(buffer, size);
        free(buffer);
        fclose(f);
        return data;
    }
    return Data();
}

Data Data::hex_to_bytes() const {
    Data data(m_size / 2);
    char byte_chars[3] = {'\0','\0','\0'};
    unsigned long whole_byte;
    
    int i = 0, counter = 0;
    while (i < m_size) {
        byte_chars[0] = m_bytes[i++];
        byte_chars[1] = m_bytes[i++];
        whole_byte = strtoul(byte_chars, NULL, 16);
        data.m_bytes[counter++] = whole_byte;
    }
    return data;
}

Data Data::hex() const {
    int counter = 0;
    Data hex(m_size * 2);
    char fmt[3] = {'\0','\0','\0'};
    for (int i = 0; i < m_size; i++) {
        sprintf(fmt, "%02X", m_bytes[i]);
        hex.m_bytes[counter++] = fmt[0];
        hex.m_bytes[counter++] = fmt[1];
    }
    hex.m_bytes[m_size * 2] = '\0';
    return hex;
}

void Data::write_to_file(std::string path) {
    FILE *f = fopen(path.c_str(), "w");
    fwrite(m_bytes, m_size, 1, f);
    fclose(f);
}
