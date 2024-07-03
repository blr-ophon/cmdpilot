#include "decode.h"

/*
 * Currently used for debugging
 */
void Decode_Packet(uint8_t *PKTbuf, bool recv){
    TLPacketHeader_t *header = (TLPacketHeader_t*)PKTbuf;
    
    char *recv_str = "\n<< ";
    char *sent_str = "\n>> ";

    switch(header->Any.type){
        case PKTTYPE_BEACON:               
            printf("%s BEACON\n", recv? recv_str : sent_str);
            BEACON_t BeaconPKT = header->Beacon;
            printf("Version: %d\nRXS Max: %d\nTXS Max: %d\nTXA Max: %d\n\n",
                    BeaconPKT.version, BeaconPKT.RXS_Max, BeaconPKT.TXS_Max,
                    BeaconPKT.TXA_Max);
            break;
        case PKTTYPE_PING:
            printf("%s PING\n", recv? recv_str : sent_str);
            PING_t PingPKT = header->Ping;
            printf("C: %d\nN: %d\nLLID: %d\nNumber: %d\n\n",
                    PingPKT.C, PingPKT.N, PingPKT.LLID, PingPKT.Number);
            break;
        case PKTTYPE_ERROR:      
            printf("%s ERROR\n", recv? recv_str : sent_str);
            ERROR_t ErrorPKT = header->Error;
            printf("Error Code: %d\n\n", ErrorPKT.ErrorCode);
            break;
        case PKTTYPE_REQUEST_ASYNC:
            printf("%s ASYNC\n", recv? recv_str : sent_str);
            ASYNC_t ReqAsyncPKT = header->Async;
            printf("Payload Length: %d\n\n", ReqAsyncPKT.payloadLength);
            break;
        case PKTTYPE_RESPONSE:
            printf("%s RESPONSE\n", recv? recv_str : sent_str);
            RESPONSE_t ResponsePKT = header->Response;
            printf("Payload Length: %d\n\n", ResponsePKT.payloadLength);
            break;
        default:
            printf("%s UNKNOWN\n", recv? recv_str : sent_str);
            break;
    }
}
