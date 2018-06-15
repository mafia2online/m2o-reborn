/** @file config.hpp
*  @brief Game's config reader
*
*
*  @author Tyldar
*/
#pragma once
#include "CommonHeaders.h"

#include "m2sdk.h"

#include <string>
#include <fstream>
#include <map>
#include <vector>

namespace M2
{
    namespace Wrappers
    {
        namespace Config
        {
            class SDesc
            {
            public:
                char            *m_name;
                unsigned short  m_size;
                unsigned short  m_type;
                uint16_t        m_u16_0;
                uint16_t        m_u16_1;
                uint16_t        m_u16_2;
                uint16_t        m_u16_3;
                uint16_t        m_u16_4;

                SDesc() {};
                SDesc(std::ifstream &file)
                {
                    uint16_t    sdsNum;

                    file.read(reinterpret_cast<char *>(&sdsNum), sizeof(sdsNum));
                    //TODO: Read name from main config file
                    file.read(reinterpret_cast<char *>(&m_size), sizeof(m_size));
                    file.read(reinterpret_cast<char *>(&m_type), sizeof(m_type));
                    file.read(reinterpret_cast<char *>(&m_u16_0), sizeof(m_u16_0));
                    file.read(reinterpret_cast<char *>(&m_u16_1), sizeof(m_u16_1));
                    file.read(reinterpret_cast<char *>(&m_u16_2), sizeof(m_u16_2));
                    file.read(reinterpret_cast<char *>(&m_u16_3), sizeof(m_u16_3));
                    file.read(reinterpret_cast<char *>(&m_u16_4), sizeof(m_u16_4));
                }
            };

            class SUnk
            {
            public:
                char            *m_name;
                uint32_t        m_u32_1;
                uint32_t        m_u32_2;
                uint8_t         m_u8_1;
                SDesc           *m_desc[1000];

                SUnk() {};
                SUnk(std::ifstream &file)
                {
                    uint16_t    sdsNum;
                    uint16_t    sdsNum2;

                    file.read(reinterpret_cast<char *>(&sdsNum), sizeof(sdsNum));
                    //TODO: Read name from main config file
                    file.read(reinterpret_cast<char *>(&m_u32_1), sizeof(m_u32_1));
                    file.read(reinterpret_cast<char *>(&m_u32_2), sizeof(m_u32_2));
                    file.read(reinterpret_cast<char *>(&m_u8_1), sizeof(m_u8_1));
                    file.read(reinterpret_cast<char *>(&sdsNum2), sizeof(sdsNum2));

                    for (unsigned short num3 = 0; num3 < sdsNum2; num3++)
                    {
                        m_desc[(int)num3] = new SDesc(file);
                    }
                }
            };

            class SProp
            {
            public:
                char            *m_name;
                uint32_t        m_u32_1;
                uint32_t        m_u32_2;
                SUnk            *m_unk[1000];

                SProp() {};
                SProp(std::ifstream &file)
                {
                    uint16_t    sdsNum;
                    uint16_t    sdsNum2;

                    file.read(reinterpret_cast<char *>(&sdsNum), sizeof(sdsNum));
                    //TODO: Read name from main config file
                    file.read(reinterpret_cast<char *>(&m_u32_1), sizeof(m_u32_1));
                    file.read(reinterpret_cast<char *>(&m_u32_2), sizeof(m_u32_2));
                    file.read(reinterpret_cast<char *>(&sdsNum2), sizeof(sdsNum2));

                    for (unsigned short num3 = 0; num3 < sdsNum2; num3++)
                    {
                        m_unk[(int)num3] = new SUnk(file);
                    }
                }
            };

            class Slot
            {
            public:
                char            *m_name;
                SDesc           *m_desc[1000];
                SProp           *m_prop[1000];

                Slot() {};
                Slot(std::ifstream &file)
                {
                    uint16_t    sdsNum;
                    uint16_t    sdsNum2;

                    file.read(reinterpret_cast<char *>(&sdsNum), sizeof(sdsNum));
                    //TODO: Get config file name
                    file.read(reinterpret_cast<char *>(&sdsNum2), sizeof(sdsNum2)); 
                    unsigned short num3;
                    for (num3 = 0; num3 < sdsNum2; num3++)
                    {
                        m_desc[(int)num3] = new SDesc(file);
                    }

                    file.read(reinterpret_cast<char *>(&sdsNum2), sizeof(sdsNum2));
                    for (unsigned short num4 = 0; num4 < sdsNum2; num4++)
                    {
                        m_prop[(int)num4] = new SProp(file);
                    }
                }
            };


            class Config
            {
            private:
                std::map<long, std::string> m_bRead;
                std::map<std::string, unsigned short> m_bWrite;
            public:

                std::ifstream   m_file;
                Slot            *m_slots[1000];
                unsigned short  m_offset;

            public:
                Config() {};
                Config(std::string file): m_file()
                {
                    m_file.open(file, std::ios::in | std::ios::binary);
                }
                ~Config()
                {
                    m_file.close();
                }

                std::string GetName(long offset)
                {
                    return m_bRead[offset];
                }

                bool Process()
                {
                    uint32_t            sdsConfig;
                    uint32_t            sdsVersion;
                    uint32_t            sdsNum;
                    uint16_t            sdsNum2;

                    m_bRead.clear();
                    m_file.clear();
                    if (!m_file.is_open()) {
                        mod_log("[ConfigEditor] : File is not open");
                        return false;
                    }

                    m_file.read(reinterpret_cast<char *>(&sdsConfig), sizeof(sdsConfig));
                    if (sdsConfig != 1935962992) {
                        mod_log("[ConfigEditor]: Not a sds config file");
                        return false;
                    }

                    m_file.read(reinterpret_cast<char *>(&sdsVersion), sizeof(sdsVersion));
                    if (sdsVersion != 2) {
                        mod_log("[ConfigEditor]: Unsupported SDS version");
                        return false;
                    }

                    m_file.read(reinterpret_cast<char *>(&sdsNum), sizeof(sdsNum));
                    m_file.read(reinterpret_cast<char *>(&sdsNum2), sizeof(sdsNum2));

                    for (unsigned short num3 = 0; num3 < sdsNum2; num3++)
                    {
                        //m_slots[(int)num3] = new Slot(m_file);
                    }

                    return true;
                }
            };
        };
    };
};
