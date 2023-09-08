#ifndef __SERVER_H__
#define __SERVER_H__

int ServerInitialise (int nPort, char * const szIPAddress);
int ServerListen (void *pData, int nDataSize, int nTimeout);
int ServerMessage (void *pData, int nDataSize, int nTimeout);
int ServerStop ();

#endif