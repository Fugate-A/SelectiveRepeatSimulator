
//-----------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

//-----------------------------------------------------------------------------------------------

#define PacketLimit 1024 //arbitrary for defining sizes within code

//-----------------------------------------------------------------------------------------------
/*
    Global variables
*/
int windowSize = 0;
int nop = 0;
int windowStartingNumIndex = 0;
int nextNumIndex = 0;

int sequence[ PacketLimit ];

bool responses[ PacketLimit ];

//-----------------------------------------------------------------------------------------------
/*
    Random function to simulate the timeout of the protocol. Since this is not a time
    based code this will act as if the time has ran out and the packet needs to be 
    retransmitted. For the main report this value will be locked at a 12.5% chance.

    This function accepts no input and is only called to return true or false (bool)
    to say whether or not that packet was lost. The function is seeded in main using
    the time of the "host" computer and will vary each and every compile. The function 
    uses modulo to determine the percent chance and can be thought of better as:
        100 / x = % of loss
            
            x = 20 = 5% chance of loss
            x = 5 = 20% chance of loss
            x = 8 = 12.5% chance of loss
            etc etc
    
    Note that this is a random function and as randomness cannot garuntee anything, it is
    possible for the packets to never be lost or always be lost; and this code is NOT optimized
    for those edge cases. Though EXTREMELY unlikely, randomness could produce these results.
*/
bool percentDrop()
{
    return( rand() % 8 ) == 0;
}

//-----------------------------------------------------------------------------------------------
/*
    Function used to send and ackknowledge the packets. Accepts the packet int from main and updates
    the respones array but does not have an explicit return type...is of type void.
    
    Proccess the int by calling the percentDrop function to decide if it was recieved or not. 
*/
void send( int passed )
{

    printf("Packet %d sent.\t", passed);

    if( !percentDrop() )
    {
        //printf("Packet %d acknowledged.\n", passed);
        printf("\n");

        responses[ passed ] = true;
    }
    
    else
    {
        printf("Packet %d lost.\n", passed);

        responses[ passed ] = false;
    }
}

//-----------------------------------------------------------------------------------------------
/*
    Main function, accepts standard inputs for window size, number of packets, and the value for
    each of these packets. Drives the simulator by doing most of the printing, maintains while
    conditions to ensure all packets are eventually acknowledged, and slides the window as needed. 
    Also seeds randomness value as mentioned above in that function's description comment. 
    Returns type int though is the standard main return type.
*/
int main()
{
    srand( time(NULL) );

    printf("\nEnter the window size: ");
    scanf("%d", &windowSize);

    printf("Enter the total number of packets to send: ");
    scanf("%d", &nop);

    printf("Enter the sequence numbers of the packets to be sent: ");
    
    for( int i = 0; i < nop; i++ )
    {
        scanf("%d", &sequence[ i ] );

        responses[ sequence[ i ] ] = false;
    }

    printf("\n");

    while( windowStartingNumIndex < nop )
    {
        while( nextNumIndex < windowStartingNumIndex + windowSize && nextNumIndex < nop )
        {
            send( sequence[ nextNumIndex ] );
            
            nextNumIndex++;
        }

        while( windowStartingNumIndex < nop && responses[ sequence[ windowStartingNumIndex ] ] )
        {
            printf("Sliding window: base packet %d acknowledged.\n", sequence[ windowStartingNumIndex ] );

            windowStartingNumIndex++;
        }

        for( int i = windowStartingNumIndex; i < nextNumIndex; i++ )
        {
            if( !responses[ sequence[ i ] ] )
            {
                printf("\nResending lost packet %d.\n", sequence[ i ] );

                send( sequence[ i ] );
            }
        }

        printf("\nWindow: [");
        
        for( int i = windowStartingNumIndex; i < windowStartingNumIndex + windowSize && i < nop; i++ )
        {
            printf(" %d", sequence[ i ] );
        }

        printf(" ]\n\n");
    }

    printf("All packets have been sent and acknowledged.\n");

    return 0;
}

//-----------------------------------------------------------------------------------------------
/*
    Selective Repeat Simulator
    Computer Communication Networks
    EEL 4781 - Project 2 - Fall 2023
    Proffessor: Saleem Sahawneh 
    University of Central Florida

    Written by: Andrew Fugate

    Last updated: 12/1/2023
*/
//-----------------------------------------------------------------------------------------------
