#ifndef DECODE_RAWDATA_BASE_H
#define DECODE_RAWDATA_BASE_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2018
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2018 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DecodeRawDataBase.h
//  
//
//  Created by Haolai TIAN 6th July 2018.
//
//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



class DecodeRawDataBase{
	protected:
		void U32_1st(uint8_t* Input, uint32_t* Output);
		void U32_2ed(uint8_t* Input, uint32_t* Output);
		void U32_3rd(uint8_t* Input, uint32_t* Output);
		void U32_4th(uint8_t* Input, uint32_t* Output);
                
	public:

		//uint32_t Decode_RawDataSegment(uint8_t *buff)=0;

                void U24to2U12(uint32_t* Input, uint32_t* Output_1, uint32_t* Ouput_2);
                void U8to2U4(uint32_t* Input, uint32_t* Output_1, uint32_t* Ouput_2);
                void U32toU12U20(uint32_t* Input, uint32_t* Output_1, uint32_t* Ouput_2);
                void U32toU14U12U6(uint32_t* Input, uint32_t* Output_1, uint32_t* Ouput_2, uint32_t* Ouput_3);
};


#endif
