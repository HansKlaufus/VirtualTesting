#ifndef MEMORYSTREAM_H
#define MEMORYSTREAM_H

#include <sstream>

class cMemoryStream : public std::stringstream
{
    public:
        cMemoryStream();
        cMemoryStream(std::string p_FileName);
        virtual ~cMemoryStream();

        unsigned int GetID() { return m_ID; }
        std::string  GetFileName() { return m_FileName; }
        void         SetFileName(std::string p_FileName) { m_FileName = p_FileName; }
        void         WriteToFile();
        void         WriteToFile(std::string p_FileName);

    protected:

    private:
        static unsigned int m_NextID;
        unsigned int        m_ID;
        std::string         m_FileName;
};

#endif // MEMORYSTREAM_H
