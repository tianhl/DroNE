#ifndef RAWDATA_INPUT_SVC_H
#define RAWDATA_INPUT_SVC_H

#include <vector>
#include <string>

#include "DroNECore/DataInputSvc.h"

class DataSvc;
class DataProvideSvc;
class DecodeRawData;

class RawDataInputSvc : public DataInputSvc
{
    public :

      RawDataInputSvc(const std::string& name);
      virtual ~RawDataInputSvc();

      bool initialize();
      bool finalize();
      bool next();

    private :
      size_t nextSegment();
      uint8_t* readByte();

    private :

      DataSvc*          m_dataSvc;
      DataProvideSvc*   m_dataPvdSvc;

      DecodeRawData*    m_decoder;

      uint8_t*          m_dataBuff;
      uint32_t          m_offset;
      uint32_t          m_buffsize;
      uint32_t          m_currbuffsize;
      bool              m_isLastSegment;
      //std::vector<std::string> m_inputFile;

};


#endif
