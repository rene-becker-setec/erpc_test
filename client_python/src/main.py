import time
import erpc
import example.example

if __name__ == '__main__':
    print("Starting")

    xport = erpc.transport.TCPTransport('localhost', 5407, False)

    client_mngr = erpc.client.ClientManager(xport, erpc.basic_codec.BasicCodec)

    client = example.example.client.DEMOClient(client_mngr)
    response = client.RD_demoHello(b'hello world')

    print(f'server responded with {response}')

    print("Sleeping for 5 seconds")
    time.sleep(5)

    print("Done")
