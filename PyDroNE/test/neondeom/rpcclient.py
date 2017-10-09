import NEON
import time

m_neonRedis = NEON.Neon.NeonRedis(host='localhost', port=9999, db = 0, isWritable = True)
clitpob     = NEON.Neon.NeonService.POBox(m_neonRedis, '/GPPD/process/detector', '192.168.0.1:cockpit01')


clitrpc = NEON.Neon.NeonService.NeonRPC(sendPOBox = clitpob, recvPOBox = clitpob)
#clitrpc.execRPC('192.168.0.1:drone01', 'Stop', {'string':'say this word', 'times':3}, 'Print', -1.0)
clitrpc.execRPC('192.168.0.1:drone01', 'Start', {'string':'say this word', 'times':3}, 'Print', -1.0)

time.sleep(10)

relist =  clitrpc.getRSLT()
for item in relist:
    leadtime, callbackfunc, result, error = item
    print 'leadtime: ', leadtime, ' callbackfunc: ', callbackfunc, ' result: ', result, ' error: ', error
    #params = (result,error)
    #print rpc.execute(callbackfunc, params)

