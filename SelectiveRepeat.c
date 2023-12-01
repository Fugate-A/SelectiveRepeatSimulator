
//-----------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

//-----------------------------------------------------------------------------------------------

#define PacketLimit 1024 //arbitrary for defining sizes within code

//-----------------------------------------------------------------------------------------------

int windowSize;
int nop;
int windowStartingNumIndex = 0;
int nextNumIndex = 0;
int sequence[ PacketLimit ];

bool responses[ PacketLimit ];

//-----------------------------------------------------------------------------------------------

//Random packet drop -> 100 / x = % of loss -> so x = 20 = 5% ... x = 5 = 20% ... etc. 
bool percentDrop()
{
    return( rand() % 8 ) == 0;
}

//-----------------------------------------------------------------------------------------------

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
