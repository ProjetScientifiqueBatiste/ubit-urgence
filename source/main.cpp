#include "MicroBit.h"

#define TAILLE_REQUETE 4

MicroBit uBit;
//MicroBitSerial serial(USBTX, USBRX); 

// Fonction déclenchée par lManagedString rec = uBit.serial.readUntil(ManagedString("\n"), ASYNC);a réception d'un message RF reçu - Vérifie que la clé est valide avant de relayé au serveur (via la fonction sendData)


void sendDataSerial(PacketBuffer message)
{
    uBit.serial.send(message);
}

void recDataRF(MicroBitEvent)
{
    PacketBuffer b(TAILLE_REQUETE);
    b = uBit.radio.datagram.recv(); // Réception du message RF dans un PacketBuffer

    PacketBuffer bBis(32);
    bBis[0] = 'T';
    bBis[1] = 'T';
    bBis[2] = 'B';
    bBis[3] = 'M';
    bBis[4] = 0;
    bBis[5] = 1;
    bBis[6] = 0;
    bBis[7] = 2;
    bBis[8] = 3;
    bBis[9] = 200;

    int tab[] = {0x45,0xd3,0x6c,0x2f,0x26,0xab,0x4e,0x1c,0xc4,0xe6,0x33,0xb3,0x38,0x20,0x01,0x4e};
    for (int i = 15; i < 31; i++)
    {
        //char c = int('A')  + (i-15);
        bBis[i] = tab[i-15];
    }

    bBis[31] = 10;


    sendDataSerial (bBis);
}

void sendDataRF(PacketBuffer message)
{
    uBit.radio.datagram.send(message);
}

void recDataSerial(MicroBitEvent)
{
    
    ManagedString rec = uBit.serial.readUntil(ManagedString("\n"), ASYNC);
    uBit.display.scroll(rec,50);
    //uBit.serial.send(rec);

    PacketBuffer b(TAILLE_REQUETE);
    for ( int i = 0; i < TAILLE_REQUETE-1; i++)
    {
        b[i] = rec.toCharArray()[i];
    }
    b[TAILLE_REQUETE-1] = '\n';
    //sendDataSerial(b);
    sendDataRF(b);
}



int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    
    uBit.serial.baud(115200);
    uBit.messageBus.listen(MICROBIT_ID_SERIAL, MICROBIT_SERIAL_EVT_DELIM_MATCH, recDataSerial);
    uBit.serial.eventOn(ManagedString("\n"), ASYNC);
    uBit.serial.read(ASYNC);
    
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, recDataRF);
    uBit.radio.enable();
    uBit.radio.setGroup(5); // Définit le groupe RF

    // Insert your code here!
    uBit.display.scroll("HELLO WORLD! :)",30);
    while (1)
    {
       uBit.sleep(10000);
    }

    release_fiber();
}

