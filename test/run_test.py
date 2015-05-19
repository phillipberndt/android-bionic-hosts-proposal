#!/usr/bin/env python
# encoding: utf-8
#
# For offline testing using test.c and a downloaded hosts file
# (This checks if all hosts are properly resolved)
#
import multiprocessing
import subprocess
import signal

class Timeout(Exception):
    pass

def raise_timeout(*_):
    raise Timeout()

def action(data):
    ip, host = data
    signal.signal(signal.SIGALRM, raise_timeout)
    signal.alarm(5)
    try:
        proc = subprocess.Popen(["./a.out", host], stdout=subprocess.PIPE)
        output = proc.stdout.read()
    except Timeout:
        print "\033[31mTimeout while waiting for %s\033[0m" % host
        return
    finally:
        proc.kill()
        signal.alarm(0)
    if ip == "-fail-":
        if output:
            print "Host %s found, this is an error!!" % (host)
            print output
    else:
        if not output:
            print host, "not found"

if __name__ == '__main__':
    pool = multiprocessing.Pool(7)
    tests = filter(lambda data: len(data) == 2, [ x.strip().split() for x in open("hosts").readlines() if x.strip() and x.strip()[0] != "#" ])
    for i, j in zip(tests[:-1], tests[1:]):
        i, j = i[1].lower(), j[1].lower()
        for n in range(len(i)):
            if j[n] != i[n]:
                break
        else:
            continue
        b = i[:n] + "-fiaidqwoodqwo"
        tests.append(("-fail-", b))

    pool.map(action, tests)
