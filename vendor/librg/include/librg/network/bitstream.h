#ifndef librg_network_bitstream_h
#define librg_network_bitstream_h

#include <string.h>

namespace librg
{
    namespace network
    {
        class bitstream_t {
            public:
                bitstream_t() :
                    raw_buffer(nullptr),
                    write_offset(0),
                    read_offset(0),
                    buffer_size(0) { }

                bitstream_t(void* data, size_t length) :
                    raw_buffer(data),
                    write_offset(0),
                    read_offset(0),
                    buffer_size(length) { }


                /**
                 * Read from bitstream buffer to variable by auto geted type
                 * @param var of auto typename
                 */
                template <typename T>
                void read(T & var)
                {
                    read(reinterpret_cast<void*>(&var), sizeof(var));
                }

                /**
                 * Read from bitstream buffer to variable by specified size
                 * @param var void variable where data will be written
                 * @param variable_size specified size of variable to be readen
                 */
                void read(void* var, size_t variable_size)
                {
                    librg_assert(var != nullptr, "bitstream_t::read Unable to read nullptr!");
                    librg_assert(variable_size <= buffer_size, "bistream_t::read Unable to read from unallocated space!");

                    memcpy(var, reinterpret_cast<char*>(raw_buffer) + read_offset, variable_size);
                    read_offset += variable_size;
                }

                /**
                 * Read from bistream vector of some static type
                 * currently in experimental development types as strings dont works !
                 * working only with static sized types such as int double etc ...
                 */
                template<class T>
                auto read_stdvector() -> std::vector<T>
                {
                    std::vector<T> returnVec;
                    size_t vector_size;
                    read(vector_size);

                    for (size_t i = 0; i < vector_size; i++) {
                        T toBePushed;
                        read(toBePushed);
                        returnVec.push_back(toBePushed);
                    }

                    return returnVec;
                }

                /**
                 * Read cstring from buffer size of string is internaly sended !
                 * could be done just by checking of nullptr this is much safer
                 */
                char* read_cstr()
                {
                    size_t string_size;
                    read(string_size);

                    void* allocated_string = malloc(string_size);
                    read(allocated_string, string_size);

                    char* return_string = reinterpret_cast<char*>(allocated_string);
                    return_string[string_size] = '\0';
                    return return_string;
                }

                bool read_bool()
                {
                    bool var;
                    read(var);
                    return var;
                }

                int read_int()
                {
                    int var;
                    read(var);
                    return var;
                }

                float read_float()
                {
                    float var;
                    read(var);
                    return var;
                }

                double read_double()
                {
                    double var;
                    read(var);
                    return var;
                }

                int8_t read_int8()
                {
                    int8_t var;
                    read(var);
                    return var;
                }

                int16_t read_int16()
                {
                    int16_t var;
                    read(var);
                    return var;
                }

                int32_t read_int32()
                {
                    int32_t var;
                    read(var);
                    return var;
                }

                int64_t read_int64()
                {
                    int64_t var;
                    read(var);
                    return var;
                }

                uint8_t read_uint8()
                {
                    uint8_t var;
                    read(var);
                    return var;
                }

                uint16_t read_uint16()
                {
                    uint16_t var;
                    read(var);
                    return var;
                }

                uint32_t read_uint32()
                {
                    uint32_t var;
                    read(var);
                    return var;
                }

                uint64_t read_uint64()
                {
                    uint64_t var;
                    read(var);
                    return var;
                }

                /**
                 * Write to bitstream buffer from variable of eny type
                 * @param var wich is templated
                 */
                template <typename T>
                void write(T var)
                {
                    write(reinterpret_cast<void*>(&var), sizeof(var));
                }

                /**
                 * Write to bitstream buffer from variable by specified size
                 * @param var void type variable
                 * @param variable_size specified size of the variable
                 */
                void write(void* var, size_t variable_size)
                {
                    librg_assert(var != nullptr, "bitstream_t::write Unable to write nullptr!");

                    raw_buffer = realloc(raw_buffer, write_offset + variable_size);
                    memcpy(reinterpret_cast<char*>(raw_buffer) + write_offset, var, variable_size);

                    write_offset += variable_size;
                    buffer_size += variable_size;
                }

                /**
                 * Write to bitstream buffer from variable by specified position
                 * @param var typename of variable
                 * @param position specified offset of where variable will be written
                 */
                template <typename T>
                void write_at(T var, size_t position)
                {
                    size_t variable_size = sizeof(T);

                    librg_assert(position + variable_size <= buffer_size, "bitstream_t::write_at Unable to write to unallocated memory!");
                    memcpy(reinterpret_cast<char*>(raw_buffer) + position, &var, variable_size);
                }

                template<class T>
                void write_stdvector( std::vector<T> var )
                {
                    size_t vec_size = var.size();
                    write(vec_size);

                    for (auto current : var) {
                        write(current);
                    }
                }

                void write_cstr(char* var)
                {
                    size_t string_size = strlen(var);
                    write(string_size);
                    write(reinterpret_cast<void*>(var), string_size);
                }

                void write_bool(bool var)
                {
                    write(var);
                }

                void write_int(int var)
                {
                    write(var);
                }

                void write_float(float var)
                {
                    write(var);
                }

                void write_double(double var)
                {
                    write(var);
                }

                void write_int8(int8_t var)
                {
                    write(var);
                }

                void write_int16(int16_t var)
                {
                    write(var);
                }

                void write_int32(int32_t var)
                {
                    write(var);
                }

                void write_int64(int64_t var)
                {
                    write(var);
                }

                void write_uint8(uint8_t var)
                {
                    write(var);
                }

                void write_uint16(uint16_t var)
                {
                    write(var);
                }

                void write_uint32(uint32_t var)
                {
                    write(var);
                }

                void write_uint64(uint64_t var)
                {
                    write(var);
                }

                /**
                 * Flush the buffer usefull for later use of same instance
                 */
                void flush()
                {
                    read_offset  = 0;
                    write_offset = 0;
                    buffer_size  = 0;

                    if (raw_buffer != nullptr) {
                        free(raw_buffer);
                    }
                }

                /**
                 * Returns raw buffer data usefull for sending over network
                 */
                void* raw() { return raw_buffer; }

                /**
                 * Move cursor to different position
                 */
                void move(size_t position) {
                    librg_assert(position <= buffer_size, "bitstream_t::move Unable to move to unallocated memory!");

                    write_offset = position;
                    read_offset  = position;
                }

                /**
                 * Skip some bytes while reading
                 * @param amount
                 */
                void skip(size_t amount) {
                    move(read_offset + amount);
                }

                /**
                 * Get raw size of the raw buffer
                 */
                size_t raw_size() { return write_offset; }

                /**
                 * Set raw size of the raw buffer
                 */
                void set_raw(void* raw_data, size_t length) {
                    raw_buffer  = raw_data;
                    buffer_size = length;
                }

            private:
                void * raw_buffer;
                size_t write_offset;
                size_t read_offset;
                size_t buffer_size;
        };
    }
}

#endif // librg_network_bitstream_h
