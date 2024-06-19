uint8_t *data;  // Dynamic data buffer
size_t dataSize = 0;  // Actual size of the data to be sent
uint16_t packetnum = 1;  // Packet number

// Structure to hold the packet information
typedef struct send_message {
    uint16_t datalen;  // Total size of the data to be sent
    uint16_t packetnum;  // Packet number
    uint8_t packet[128];  // Packet data (adjust size as needed)
} send_message;

send_message sndxMsg;

void processPackets() {
    size_t offset = 0;
    size_t dataLen = dataSize;  // Total size of the data to be sent

    while (offset < dataSize) {
        size_t sizeforpacket = min(sizeof(sndxMsg.packet), dataSize - offset);  // Size for the current packet
        memcpy(sndxMsg.packet, &data[offset], sizeforpacket);  // Copy data into packet buffer

        sndxMsg.datalen = dataLen;  // Total length of the data array
        sndxMsg.packetnum = packetnum;  // Current packet number

        // Here you would send the sndxMsg (e.g., via Serial or other communication method)
        // For example: Serial.write((uint8_t*)&sndxMsg, sizeof(sndxMsg));

        packetnum++;
        offset += sizeforpacket;
    }

    // Send the final packet with packet number 0 to indicate end of transmission
    sndxMsg.datalen = dataLen;
    sndxMsg.packetnum = 0;
    memset(sndxMsg.packet, 0, sizeof(sndxMsg.packet));  // Clear packet buffer

    // Send the final packet
    // For example: Serial.write((uint8_t*)&sndxMsg, sizeof(sndxMsg));

    // Reset dataSize to 0 after processing all packets
    dataSize = 0;
}

void setup() {
    Serial.begin(115200);

    // Example: Allocate dynamic memory for data
    size_t maxDataSize = 3000;  // Example maximum data size
    data = (uint8_t *)malloc(maxDataSize);
    if (data == NULL) {
        Serial.println("Failed to allocate memory");
        return;
    }

    // Example: Accumulate data from serial
    while (Serial.available() > 0 && dataSize < maxDataSize) {
        data[dataSize++] = Serial.read();
    }

    // Process packets after accumulating data
    processPackets();

    // Free the allocated memory after processing packets
    free(data);
}

void loop() {
    // Empty loop, all processing is done in setup for this example
}
