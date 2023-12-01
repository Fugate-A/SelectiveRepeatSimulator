
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_SEQ_NUM 256

void send_packet(int packet_no) {
    printf("Sending packet %d\n", packet_no);
}

bool is_packet_lost() {
    // Simulate packet loss with a 10% chance
    return (rand() % 10) == 0;
}

int main() {
    srand(time(NULL)); // Seed the random number generator

    int window_size, total_packets, base = 0, next_seq_num = 0;
    bool acked[MAX_SEQ_NUM] = {false}; // Acknowledgement status of packets

    printf("Enter the window size: ");
    scanf("%d", &window_size);

    printf("Enter the total number of packets to send: ");
    scanf("%d", &total_packets);

    while(base < total_packets) {
        while(next_seq_num < base + window_size && next_seq_num < total_packets) {
            if(!is_packet_lost()) {
                send_packet(next_seq_num);
                acked[next_seq_num] = true;
            } else {
                printf("Packet %d lost\n", next_seq_num);
            }
            next_seq_num++;
        }

        // Simulate receiving ACK for the base packet
        printf("Received ACK for packet %d\n", base);
        base++;

        // For simplicity, we immediately move the window after receiving an ACK
        // A real implementation would wait for a timeout or a certain number of ACKs
    }

    printf("All packets have been sent successfully.\n");

    return 0;
}
