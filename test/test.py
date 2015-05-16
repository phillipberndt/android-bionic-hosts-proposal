#!/usr/bin/env python
# encoding: utf-8
#
# For on-device testing
#
import socket
import timeit

times = []

def lookup(host, ip):
    cname, _, ips = socket.gethostbyaddr(host)
    if ips != [ ip ]:
        raise RuntimeError("Lookup for %s gave %s" % (host, ips))

n = 0
with open("times", "w") as out:
    for data in ( x.strip().split() for x in open("/etc/hosts").readlines() if x.strip()[0] != "#" ):
        n += 1
        if n % 100 == 0:
            print n
            out.flush()
        if len(data) != 2: continue
        ip, host = data
        print >> out, host, timeit.timeit(lambda: lookup(host, ip), number=1)
