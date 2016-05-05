#ifndef CFILEIO_H
#define CFILEIO_H

#include "object.h"
#include "wing.h"

#include <wx/xml/xml.h>

class cFileIo
{
    public:
        cFileIo();
        cFileIo(cObject *p_Object);
        virtual ~cFileIo();

        void XmlWriteFile(std::string p_FileName);
        void XmlReadFile(std::string p_FileName);

    protected:

    private:
        static unsigned int m_NextID;
        unsigned int        m_ID;

        cObject            *m_Object;

        void XmlWriteNodes(wxXmlNode *p_Parent);
        void XmlWriteConnections(wxXmlNode *p_Parent);
        void XmlWriteSignalGenerators(wxXmlNode *p_Parent);
        void XmlWriteControlLoops(wxXmlNode *p_Parent);
        void XmlWriteActuators(wxXmlNode *p_Parent);
        void XmlWriteScus(wxXmlNode *p_Parent);
        void XmlWriteWing(wxXmlNode *p_Parent);

        void XmlReadNodes(wxXmlNode *p_Parent, std::vector<cIoInterface*> *p_LookupTable, std::vector< std::vector<unsigned int> > *p_DelayedSources);
        void XmlReadConnections(wxXmlNode *p_Parent, std::vector<cIoInterface*> *p_LookupTable, std::vector< std::vector<unsigned int> > *p_DelayedSources);
        void XmlReadSignalGenerators(wxXmlNode *p_Parent, std::vector<cIoInterface*> *p_LookupTable, std::vector< std::vector<unsigned int> > *p_DelayedSources);
        void XmlReadControlLoops(wxXmlNode *p_Parent, std::vector<cIoInterface*> *p_LookupTable, std::vector< std::vector<unsigned int> > *p_DelayedSources);
        void XmlReadActuators(wxXmlNode *p_Parent, std::vector<cIoInterface*> *p_LookupTable, std::vector< std::vector<unsigned int> > *p_DelayedSources);
        void XmlReadScus(wxXmlNode *p_Parent, std::vector<cIoInterface*> *p_LookupTable, std::vector< std::vector<unsigned int> > *p_DelayedSources);
        void XmlReadWings(wxXmlNode *p_Parent);
        void XmlReadWingSections(wxXmlNode *p_Parent, cWing *p_Wing, unsigned int p_NumberNodes);
};

#endif // CFILEIO_H
