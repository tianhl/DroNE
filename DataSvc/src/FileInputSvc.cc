#include "DataSvc/FileInputSvc.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"


DECLARE_SERVICE(FileInputSvc);

FileInputSvc::FileInputSvc(const std::string& name)
	: DataProvideSvc(name)
{
	declProp("InputFile", m_inputFiles);
}

FileInputSvc::~FileInputSvc(){
}

bool FileInputSvc::initialize(){
	m_nFiles = m_inputFiles.size();
	m_offset = 0;
	if (m_nFiles > 0 ) open();
	else throw SniperException("PLS specified input file names!");
	return true;
}

bool FileInputSvc::finalize()
{
	return true;
}

void FileInputSvc::open(){
	LogInfo << "Open File: " << m_inputFiles[m_offset] << std::endl;
	open(m_inputFiles[m_offset++]);
}


void FileInputSvc::open(const std::string& name){
	m_filestream.open(name.c_str(), std::ios::binary);
	if(!m_filestream.is_open()) throw SniperException("Can not open file " + name);
}

void FileInputSvc::close(){
	m_filestream.close();
}

bool FileInputSvc::read(uint8_t* buff, uint32_t buffsize){
	m_filestream.read((char*)buff, buffsize);
	//size_t size = count();
	size_t size = m_filestream.gcount();
        m_count     = size;
	if(size < buffsize){
		if (next()) {
                    m_filestream.read((char*)(buff+size), buffsize-size);
                    m_count += m_filestream.gcount();
                 }
		else return false;
	};
	return true;
}


bool FileInputSvc::next(){
	close();
	if(m_offset ==  m_nFiles) return false;
	open();
	return true;
}

size_t FileInputSvc::count() const{
        return m_count;
}
