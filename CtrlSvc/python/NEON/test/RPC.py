#!/usr/bin/env python

# after v05

import NEON
import time

class RPCMethods(NEON.Neon.NeonService.NeonRPC.MethodCall):

    def Hello(self, params):
        iString = params['string'] 
        num     = params['times']
        returnValue = ''
        singlestring = ' hello, ' + iString + '. '
        for i in xrange(num):
            returnValue = returnValue + singlestring

        return returnValue    

    def Print(self, params):
        iString = params[0]
        print 'client says: ' + iString
        error = params[1]
        print 'client get error: ', error

if __name__ == '__main__':


    m_neonRedis    = NEON.Neon.NeonRedis(host='localhost', port=9999, db = 0, isWritable = True) # server mode, set msgs to Redis
    rpc = RPCMethods()

#Redis impl
    servpob = NEON.Neon.NeonService.POBox(m_neonRedis, '/NeonRPC', 'server')    
    servrpc = NEON.Neon.NeonService.NeonRPC(sendPOBox = servpob, recvPOBox = servpob, isServer=True)

    clitpob = NEON.Neon.NeonService.POBox(m_neonRedis, '/NeonRPC', 'client')
    clitrpc = NEON.Neon.NeonService.NeonRPC(sendPOBox = clitpob, recvPOBox = clitpob)

    clitrpc.execRPC('server', 'Hello', {'string':'say this word', 'times':3}, 'Print', -1.0)
    rpclist = servrpc.recvRPC()

    for item in rpclist:
        neonid, method, params = item
        print 'uuid: ', neonid, ' method: ', method, ' params: ', params 
	returnValue, error = rpc.execute(method, params)
        servrpc.sndRSLT(neonid, returnValue, error = None)


    relist =  clitrpc.getRSLT()
    for item in relist:
        leadtime, callbackfunc, result, error = item
        print 'leadtime: ', leadtime, ' callbackfunc: ', callbackfunc, ' result: ', result, ' error: ', error 
        params = (result,error)
	print rpc.execute(callbackfunc, params)

#Gevent impl
    groupName      = '/GNeonRPC'
    m_neonReg      = NEON.Neon.NeonService.Register(m_neonRedis, groupName, 'LOCALE')
    m_neonGrecv    = NEON.Neon.NeonGevent(host='localhost', port=9998, db = m_neonReg, isWritable = False) 
    m_neonGsend    = NEON.Neon.NeonGevent(db = m_neonReg, isWritable = True)  
    servrecv = NEON.Neon.NeonService.POBox(m_neonGrecv, groupName, 'rpcservrecv')
    servsend = NEON.Neon.NeonService.POBox(m_neonGsend, groupName, 'rpcservsend')
    gservrpc = NEON.Neon.NeonService.NeonRPC(sendPOBox = servsend, recvPOBox = servrecv, isServer=True)
    
    m_neonGrecv1    = NEON.Neon.NeonGevent(host='localhost', port=9997, db = m_neonReg, isWritable = False) 
    m_neonGsend1    = NEON.Neon.NeonGevent(db = m_neonReg, isWritable = True)  
    clitrecv = NEON.Neon.NeonService.POBox(m_neonGrecv1, groupName, 'rpcclitrecv')
    clitsend = NEON.Neon.NeonService.POBox(m_neonGsend1, groupName, 'rpcclitsend')
    gclitrpc = NEON.Neon.NeonService.NeonRPC(sendPOBox = clitsend, recvPOBox = clitrecv)

    m_neonGrecv.run()
    m_neonGsend.run()
    m_neonGrecv1.run()
    m_neonGsend1.run()

    time.sleep(10)
    gclitrpc.execRPC('rpcservrecv', 'Hello', {'string':'say this word gevent', 'times':4}, 'Print', -1.0)
    time.sleep(10)

    grpclist = gservrpc.recvRPC()

    for item in grpclist:
        neonid, method, params = item
        print 'uuid: ', neonid, ' method: ', method, ' params: ', params 
	returnValue, error = rpc.execute(method, params)
        gservrpc.sndRSLT(neonid, returnValue, error = None)

    time.sleep(10)
    grelist =  gclitrpc.getRSLT()
    for item in grelist:
        leadtime, callbackfunc, result, error = item
        print 'leadtime: ', leadtime, ' callbackfunc: ', callbackfunc, ' result: ', result, ' error: ', error 
        params = (result, error)
	print rpc.execute(callbackfunc, params)

