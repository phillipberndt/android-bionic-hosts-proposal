#!/usr/bin/env python
# encoding: utf-8
import os
from pylab import *

for fn in os.listdir("."):
    if "-" in fn and ".png" not in fn:
        data = map(float, open(fn).readlines())
        clf()
        hist(data, bins=30)
        xlabel("Response time [s]")
        ylabel("Amount")
        title("Time to lookup misses in /etc/hosts for %s" % fn)
        savefig("%s.png" % fn)
