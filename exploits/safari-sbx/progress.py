import os
import time
import glob

start = time.time()
def cur():
    t = time.time() - start
    return '%d mins %d secs' % (t//60, t%60)

log = "/cores/log_webcontent*"

while not glob.glob(log):
    time.sleep(1)
print "Stage one finished at %s" % cur()

fname = glob.glob(log)[0]
while True:
    with open(fname) as f:
        if "Success!" in f.read():
            break
    time.sleep(1)
print "Stage two finished at %s" % cur()

while not os.path.exists("/tmp/pwned"):
    time.sleep(1)

print "Exploit took %s" % cur()
