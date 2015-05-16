#!/usr/bin/env python
# encoding: utf-8
#
# For offline testing using test.c and a downloaded hosts file
# (This checks if all hosts are properly resolved)
#
import os
import multiprocessing

def action(data):
    ip, host = data
    if not os.popen("./a.out %s" % host).read().strip():
        print host, "not found"

if __name__ == '__main__':
    pool = multiprocessing.Pool(7)
    tests = filter(lambda data: len(data) == 2, [ x.strip().split() for x in open("hosts").readlines() if x.strip()[0] != "#" ])
    pool.map(action, tests)
