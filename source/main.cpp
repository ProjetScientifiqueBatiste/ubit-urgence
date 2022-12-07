#include "MicroBit.h"

MicroBit uBit;
 MicroBitSerial serial(USBTX, USBRX); 

// Fonction déclenchée par la réception d'un message RF reçu - Vérifie que la clé est valide avant de relayé au serveur (via la fonction sendData)
void recDataRF(MicroBitEvent)
{
    ManagedString recData;
    recData = uBit.radio.datagram.recv(); // Réception du message RF dans un PacketBuffer
    serial.send(recData.toCharArray());
}

void recDataSerial(MicroBitEvent)
{
    uBit.display.scroll("rec !");
    uint8_t requestsmartphone[3];
    serial.read(requestsmartphone,3*sizeof(uint8_t));
    PacketBuffer sendRequest(4);
    sendRequest[0] = 'O';
    sendRequest[1] = 'K';
    sendRequest[2] = '\n';
    sendRequest[3] = '\r';
    serial.send(sendRequest);
}


int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    


    serial.baud(115200);
    
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, recDataRF);

    uBit.radio.enable();
    uBit.radio.setGroup(5); // Définit le groupe RF

    

    // Insert your code here!
    uBit.display.scroll("HELLO WORLD! :)",50);
    while (1)
    {
       serial.send("OK MOI ");
       serial.send("OK TOI ");
       uBit.sleep(10000);
    }

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}

