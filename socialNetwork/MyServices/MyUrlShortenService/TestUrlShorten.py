import sys
sys.path.append('../gen-py')

import uuid
from social_network import MyUrlShortenService
from social_network.ttypes import Url

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.protocol import TCompactProtocol

import multiprocessing
import os


def main():
  iteration = 50000

  socket = TSocket.TSocket("localhost", 9090)
  transport = TTransport.TFramedTransport(socket)
  protocol = TCompactProtocol.TCompactProtocol(transport)
  client = MyUrlShortenService.Client(protocol)
  transport.open()

  print(multiprocessing.current_process().name, "connected!")

  while iteration > 0:
  # while True:
    # socket = TSocket.TSocket("localhost", 9090)
    # transport = TTransport.TFramedTransport(socket)
    # protocol = TCompactProtocol.TCompactProtocol(transport)
    # client = MyUrlShortenService.Client(protocol)

    # transport.open()
    req_id = uuid.uuid4().int & (1 << 32)

    urls = ["https://url_0.com", "https://url_1.com", "https://url_2.com"]

    result = client.UploadUrls(req_id, urls)
    # print(result)
    # transport.close()
    iteration -= 1

  transport.close()
  print(multiprocessing.current_process().name, "stopped!")

if __name__ == '__main__':
  try:
    jobs = []
    n_processes = 15
    for i in range(n_processes):
      p = multiprocessing.Process(target=main)
      jobs.append(p)      
      p.start()
      os.system("taskset -p -c %d %d" % ((i % os.cpu_count()), p.pid))
      # main()

    for p in jobs:
      p.join()

  except Thrift.TException as tx:
    print('%s' % tx.message)