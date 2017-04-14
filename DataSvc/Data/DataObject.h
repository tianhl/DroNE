#ifndef DATA_OBJECT_H
#define DATA_OBJECT_H

#include <stdint.h>
#include <deque>

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
