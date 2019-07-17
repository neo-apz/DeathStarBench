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

def main():
  iteration = 1
  while iteration > 0:
    socket = TSocket.TSocket("localhost", 9090)
    transport = TTransport.TFramedTransport(socket)
    protocol = TCompactProtocol.TCompactProtocol(transport)
    client = MyUrlShortenService.Client(protocol)

    transport.open()
    req_id = uuid.uuid4().int & (1 << 32)

    urls = ["https://url_0.com", "https://url_1.com", "https://url_2.com"]

    result = client.UploadUrls(req_id, urls)
    # print(result)
    transport.close()
    iteration -= 1

if __name__ == '__main__':
  try:
    main()
  except Thrift.TException as tx:
    print('%s' % tx.message)