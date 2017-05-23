#ifndef FILEINPUTSVC_H
#define FILEINPUTSVC_H


#include <string>
#include <vector>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "DroNECore/DataProvideSvc.h"

class FileInputSvc : public DataProvideSvc{
	public:
		FileInputSvc(const std::string& name);
		virtual ~FileInputSvc();

		virtual bool   initialize();
		virtual bool   finalize();
		virtual bool   read(uint8_t* buff, uint32_t size);
		virtual size_t count() const;
	private:
		void open();
		void close();
		bool next();
		void open(const std::string& filename);
	private:
		std::ifstream               m_filestream;     
		std::vector<std::string>    m_inputFiles;
		uint32_t                    m_offset;
		uint32_t                    m_nFiles;
		uint32_t                    m_count;
};

#endif
