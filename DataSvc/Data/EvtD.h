#ifndef EVENT_DOUBLE_H
#define EVENT_DOUBLE_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2018
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2018 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  EvtD.h
//  
//
//  Created by Haolai TIAN 7th July 2018.
//
//

#include "DroNECore/DataObject.h"


class EvtD:public DataObject{
	public:
		const uint64_t getPixelID() const {return m_pixelid; }
		void setPixelID(uint64_t id) { m_pixelid = id; }

		const double getTOF() const {return m_tof; }
		void setTOF(double tof) { m_tof = tof; }

		const double getX() const {return m_x; }
		void setX(double x) { m_x = x; }

		const double getY() const {return m_y; }
		void setY(double y) { m_y = y; }
	private:
		uint64_t m_pixelid;
		double   m_tof;
		double   m_x;
		double   m_y;
		// pixelcollection?
};

typedef DataList<EvtD> EvtDList;
#endif
