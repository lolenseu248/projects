uint8_t *data;  
size_t dataSize = 0; 
uint16_t packetnum = 1; 
uint16_t packetcount = 1;
uint16_t reponse = 0;

// Structure to hold the packet information
typedef struct receive_message {
    uint16_t datalen;  
    uint16_t packetnum;  
    uint8_t packet[128]; 
} receive_message;

receive_message rcvxMsg;

// generate a response if still 0 the sender resend the packet
typedef struct send_message {
    uint16_t reponse = 0;
send_message sndxMsg

void processPacket() {

    while (!dataComplete) {
        if(packetnum=0){
            dataComplete = true;
        }

        else if(packetnum=packetcount){
            memcpy(&data[dataLen], rcvxMsg.packet, sizeof(rcvxMsg.packet)); // update this line make make sure reads the content of rcvxmsg.packet 
            packetcount+=1;
        }
    }


    sndxMsg.reponse=1;
}


void setup() {
    Serial.begin(115200);
}

void loop() {
    processPacket();

    if(Serial.avialbeForwite()>0){
        Serial.wite(data,rcvxMsg.datalen)
    }
}