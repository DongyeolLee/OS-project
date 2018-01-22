#include "Utility.h"

/**
 *  메모리를 특정 값으로 채움
 */
void kMemSet( void* pvDestination, BYTE bData, int iSize )
{
    int i;

    // 8 바이트씩 먼저 채움
    for( i = 0 ; i < iSize ; i++ )
    {
        ( ( char* ) pvDestination )[ i ] = bData;
    }

}

int kMemCpy( void* pvDestination, const void* pvSource, int iSize )
{
    int i;

    for( i = 0 ; i < iSize ; i++ )
    {
        ( ( char* ) pvDestination )[ i ] = ( ( char* ) pvSource )[ i ];
    }

    return iSize;
}

int kMemCmp( const void* pvDestination, const void* pvSource, int iSize )
{
    int i;
    char cTemp;

    for( i = 0 ; i < iSize ; i++ )
    {
        cTemp = ( ( char* ) pvDestination )[ i ] - ( ( char* ) pvSource )[ i ];
        if( cTemp != 0 )
        {
            return ( int ) cTemp;
        }
    }
    return 0;
}
