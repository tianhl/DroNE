#ifndef DIFF_EVENT_H
#define DIFF_EVENT_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  Evt.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

#include "DroNECore/DataObject.h"


class Evt:public DataObject{
	public:
		const uint64_t getPixelID() const {return m_pixelid; }
		void setPixelID(uint64_t id) { m_pixelid = id; }

		const uint32_t getTOF() const {return m_tof; }
		void setTOF(uint32_t tof) { m_tof = tof; }

		const uint32_t getX() const {return m_x; }
		void setX(uint32_t x) { m_x = x; }

		const uint32_t getY() const {return m_y; }
		void setY(uint32_t y) { m_y = y; }
	private:
		uint64_t m_pixelid;
		uint32_t m_tof;
		uint32_t m_x;
		uint32_t m_y;
		// pixelcollection?
};

typedef DataList<Evt> EvtList;
#endif
