import time
import erpc

if __name__ == '__main__':
    print("Starting")

    xport = erpc.transport.TCPTransport('localhost', 5407, False)

    client = erpc.client.ClientManager(xport, erpc.basic_codec.BasicCodec)

    print("Sleeping for 5 seconds")
    time.sleep(5)

    print("Done")
