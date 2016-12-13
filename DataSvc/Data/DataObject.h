#ifndef DATA_OBJECT_H
#define DATA_OBJECT_H

#include <stdint.h>
#include <deque>

/** @class DataObject 

    DataObject is a basic data class in DroNE.
    All data object in DataSvc must be drived from this class. 

    @author Haolai TIAN, CSNS, IHEP, CAS
    @date 12/12/2016

    Copyright &copy; 2016-now CSNS, IHEP, CAS

    This file is part of DroNE.

    DroNE is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    DroNE is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
class DataObject{
	public:
		virtual ~DataObject(){}
	//private:
	//	std::string m_name;
};

template<class DATA>
class DataList: public DataObject{

  typedef std::deque<DATA*>  DATALIST;
  typedef typename DATALIST::iterator DATAITER;
  

  public:
    virtual ~DataList(){}
    // iterator
    DATAITER begin(){return _list.begin();}
    DATAITER end(){return _list.end();}
    // capacity
    uint32_t size(){return _list.size();}
    bool empty(){return _list.empty();}
    // access
    DATA* at(uint32_t i){return _list.at(i);}
    DATA* front(){return _list.front();}
    DATA* back(){return _list.back();}
    // modify
    void  push_back(DATA* o){_list.push_back(o);}
    DATA* add_item(){push_back(new DATA); return back();}
    void  clear();


  private:
    DATALIST _list;

};

template<class DATA>
void DataList<DATA>::clear(){
  for(DATAITER it = begin(); it != end(); it++)delete *it;
  _list.clear();
};


#endif
