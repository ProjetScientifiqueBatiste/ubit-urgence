/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

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

