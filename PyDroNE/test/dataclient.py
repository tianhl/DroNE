import NEON

import time
import json

m_taskheartbeat = NEON.Data.RedisRemoteData("127.0.0.1", 9999, \
                    '/GPPD/heartbeat/detector/127.0.0.1:test')
while(True):
    time.sleep(2)
    m_taskheartbeat.load()
    try:
        j=json.loads(m_taskheartbeat.getData())
    except TypeError:
        print "NULL Data"
        continue
    print 'time: ', j["timestamp"], ' status: ', j["status"]
    print "    pulse: ", j["pulse"], " event: ", j["event"], " hits: ", j["hit"]
