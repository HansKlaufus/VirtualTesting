#include "memorystream.h"

#include <fstream>
#include <iostream>

unsigned int cMemoryStream::m_NextID = 0;

cMemoryStream::cMemoryStream()
{
    m_ID = cMemoryStream::m_NextID++;
}

cMemoryStream::cMemoryStream(std::string p_FileName)
{
    m_ID = cMemoryStream::m_NextID++;

    this->SetFileName(p_FileName);
}

cMemoryStream::~cMemoryStream()
{
    //dtor
}

void cMemoryStream::WriteToFile()
{
    std::ofstream v_OutputFile;

    v_OutputFile.open(m_FileName.c_str(), std::ios::out);
    if(v_OutputFile.is_open())
    {
        v_OutputFile << this->str();
        v_OutputFile.close();
    }
    else
    {
        std::cout << "ERROR in cMemoryStream::WriteToFile: Unable to open %s" << m_FileName << std::endl;
    }
}

void cMemoryStream::WriteToFile(std::string p_FileName)
{
    this->SetFileName(p_FileName);
    this->WriteToFile();
}
