#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define PCAP_MAGIC_NUMBER 0xA1B2C3D4

typedef struct __attribute__((packed)) {
  uint32_t magicNumber;
  uint16_t majorVersion;
  uint16_t minorVersion;
  uint32_t timezoneOffset;    // always 0
  uint32_t timezoneAccuracy;  // always 0
  uint32_t snapshotLength;
  uint32_t linklayerHeaderType;
} PCAP_Header;

typedef struct __attribute__((packed)) {
  uint32_t timestampSeconds;
  uint32_t timestampMicroNano;
  uint32_t packetDataLength;
  uint32_t packetDataUntruncatedLength;
} Packet_Header;

typedef struct __attribute__((packed)) {
  uint8_t versionIhl;     // Version(4 bits) + IHL(4 bits)
  uint8_t dscpEcn;        // DSCP(6 bits) + ECN(2 bits)
  uint16_t totalLength;
  uint16_t identification;
  uint16_t flagsFragment;  // Flags(3bits) + Fragment Offset(13bits)
  uint8_t ttl;
  uint8_t protocol;
  uint16_t headerChecksum;
  uint32_t sourceAddr;
  uint32_t destinationAddr;
} IPv4_Header;

typedef struct __attribute__((packed)) {
  uint16_t sourcePort;
  uint16_t destinationPort;
  uint32_t sequenceNumber;
  uint32_t ackNumber;
  uint8_t dataOffset; // Data Offset(4bits) + Reserved(4bits)
  uint8_t flags;
  uint16_t window;
  uint16_t checksum;
  uint16_t urgentPointer;
} TCP_Header;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: %s PCAPFILE\n", *argv);
    return EXIT_FAILURE;
  }

  FILE *fp = fopen(*++argv, "r");
  if (fp == nullptr) {
    perror(*argv);
    return EXIT_FAILURE;
  }
  
  PCAP_Header pcap_header;
  if (fread(&pcap_header, sizeof(PCAP_Header), 1, fp) != 1) {
    fclose(fp);
    return EXIT_FAILURE;
  }

  if (pcap_header.linklayerHeaderType != 0x0) {
    printf("This program only supports 'loopback captures' \
        BSD loopback encapsulation)\n");
    fclose(fp);
    return EXIT_FAILURE;
  }

  if (pcap_header.magicNumber != PCAP_MAGIC_NUMBER) {
    printf("This program only supports little endian PCAP files\n");
    fclose(fp);
    return EXIT_FAILURE;
  }

  /* save the position of the end of the file in eof */
  if (fseek(fp, 0, SEEK_END) != 0) {
    perror("fseek() failed");
    fclose(fp);
    return EXIT_FAILURE;
  }
  int eof = ftell(fp);
  if (eof == -1) {
    perror("ftell() failed");
    fclose(fp);
    return EXIT_FAILURE;
  }

  if (fseek(fp, sizeof(PCAP_Header), SEEK_SET) != 0) {
    perror("fseek() failed");
    fclose(fp);
    return EXIT_FAILURE;
  } 

  int acc = 0;
  int outbound_syn_counter = 0;
  int outbound_synack_counter = 0;

  while (ftell(fp) < eof) {
    Packet_Header packeth;
    assert(fread(&packeth, sizeof(Packet_Header), 1, fp) == 1);

    uint32_t dataLength = packeth.packetDataLength;

    /* Save this file position to help jump to the next packet */
    int saved_file_position = ftell(fp);

    uint32_t link_data_type;
    assert(fread(&link_data_type, sizeof(uint32_t), 1, fp) == 1);

    IPv4_Header ipv4h;
    assert(fread(&ipv4h, sizeof(IPv4_Header), 1, fp) == 1);

    uint8_t ihl = ipv4h.versionIhl & 0x0F;

    /* jump over the IP 'Options' field */
    if (ihl > 5) {
      uint8_t ipv4_header_length = ihl * 4;
      uint8_t offset = ipv4_header_length - sizeof(IPv4_Header);
      assert(fseek(fp, offset, SEEK_CUR) == 0);
    }

    TCP_Header tcph;
    assert(fread(&tcph, sizeof(TCP_Header), 1, fp) == 1);

    /* Network headers use big endian 
       0x5000 corresponds to 0x0050 (80 in base 10) in little endian */
    if (tcph.destinationPort == 0x5000 && tcph.flags == 0x02) // SYN
      ++outbound_syn_counter;

    if (tcph.sourcePort == 0x5000 && tcph.flags == 0x12) // SYN ACK
      ++outbound_synack_counter;

    /* Jump to the next packet */
    assert(fseek(fp, saved_file_position + dataLength, SEEK_SET) == 0);

    ++acc;
  }

  printf("Total packet captured: %d\n", acc);
  printf("Total inbound SYN: %d\n", outbound_syn_counter);
  printf("Total outbound SYN/ACK: %d\n", outbound_synack_counter);
  printf("Percentage of incoming SYNC that were ACKed: %0.2f%%\n", (double)outbound_synack_counter/outbound_syn_counter * 100);

  fclose(fp);
  return EXIT_SUCCESS;
}
