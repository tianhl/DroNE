import NEON
import ROOT


#def ShowStat(dtime, dhit, devt, dpulse):
#    m_padst.cd()

m_neonRediscli   = NEON.Neon.NeonRedis(host='localhost', port=9999, db = 0, isWritable = False) # server mode, set msgs to Redis
m_taskheartbeat2 = NEON.Data.SingleValue(m_neonRediscli, '/GPPD/hearbeat/detector/192.168.0.1:drone01')
m_taskhist2      = NEON.Data.Hist1D(m_neonRediscli, '/GPPD/workspace/detector/module001')
m_canvas         = ROOT.TCanvas('Canvas', 'Dynamic HotMap', 100, 10, 600, 500)
m_pad1d          = ROOT.TPad('pad1d', '1D HotMap', 0.03, 0.61, 0.98, 0.92)
m_pad2d          = ROOT.TPad('pad2d', '2D HotMap', 0.03, 0.02, 0.98, 0.57)
m_pad1d.Draw()
m_pad2d.Draw()


m_hist1d         = ROOT.TH1D('hotmap', 'The Hot Map of Neutron Hits 1D', 111, 0, 111)
m_hist2d         = ROOT.TH2D('hotmap', 'The Hot Map of Neutron Hits 2D', 111, 0, 111, 48, 0, 48)
m_hist1d.SetStats(0)
m_hist2d.SetStats(0)
idxsize          = 111*48

m_pad1d.cd()
m_hist1d.Draw()
m_pad2d.cd()
m_hist2d.Draw('COLZ')

import time
import json

while(True):
    time.sleep(2)
    m_taskheartbeat2.load()
    try:
        j=json.loads(m_taskheartbeat2.getData())
    except TypeError:
        print "NULL Data"
        continue
    print 'time: ', j["timestamp"], ' status: ', j["status"]
    print "    pulse: ", j["pulse"], " event: ", j["event"], " hits: ", j["hit"]
    
    m_taskhist2.load()
    l = m_taskhist2.getData()
    try:
        a = l.split(':')
        if(len(a)<idxsize):
            continue
    except AttributeError:
        print "NULL HIST"
        continue
    xc = []
    for i in xrange(111):
        xc.append(0)
    
    for idx in xrange(idxsize):
        x = idx%111
        y = idx/111
        try: 
            c2 = int(a[idx])  
            m_hist2d.SetBinContent(x,y,c2)
            xc[x] += c2
        except ValueError:
            print 'ValueError'
            continue
    for x in xrange(111):
        m_hist1d.SetBinContent(x,xc[x])

 
    m_pad1d.cd()
    m_hist1d.Draw()
    m_pad2d.cd()
    m_hist2d.Draw('COLZ')
    m_canvas.Modified()
    m_canvas.Update()

