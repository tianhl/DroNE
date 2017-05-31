// @(#)DroNE/DroNECore:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DroNEIncidentHandler.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include "DroNECore/DroNEIncident.h"
#include "DroNECore/DroNEIncidentHandler.h"

#include <time.h>
#include <stdio.h> 
#include <sstream>
#include <iostream>
#include <Python.h>


	DroNEIncidentHandler::DroNEIncidentHandler(Task* par)
: m_par(par)
{

}

DroNEIncident* DroNEIncidentHandler::getDroNEIncident(Incident& incident){
	DroNEIncident* pIncident = dynamic_cast<DroNEIncident*>(&incident);
	if (NULL==pIncident)return NULL;
	PyObject* dict = pIncident->getPyDict();
	if(NULL == dict){
		m_dict = NULL;
	}
	else if(not PyDict_Check(dict)){
		m_dict = NULL;
		return NULL;
	}
	else{
		m_dict = dict;
	}
        return pIncident;
}

bool DroNEIncidentHandler::getBool(const char* kname){
	if(m_dict==NULL)throw SniperException("Parameters dose not set!");
	PyObject* obj = PyDict_GetItemString(m_dict, kname);
	if(NULL==obj)throw SniperException("Parameter name is invalid!");
	if(0==PyInt_AsLong(obj))return false;
	else return true;
}

int DroNEIncidentHandler::getInt(const char* kname){
	if(m_dict==NULL)throw SniperException("Parameters dose not set!");
	PyObject* obj = PyDict_GetItemString(m_dict, kname);
	if(NULL==obj)throw SniperException("Parameter name is invalid!");
	return PyInt_AsLong(obj);
}

long DroNEIncidentHandler::getLong(const char* kname){
	if(m_dict==NULL)throw SniperException("Parameters dose not set!");
	PyObject* obj = PyDict_GetItemString(m_dict, kname);
	if(NULL==obj)throw SniperException("Parameter name is invalid!");
	return PyLong_AsLong(obj);
}

float DroNEIncidentHandler::getFloat(const char* kname){
	if(m_dict==NULL)throw SniperException("Parameters dose not set!");
	PyObject* obj = PyDict_GetItemString(m_dict, kname);
	if(NULL==obj)throw SniperException("Parameter name is invalid!");
	return PyFloat_AsDouble(obj);
}

char* DroNEIncidentHandler::getChar(const char* kname){
	if(m_dict==NULL)throw SniperException("Parameters dose not set!");
	PyObject* obj = PyDict_GetItemString(m_dict, kname);
	if(NULL==obj)throw SniperException("Parameter name is invalid!");
	return PyString_AsString(obj);
}

bool DroNEIncidentHandler::handle(Incident& incident)
{
	if(NULL == getDroNEIncident(incident))return false;
	return true;
}
