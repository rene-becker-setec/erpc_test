import time
import erpc
import logging
import random
import example.example_pc2emu as pc2emu
import example.example_emu2pc as emu2pc

logging.basicConfig(
    format='%(asctime)s.%(msecs)03d [%(levelname)8s] %(message)s (%(filename)s:%(lineno)s)',
    level=logging.INFO,
    handlers=[
        # logging.FileHandler(f'./{datetime.datetime.now().strftime("%Y-%m-%d_%H.%M.%S")}.log'),
        logging.StreamHandler()
    ]
)

LOGGER = logging.getLogger(__name__)

if __name__ == '__main__':
    LOGGER.info("Starting")

    xport = erpc.transport.TCPTransport('localhost', 5407, False)
    arbitrator = erpc.arbitrator.TransportArbitrator(
        sharedTransport=xport, codec=erpc.basic_codec.BasicCodec()
    )

    ############################################################################
    #
    # Server for Emulator -> PC Communication
    #
    ############################################################################

    class Emu2PcHandler(emu2pc.interface.IIoExpanderEmulatorAsync):

        def canMsgRcvd(self, canMsg):
            LOGGER.info(f'CAN Message received {canMsg}')

    handler = Emu2PcHandler()
    service = emu2pc.server.IoExpanderEmulatorAsyncService(handler)
    server = erpc.simple_server.ServerThread(arbitrator, erpc.basic_codec.BasicCodec)
    server.add_service(service)

    LOGGER.info('Starting Server ...')
    server.start()

    ############################################################################
    #
    # Client for PC -> Emulator Communication
    #
    ############################################################################

    LOGGER.info('Starting up client ...')

    client_mngr = erpc.client.ClientManager(xport, erpc.basic_codec.BasicCodec)
    client_mngr.arbitrator = arbitrator

    client = pc2emu.client.IoExpanderEmulatorClient(client_mngr)
    response = client.sendCanMsg(b'hello world')

    LOGGER.info(f'server responded with {response}')

    for i in range(10):
        sleep_duration = random.randint(1, 50)/10
        LOGGER.info(f'Sleeping for {sleep_duration} seconds')
        time.sleep(sleep_duration)
        response = client.sendCanMsg(f'Injecting {i}'.encode())
        LOGGER.info(f'server responded with {response}')

    LOGGER.info("Done")

