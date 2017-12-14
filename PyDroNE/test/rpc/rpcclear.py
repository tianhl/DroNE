import NEON
import time

redisip = [('10.1.34.116', 9001), ('10.1.34.116', 9011)]
#m_neonRedis = NEON.Neon.NeonRedisSentinel(redisip, i_socket_timeout=10,\
#               master_name='neonmaster', master_timeout = 120, master_password='sanlie;123', idb = 0, isWritable = True)

m_neonRedis = NEON.Neon.NeonRedis(host='localhost', port=9999, db = 0, isWritable = True)
clitpob     = NEON.Neon.NeonService.POBox(m_neonRedis, '/GPPD/process/detector', '192.168.0.1:cockpit01')


clitrpc = NEON.Neon.NeonService.NeonRPC(sendPOBox = clitpob, recvPOBox = clitpob)
clitrpc.execRPC('192.168.0.1:drone01', 'Clear', {'string':'ddddd'}, 'Print', -1.0)
#clitrpc.execRPC('192.168.0.1:drone01', 'Start', {'string':'say this word', 'times':3}, 'Print', -1.0)

time.sleep(5)

relist =  clitrpc.getRSLT()
for item in relist:
    leadtime, callbackfunc, result, error = item
    print 'leadtime: ', leadtime, ' callbackfunc: ', callbackfunc, ' result: ', result, ' error: ', error
    #params = (result,error)
    #print rpc.execute(callbackfunc, params)

