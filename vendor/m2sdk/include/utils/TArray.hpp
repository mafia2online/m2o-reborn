#pragma once

namespace M2
{
    namespace Utils
    {
        template <typename TYPE>
        class T_Array
        {
        public:
            TYPE *m_pStart;
            TYPE *m_pEnd;

            T_Array(TYPE *const pStart, const unsigned uSize)
            {
                m_pStart = pStart;
                m_pEnd = pStart + uSize;
            }

            unsigned GetSize(void) const
            {
                return (unsigned)(m_pEnd - m_pStart) / unsigned(sizeof(TYPE));
            }

            TYPE &operator[] (const unsigned uIndex)
            {
                return m_pStart[uIndex];
            }

            const TYPE &operator[] (const unsigned uIndex) const
            {
                return m_pStart[uIndex];
            }
        };
    }
}
