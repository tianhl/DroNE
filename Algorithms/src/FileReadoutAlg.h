#ifndef FILEREADOUTALG_h
#define FILEREADOUTALG_h

#include "SniperKernel/AlgBase.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "time.h"

class DataSvc;

class FileReadoutAlg: public AlgBase
{
    public:
        FileReadoutAlg(const std::string& name);

        ~FileReadoutAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        uint64_t m_count;
	uint64_t m_evtnum;
	uint64_t m_hitnum;
	uint64_t m_pulsenum;
	clock_t  m_start;
	clock_t  m_finish;
	std::string m_outputfile;
	int m_initXNum;
	int m_initYNum;

	std::ofstream m_ofstream0;
	std::ofstream m_ofstream1;
	std::ofstream m_ofstream2;
        int m_0pulseid;
        int m_1pulseid;
        int m_2pulseid;
        int m_0pulseCt;
        int m_1pulseCt;
        int m_2pulseCt;
	std::vector<uint64_t> m_xvect;
	std::vector<uint64_t> m_yvect;

	DataSvc* m_svc;
};

#endif
