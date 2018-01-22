#include "Descriptor.h"
#include "Utility.h"

//==============================================================================
//  GDT �� TSS
//==============================================================================

/**
 *  GDT ���̺��� �ʱ�ȭ
 */
void kInitializeGDTTableAndTSS( void )
{
    GDTR* pstGDTR;
    GDTENTRY8* pstEntry;
    TSSSEGMENT* pstTSS;
    int i;

    // GDTR ����
    pstGDTR = ( GDTR* ) GDTR_STARTADDRESS;
    pstEntry = ( GDTENTRY8* ) ( GDTR_STARTADDRESS + sizeof( GDTR ) );
    pstGDTR->wLimit = GDT_TABLESIZE - 1;
    pstGDTR->qwBaseAddress = ( QWORD ) pstEntry;

    // TSS ���׸�Ʈ ���� ����, GDT ���̺��� ���ʿ� ��ġ
    pstTSS = ( TSSSEGMENT* ) ( ( QWORD ) pstEntry + GDT_TABLESIZE );

   // NULL, Ŀ�� �ڵ�/������, ���� �ڵ�/������, TSS�� ���� �� 5 + 16���� ���׸�Ʈ�� ����
    kSetGDTEntry8( &( pstEntry[ 0 ] ), 0, 0, 0, 0, 0 );
    // Ŀ�� ���� �ڵ�/������ ��ũ���� ����
    kSetGDTEntry8( &( pstEntry[ 1 ] ), 0, 0xFFFFF, GDT_FLAGS_UPPER_CODE,
            GDT_FLAGS_LOWER_KERNELCODE, GDT_TYPE_CODE );
    kSetGDTEntry8( &( pstEntry[ 2 ] ), 0, 0xFFFFF, GDT_FLAGS_UPPER_DATA,
            GDT_FLAGS_LOWER_KERNELDATA, GDT_TYPE_DATA );
    kSetGDTEntry16((GDTENTRY16*) &(pstEntry[3]), (QWORD) pstTSS,
    		sizeof(TSSSEGMENT) - 1, GDT_FLAGS_UPPER_TSS, GDT_FLAGS_LOWER_TSS, GDT_TYPE_TSS);

    // TSS �ʱ�ȭ, GDT ���� ������ �����
    kInitializeTSSSegment( pstTSS );
}

/**
 *  8����Ʈ ũ���� GDT ��Ʈ���� ���� ����
 *      �ڵ�� ������ ���׸�Ʈ ��ũ���͸� �����ϴµ� ���
 */
void kSetGDTEntry8( GDTENTRY8* pstEntry, DWORD dwBaseAddress, DWORD dwLimit,
        BYTE bUpperFlags, BYTE bLowerFlags, BYTE bType )
{
    pstEntry->wLowerLimit = dwLimit & 0xFFFF;
    pstEntry->wLowerBaseAddress = dwBaseAddress & 0xFFFF;
    pstEntry->bUpperBaseAddress1 = ( dwBaseAddress >> 16 ) & 0xFF;
    pstEntry->bTypeAndLowerFlag = bLowerFlags | bType;
    pstEntry->bUpperLimitAndUpperFlag = ( ( dwLimit >> 16 ) & 0xFF ) |
        bUpperFlags;
    pstEntry->bUpperBaseAddress2 = ( dwBaseAddress >> 24 ) & 0xFF;
}

/**
 *  16����Ʈ ũ���� GDT ��Ʈ���� ���� ����
 *      TSS ���׸�Ʈ ��ũ���͸� �����ϴµ� ���
 */
void kSetGDTEntry16( GDTENTRY16* pstEntry, QWORD qwBaseAddress, DWORD dwLimit,
        BYTE bUpperFlags, BYTE bLowerFlags, BYTE bType )
{
    pstEntry->wLowerLimit = dwLimit & 0xFFFF;
    pstEntry->wLowerBaseAddress = qwBaseAddress & 0xFFFF;
    pstEntry->bMiddleBaseAddress1 = ( qwBaseAddress >> 16 ) & 0xFF;
    pstEntry->bTypeAndLowerFlag = bLowerFlags | bType;
    pstEntry->bUpperLimitAndUpperFlag = ( ( dwLimit >> 16 ) & 0xFF ) |
        bUpperFlags;
    pstEntry->bMiddleBaseAddress2 = ( qwBaseAddress >> 24 ) & 0xFF;
    pstEntry->dwUpperBaseAddress = qwBaseAddress >> 32;
    pstEntry->dwReserved = 0;
}

/**
 *  TSS ���׸�Ʈ�� ������ �ʱ�ȭ
 */
void kInitializeTSSSegment( TSSSEGMENT* pstTSS )
{

	// 0���� �ʱ�ȭ
	kMemSet( pstTSS, 0, sizeof( TSSSEGMENT ) );

	pstTSS->qwIST[0] = IST_STARTADDRESS + IST_SIZE;

	// IO Map�� ���� �ּҸ� TSS ��ũ������ Limit �ʵ庸�� ũ�� ���������ν�
	// IO Map�� ������� �ʵ��� ��
	pstTSS->wIOMapBaseAddress = 0xFFFF;

}

// �ӽ� ���� �Ǵ� ���ͷ�Ʈ �ڵ鷯
void kDummyHandler(void) {
	kPrintString(0, 0, "========================================================");
	kPrintString(0, 1, "             Dummy Interrupt Handler Excute!!!!!!!!     ");
	kPrintString(0, 2, "             Interrupt or Exception Occur ~!!!!!!!!     ");
	kPrintString(0, 3, "========================================================");
}

//==============================================================================
//  IDT
//==============================================================================
/**
 *  IDT ���̺��� �ʱ�ȭ
 */
void kInitializeIDTTables( void )
{
    IDTR* pstIDTR;
    IDTENTRY* pstEntry;
    int i;

    // IDTR�� ���� ��巹��
    pstIDTR = ( IDTR* ) IDTR_STARTADDRESS;
    // IDT ���̺��� ���� ����
    pstEntry = ( IDTENTRY* ) ( IDTR_STARTADDRESS + sizeof( IDTR ) );
    pstIDTR->qwBaseAddress = ( QWORD ) pstEntry;
    pstIDTR->wLimit = IDT_TABLESIZE - 1;

    for(i = 0; i < IDT_MAXENTRYCOUNT; i ++ ) {
    	kSetIDTEntry(&(pstEntry[i]), kDummyHandler, 0x08, IDT_FLAGS_IST1,
    			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
    }
}

/**
 *  IDT ����Ʈ ��ũ���Ϳ� ���� ����
 */
void kSetIDTEntry( IDTENTRY* pstEntry, void* pvHandler, WORD wSelector,
        BYTE bIST, BYTE bFlags, BYTE bType )
{
    pstEntry->wLowerBaseAddress = ( QWORD ) pvHandler & 0xFFFF;
    pstEntry->wSegmentSelector = wSelector;
    pstEntry->bIST = bIST & 0x3;
    pstEntry->bTypeAndFlags = bType | bFlags;
    pstEntry->wMiddleBaseAddress = ( ( QWORD ) pvHandler >> 16 ) & 0xFFFF;
    pstEntry->dwUpperBaseAddress = ( QWORD ) pvHandler >> 32;
    pstEntry->dwReserved = 0;
}

