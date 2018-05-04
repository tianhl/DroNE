#!/usr/bin/env python

import redis, sys
import numpy, time
import uuid, base64
import json, zlib
import gevent
import thread
import socket

try:
    import cPickle as pickle
except ImportError:
    import pickle

class NeonCore:
    class GroupedList:
        def __init__(self, impl, group, name):
            raise TypeError('Pure virtual based class for GroupList')
        def push(self, peer_name, msgs):
            raise TypeError('Pure virtual based class for GroupList')
        def pull(self):
            raise TypeError('Pure virtual based class for GroupList')

    class SortedSet:    
        def __init__(self, impl, name):
            raise TypeError('Pure virtual based class for SortedSet')
        def request(self, begin, end, isByScore):
            raise TypeError('Pure virtual based class for SortedSet')
        def publish(self, items):
            raise TypeError('Pure virtual based class for SortedSet')

    class Hashtable:    
        def __init__(self, impl, name):
            raise TypeError('Pure virtual based class for Hashtable')
        def request(self, keys):
            raise TypeError('Pure virtual based class for Hashtable')
        def publish(self, kvs):
            raise TypeError('Pure virtual based class for Hashtable')


    class NeonThread:
        class BasedMsgProc:
            def __init__(self):
                self.msgs    = []
                self.msgSize = 0
                self.msgLock = thread.allocate_lock()
                self.slot = None
        
            def addMsgs(self, msgs):
                self.msgLock.acquire()
                for msg in msgs:
                    self.msgs.append(msg)
                self.msgSize = len(self.msgs)
                self.msgLock.release()
                if(self.slot is not None):
                    self.slot.send()

            def addMsg(self, msg):
                self.msgLock.acquire()
                self.msgs.append(msg)
                self.msgSize = len(self.msgs)
                self.msgLock.release()
                if(self.slot is not None):
                    self.slot.send()
        
            def getMsgs(self):
                if(self.msgSize is 0):
                    return None
                self.msgLock.acquire()
                results      = self.msgs
                self.msgs    = []
                self.msgSize = 0
                self.msgLock.release()
                return results

class NeonGevent:
    def __init__(self, host = None, port = None, db = None, isWritable =True):
        self.host = host
        self.port = port
        if(db is None):
            raise NameError('PLS specify the db = NeonRegister')
        self.reg  = db
        self.isWritable = isWritable
        self.svc  = 0
        self.status = False
        if(self.isWritable is False): # server mode, accepts msgs from clients
            if((host is None) or (port is None)):
                raise NameError('PLS specify the host and the port')
            self.svc  = self.__ThreadedServer(self.host, self.port)
        else: # client mode, sends msgs to server
            self.svc  = self.__ThreadedClient()

    def run(self):    
        if(self.status is False): # server/client thread dose not start
            thread.start_new_thread(self.svc.run, ())
            self.status = True
        else: # server/client thread has started
            return None

    class __ThreadedClient(NeonCore.NeonThread.BasedMsgProc, object):
        def __init__(self):
            NeonCore.NeonThread.BasedMsgProc.__init__(self)
            self.tcpcli = self.GeventClient(self)

        def run(self):
            self.tcpcli.run()

        class GeventClient:
            import sys

            def __init__(self, impl):
                self.impl = impl
                #self.loop = gevent.core.loop()
                self.loop = gevent.get_hub().loop
                impl.slot = self.loop.async()
                impl.slot.start(self.__reader)

            def run(self):    
                self.loop.run()

            def __reader(self):
                # read msg from outside MsgList(Main Thread), and execute 
                results = self.impl.getMsgs()
                if(results is not None):
                    for item in results:
                        self.__execute(item)

            def __execute(self, envelope):
                host = envelope['addr']
                port = envelope['port']
                msg  = envelope['msg']

                sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                try:
                    sock.connect((host, port))
                except:
                    (ErrorType, ErrorValue, ErrorTB) = sys.exc_info()
                    print "Connect Failed: ", ErrorValue
                    return None
                sock.sendall(msg)
                sock.close()    

    class __ThreadedServer(NeonCore.NeonThread.BasedMsgProc, object):
        def __init__(self, host, port):
            # how to use super to call the father class init function?
            NeonCore.NeonThread.BasedMsgProc.__init__(self)
            self.host = host
            self.port = port

        def run(self):
            tcpsvc = self.GeventServer(self, self.host, self.port)
            tcpsvc.run()

        class GeventServer:
            from gevent import server
            from gevent import queue

            def __init__(self, impl, host, port):
                self.streamserver = gevent.server.StreamServer((host, port), self.__handle)
                self.impl = impl

            def run(self):
                self.streamserver.serve_forever()

            def __handle(self, socket, client_addr):
                #print 'client addr, IP:  ', client_addr[0], ' PORT: ', client_addr[1]
                handle = socket.makefile()
                #socket.send("hello, this is gevent\n")
                queue= gevent.queue.Queue()

                try:
                    r = gevent.spawn(self.__reader, handle, queue)
                    w = gevent.spawn(self.__writer, queue)
                    gevent.joinall([r,w])
                finally:
                    pass

            def __writer(self, queue):
                # get msg from the Queue, and write to outside MsgList(Main Thread)
                while True:
                    item = queue.get()  # if queue size is 0, then block here
                    self.impl.addMsg(item)
        
            def __reader(self, handle, queue):
                # read msg from the socket, and put it into the Queue
                for item in handle:
                    queue.put(item)

    class GroupedList:
        def __init__(self, impl, group = None, name = None):
        # impl = NeonGevent, group = NeonRegister(Group), name = NeonRegister(name)
        # for server, isWritable is False, runs GeventServer, uses pull method, resv msgs from other hosts
        # for client, isWritable is True,  runs GeventClient, uses push method, send msgs to   other hosts
            self.host = impl.host
            self.port = impl.port
            self.reg  = impl.reg  # NeonRegister
            self.svc  = impl.svc  # server or client
            self.isWritable = impl.isWritable
            if(self.isWritable is False):  # server mode, register the interface in the center db
                interface = {'name':name,
                        'addr':self.host,
                        'port':self.port}
                self.reg.register(name, attribute = interface)

        def push(self, peer_name = None, msgs = None):
            if(peer_name is not None):
                peer_addr = self.reg.request(name = peer_name)
            else:
                raise NameError('Need specify the name of peer host')
            if(msgs is None):
                return None
            if(self.isWritable is True):
                for msg in msgs:
                    envelope = {'addr':peer_addr['addr'],
                            'port':peer_addr['port'],
                            'msg':msg}
                    self.svc.addMsg(envelope)
            else:
                raise IOError('Server mode, can not send messages to other hosts')

        def pull(self):
            if(self.isWritable is False): # server mode, accepts msgs from clients
                return self.svc.getMsgs()
            else:
                raise IOError('Client mode, can not accept messages from other hosts')

class NeonRedis:
    def __init__(self, host='localhost', port=9999, db = 0, isWritable = False):
        self.host  = host
        self.port  = port
        self.db    = db
        self.isWritable = isWritable
        self.NeonConnectionError = 'Redis sever(host='+self.host+', port='+str(self.port)+') Connection Error'
        self.redis = redis.StrictRedis(host = self.host, port = self.port, db = self.db)
        try:
            response = self.redis.client_list()
        except redis.ConnectionError:
            print self.NeonConnectionError 
        else:
            pass
            #print response
    def serv_status():
        try:
            response = self.redis.client_list()
        except redis.ConnectionError:
            return False
        
        return True
            
    class GroupedList:
        def __init__(self, impl, group = None, name = None):
            self.redis = impl.redis
            self.name  = name
            if(group is None):
                raise NameError('GroupedList needs to be specified an group name!')
            else:
                self.group = group

            if(self.name is not None):
                if (self.redis.sismember(self.group, self.name)):
                    print "GroupedList(group: ", self.group, ", name: ", self.name, ") is already existed!"
                else:
                    self.redis.sadd(self.group, self.name)

        def push(self, peer_name, msgs):
            fullname = self.group+'/'+peer_name
            if(self.redis.sismember(self.group, peer_name)):
                pipe = self.redis.pipeline()
                for msg in msgs:
                    pipe.rpush(fullname, msg)
                pipe.execute()    
            else:
                raise NameError(fullname+' is not existed!')

        def pull(self):
            if(self.name is None):
                raise NameError('Pls specify the name, before pull items from GroupedList')
            name = self.group+'/'+self.name
            size = self.redis.llen(name)
            pipe = self.redis.pipeline()
            for i in xrange(size):
                pipe.lpop(name)
            results = []
            for item in pipe.execute():
                if(item is not None):
                    results.append(item)
            while True:
                result = self.redis.lpop(name)
                if(result is not None):
                    results.append(result)
                else:
                    break
            return results    

    class SortedSet(NeonCore.SortedSet):
        def __init__(self, impl, name):
            self.neon      = impl
            self.redis     = impl.redis
            self.redisint  = name
            self.sortedset = base64.b64encode(name)

        def request(self, begin = 0, end = -1, isByScore = True):
            if(isByScore):
                response = self.redis.zrangebyscore(self.sortedset, begin, end, withscores = True )
            else:
                response = self.redis.zrange(self.sortedset, begin, end, withscores = True)

            results = []
            for eachitem in response:
                counter, element = pickle.loads(eachitem[0])
                score = eachitem[1]
                results.append((counter, element, score))

            return results    

        def publish(self, items):
            results = []
            size    = len(items)
            counter = self.redis.incr(self.redisint, size) - size
            pipe = self.redis.pipeline()

            for eachitem in items:
               member, score = eachitem
               element = pickle.dumps((counter, member))
               pipe.zadd(self.sortedset, score, element)
               results.append((counter, member, score))
               counter += 1
            pipe.execute()   
            return results   

    class Hashtable(NeonCore.Hashtable):
        def __init__(self, impl, name):
            self.neon      = impl
            self.redis     = impl.redis
            self.hashtable = name

        def request(self, keys):
            try:
                response = []
                pipe = self.redis.pipeline()
                for key in keys:
                    pipe.hget(self.hashtable, key)
                for eachitem in pipe.execute():
                    if(eachitem is not None):
                        response.append(zlib.decompress(eachitem))
                    else:
                        # there is not this key-value in the redis
                        response.append(None)
            except redis.ConnectionError:
                raise IOError(self.neon.NeonConnectionError) 
            else:
                return response

        def publish(self, kvs):
            if(not self.neon.isWritable):
                raise IOError('No Access Authority!')
            try:
                response = []
                pipe = self.redis.pipeline()
                for kv in kvs:
                    pipe.hset(self.hashtable, kv[0], zlib.compress(kv[1]))
                for eachitem in pipe.execute():
                    response.append(eachitem)
            except redis.ConnectionError:
                raise IOError(self.neon.NeonConnectionError) 
            else:
                return response

class NeonRedisSentinel(NeonRedis):
    def __init__(self, sentinel_list, i_socket_timeout, master_name, master_timeout, master_password, idb=0, isWritable = False):
        from redis.sentinel import Sentinel
        sentinel=Sentinel(sentinel_list, socket_timeout = i_socket_timeout)

        self.NeonConnectionError = 'Redis Sentinel('+str(sentinel_list)+') Connection Error'
        self.isWritable = isWritable
        self.redis = sentinel.master_for(master_name, socket_timeout = master_timeout, password=master_password, db=idb)


class NeonService:
    class NeonRPC:
        # use uuid as the id
        def __init__(self, sendPOBox = None, recvPOBox = None, isServer = False, timeout = -1.0):
            if((sendPOBox is None) or (recvPOBox is None)):
                raise NameError("MUST Specify the send/recv POBOX")
            self.name       = recvPOBox.name
            self.isServer   = isServer
            self.remotecall = {}
            self.sendPOBox  = sendPOBox
            self.recvPOBox  = recvPOBox
            self.timeout = timeout
            
        class MethodCall:
            # params must be a tuple, returnValue could be a tuple
	    def execute(self, method = None, params = None):    
                if(method is None):
                    returnValue = None
	    	    errorCode   = 'PLS specify the method name'
	            return (returnValue, errorCode)	

                func = getattr(self, method)
	        if callable(func):
	    	    if((params is not None)or(len(params) is not 0)):    
	                returnValue = func(params)
	            else: 
	                returnValue = func()
	    	    errorCode = None    
	        else:
	    	    errorCode = 'The method: ' + method + ' is not callable!'
	    	    returnValue = None
	        return (returnValue, errorCode)	

        def __decode(self, encodedstring):    
            return pickle.loads(base64.b64decode(encodedstring))

        def __encode(self, obj):
            return base64.b64encode(pickle.dumps(obj))

        # client code
        def execRPC(self, peer_name, method, params, callbackfunc, timeout = -1.0):
            if(self.isServer is True):
                return False
            neonuuid  = uuid.uuid1()
            neonrpcid = json.dumps({
                'to':peer_name,
                'from':self.name,
                'uuid':self.__encode(neonuuid)
                })
            envelope = json.dumps({
                    'method': method,
                    'params':self.__encode(params),
                    'id':neonrpcid
                    })
            # local cache
            if(timeout < 0.0):
                leadtime = -1.0
            else:
                leadtime = time.time()+timeout
            self.remotecall[neonuuid] = (leadtime, envelope, callbackfunc)
            self.sendPOBox.send(peer_name, [envelope])
            return True

        def getRSLT(self):
            if(self.isServer is True):
                return False
            rpcResults = []
            for item in self.recvPOBox.recv():
                neonresult= json.loads(item)
                neonrpcid = json.loads(neonresult['id'])
                neonuuid  = self.__decode(neonrpcid['uuid'])
                result    = self.__decode(neonresult['result'])
                error     = neonresult['error']
                if self.remotecall.has_key(neonuuid):
                    leadtime, envelope, callbackfunc = self.remotecall.pop(neonuuid)
                    rpcResults.append((leadtime, callbackfunc, result, error))
                else:
                    print 'Abandoned RPC result: ', neonuuid 

            for key, value in self.remotecall.items():    
                leadtime, envelope, callbackfunc = value
                if((leadtime > 0.0) and (time.time() > leadtime)):
                    # exceed the time limit
                    result = None
                    error  = 'Client::ExceedTimeLimit'
                    rpcResults.append((leadtime, callbackfunc, result, error))
                    del self.remotecall[key]
                
            return rpcResults    

        # server code
        def recvRPC(self):
            if(self.isServer is False):
                return False
            rpclist = []
            for item in self.recvPOBox.recv():
                neonrpc   = json.loads(item)
                neonrpcid = json.loads(neonrpc['id'])
                neonuuid  = self.__decode(neonrpcid['uuid'])
                addr      = neonrpcid['from']
                params    = self.__decode(neonrpc['params'])
                callmethod= (neonuuid, neonrpc['method'], params)
                if(self.timeout > 0.0):
                    leadtime = time.time() + self.timeout
                else:
                    leadtime = -1.0
                self.remotecall[neonuuid] = (addr, leadtime)
                rpclist.append(callmethod)
            return rpclist    

        def sndRSLT(self, neonuuid, result, error):
            if(self.isServer is False):
                return False
            if not self.remotecall.has_key(neonuuid):
		print "can not find neonuuid: ", neonuuid
                return False
            peer_name, leadtime = self.remotecall.pop(neonuuid)
            neonrpcid= json.dumps({
                    'to':peer_name,
                    'from':self.name,
                    'uuid':self.__encode(neonuuid)
                    })
            envelope = json.dumps({
                    'result':self.__encode(result),
                    'error':error,
                    'id':neonrpcid
                    })
            self.sendPOBox.send(peer_name, [envelope])

            for key, value in self.remotecall.items():
                peer_name, leadtime = value
                if((leadtime > 0.0) and time.time() > leadtime):
                    neonrpcid= json.dumps({
                            'to':peer_name,
                            'from':self.name,
                            'uuid':self.__encode(key)
                            })
                    envelope = json.dumps({
                            'result':None,
                            'error':'Server::ExceedTimeLimit',
                            'id':neonrpcid
                            })
                    self.sendPOBox.send(peer_name, [envelope])
                    del self.remotecall[key]

    class Register:
        def __init__(self, remoteDB_backend, reception = None, location = 'CENTER'):
            if(reception is None):
                raise NameError('NeonRegister needs a reception name')
            else:
                self.reception = reception
            locationType = ('CENTER', 'LOCALE')
            location = location.upper()
            try:
                locationType.index(location)
            except:
                raise ValueError("location must be 'CENTER' or 'LOCALE'")
            else:
                self.location = location
            self.name     = reception+'/'+location
            self.remoteDB_backend = remoteDB_backend.Hashtable(remoteDB_backend, self.reception)
    
        def register(self, name = None, attribute = None):
            if(attribute is None):
                raise IOError('Pls specify attribute')
            self.remoteDB_backend.publish([(name, json.dumps(attribute))])
    
        def unregister(self, name = None):
            # hashtable need add delete method
            pass
    
        def request(self, name = None):
            if(name is None):
                raise IOError('Pls specify name')
            return json.loads(self.remoteDB_backend.request([name])[0])
    
    class POBox:
        def __init__(self, backend, zipcode = None, name = None):
            if(zipcode is None):
                raise NameError('POBox needs an zipcode')
            self.zipcode = zipcode
            self.name    = name
            self.backend = backend.GroupedList(backend, self.zipcode, self.name)
    
        def send(self, peer_name, msgs):
            self.backend.push(peer_name, msgs)
    
        def recv(self):
            return self.backend.pull()

class NeonData:
    class Tuple:
        #  (id, value, index) tuple
        def __init__(self, remoteDB_backend, name, data = []):
            self.name    = name
            self.remoteDB_backend = remoteDB_backend.SortedSet(remoteDB_backend, self.name)
            self.waiting = data
            self.data    = []
    
        def addItems(self, items):
            for eachitem in items:
                self.waiting.append(eachitem)
    
        def getSize(self):
            return len(self.data)
    
        def getItems(self, begin = 0 , end = -1):
            if(len(self.data)==0):
                return None
            else:
                return self.data[begin:end]
    
        def load(self, maxitem):
            if(len(self.data) == 0):
                # first load tuple from service
                lastitem = (0,0,0.0)
            else:
                lastitem = self.data[-1]
            response = self.remoteDB_backend.request(begin = lastitem[2], end = maxitem)
            for eachitem in response:
                self.data.append(eachitem)
    
        def dump(self):
            response = self.remoteDB_backend.publish(self.waiting)
            for eachitem in response:
                self.data.append(eachitem)
    
    class BasedData:
        #  (stamp, static, data)    
        def __init__(self, remoteDB_backend, name, static = None, data = None):
            self.name    = name
            self.remoteDB_backend = remoteDB_backend.Hashtable(remoteDB_backend, self.name)
            self.stamp   = (uuid.uuid1(), time.time()) # uuid for static, time for data
            self.static  = static
            self.data    = data
    
        def setStatic(self, static):
            self.static = static
            self.stamp  = (uuid.uuid1(), time.time()) # force client to refresh all data
    
        def getStatic(self):
            return self.static
    
        def setData(self, data):
            self.data  = data
            uuid = self.stamp[0]  # static is not changed
            now  = time.time()    # data is changed
            self.stamp = (uuid, now)
    
        def getData(self):
            return self.data
    
        def getStamp(self):
            return self.stamp
    
        def load(self):
            results = self.remoteDB_backend.request(['Stamp'])
            stamp = results[0]
            if(stamp is None):
                # the data has not yet dump to server
                return False
            else:
                uuid, time = pickle.loads(stamp)

            if((uuid is not self.stamp[0]) and (time is not self.stamp[1])):
                # all data need to refresh
                tasks    = ['Stamp', 'Static', 'Data']
                response = self.remoteDB_backend.request(tasks)
                self.stamp  = pickle.loads(response[0])
                self.static = pickle.loads(response[1])
                self.data   = pickle.loads(response[2])
                return True
            elif((uuid is self.stamp[0]) and (time is not self.stamp[1])):
                # only data need to refresh
                tasks    = ['Stamp', 'Data']
                response = self.remoteDB_backend.request(tasks)
                stamp    = pickle.loads(response[0])
                if(stamp[0] is not self.stamp[0]):
                    # second check false
                    self.load()
                    return True
                else:
                    # second check success
                    self.stamp = stamp
                    self.data   = pickle.loads(response[1])
                    return True
            elif((uuid is self.stamp[0]) and (time is self.stamp[1])):
                # all data is the same as the remoteDB
                return True
    
        def dump(self):
            results = self.remoteDB_backend.request(['Stamp'])
            stamp = results[0]
            if(stamp is None): # first commit, all data need to dump to service
                tasks    = [('Stamp', pickle.dumps(self.stamp)),
                           ('Static', pickle.dumps(self.static)), 
                             ('Data', pickle.dumps(self.data))]
            else:
                uuid, time = pickle.loads(stamp)
                if((uuid is not self.stamp[0]) and (time is not self.stamp[1])):
                    # all data need to refresh
                    tasks    = [('Stamp', pickle.dumps(self.stamp)),
                               ('Static', pickle.dumps(self.static)), 
                                 ('Data', pickle.dumps(self.data))]
                elif((uuid is self.stamp[0]) and (time is not self.stamp[1])):
                    # only data need to refresh
                    tasks    = [('Stamp', pickle.dumps(self.stamp)),
                                 ('Data', pickle.dumps(self.data))]
                else:
                    # all data is the same as the remoteDB
                    return None

            self.remoteDB_backend.publish(tasks)

