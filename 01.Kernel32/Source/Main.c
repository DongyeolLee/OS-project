#include "Types.h"
#include "Page.h"
#include "ModeSwitch.h"

// �Լ� ����
void kPrintString( int iX, int iY, const char* pcString );
BOOL kIntializeKernel64Area( void );
BOOL kIsMemoryEnough( void );
void kCopyKernel64ImageTo2Mbyte( void );

/**
 *  �Ʒ� �Լ��� C ��� Ŀ���� ���� �κ���
 *      �ݵ�� �ٸ� �Լ��� ���� ���� ���ʿ� �����ؾ� ��
 */
void Main( void )
{
	DWORD i;
	DWORD dwEAX, dwEBX, dwECX, dwEDX;
	char vcVendorString[13] = {0, };

	kPrintString(0, 3, "Protected Mode C Language Kernel Start -----[PASS]");

	// �ּ� �޸� ũ�⸦ Ȯ��
	kPrintString(0, 4, "Minimum Memory Size Check ------------------[    ]");
	if( kIsMemoryEnough() == FALSE )
	{
		kPrintString(45, 4, "FAIL");
		kPrintString(0, 5, "Not Enough Memory~!! OS requires Over 64Mbyte Memory.");

		while(1);
	}
	else
	{
		kPrintString(45, 4, "PASS");
	}

	// IA-32e ����� Ŀ�� ������ �ʱ�ȭ
	kPrintString(0, 5, "IA-32e Kernel Area Initialize --------------[    ]");
	if(kIntializeKernel64Area() == FALSE)
	{
		kPrintString(0, 5, "FAIL");
		kPrintString(0, 6, "Kernel Area Initialization Fail");
		while(1);
	}

	kPrintString(45, 5, "PASS");

	// IA-32e ��� Ŀ���� ���� ������ ���̺� ����
	kPrintString(0, 6, "IA-32e Page Tables Initialize --------------[    ]");
	kInitializePageTables();
	kPrintString(45, 6, "PASS");

    // ���μ��� ������ ���� �б�
    kReadCPUID( 0x00, &dwEAX, &dwEBX, &dwECX, &dwEDX );
    *( DWORD* ) vcVendorString = dwEBX;
    *( ( DWORD* ) vcVendorString + 1 ) = dwEDX;
    *( ( DWORD* ) vcVendorString + 2 ) = dwECX;
    kPrintString( 0, 7, "Processor Vendor String --------------------[            ]" );
    kPrintString( 45, 7, vcVendorString );

    // 64��Ʈ ���� ���� Ȯ��
    kReadCPUID( 0x80000001, &dwEAX, &dwEBX, &dwECX, &dwEDX );
    kPrintString( 0, 8, "64bit Mode Support Check -------------------[    ]" );
    if( dwEDX & ( 1 << 29 ) )
    {
        kPrintString( 45, 8, "PASS" );
    }
    else
    {
        kPrintString( 45, 8, "FAIL" );
        kPrintString( 0, 9, "This processor does not support 64bit mode~!!" );
        while( 1 ) ;
    }

    // IA-32e ��� Ŀ���� 0x200000(2Mbyte) ��巹���� �̵�
    kPrintString( 0, 9, "Copy IA-32e Kernel To 2M Address -----------[    ]" );
    kCopyKernel64ImageTo2Mbyte();
    kPrintString( 45, 9, "PASS" );

    // IA-32e ���� ��ȯ
    kPrintString( 0, 10, "Switch To IA-32e Mode" );
    kSwitchAndExecute64bitKernel();

    while( 1 ) ;
}

/**
 *  ���ڿ��� X, Y ��ġ�� ���
 */
void kPrintString( int iX, int iY, const char* pcString )
{
    CHARACTER* pstScreen = ( CHARACTER* ) 0xB8000;
    int i;

    // X, Y ��ǥ�� �̿��ؼ� ���ڿ��� ����� ��巹���� ���
    pstScreen += ( iY * 80 ) + iX;

    // NULL�� ���� ������ ���ڿ� ���
    for( i = 0 ; pcString[ i ] != 0 ; i++ )
    {
        pstScreen[ i ].bCharactor = pcString[ i ];
    }
}

// IA-32e mode Kernel initialize to 0
BOOL kIntializeKernel64Area()
{
	DWORD* pdwCurrentAddress;

	// �ʱ�ȭ�� ������ ��巹�� 0x100000(1MB)�� ����
	pdwCurrentAddress = (DWORD*) 0x100000;

	// ������ ��巹���� 0x600000���� ������ ���鼭 4����Ʈ�� 0���� ä��
	while( ( DWORD ) pdwCurrentAddress < 0x600000 )
	{
		*pdwCurrentAddress = 0x00;

		// 0���� ������ �� �ٽ� �о��� �� 0�� ������ ������ �ش� ��巹����
		// ����ϴµ� ������ ��������� ����
		if ( *pdwCurrentAddress != 0)
		{
			return FALSE;
		}

		// ���� �ּҷ� �̵�
		pdwCurrentAddress++;
	}

	return TRUE;
}

// OS�� �����ϱ⿡ ����� �޸𸮸� ������ �ִ��� Ȯ��
BOOL kIsMemoryEnough(void)
{
	DWORD* pwdCurrentAddress;

	pwdCurrentAddress = (DWORD*) 0x100000;

	while((DWORD) pwdCurrentAddress < 0x4000000)
	{
		*pwdCurrentAddress = 0x12345678;

		if(*pwdCurrentAddress != 0x12345678)
		{
			return FALSE;
		}

		// 1MB�� �̵��ϸ鼭 Ȯ��
		pwdCurrentAddress += (0x100000 / 4);
	}

	return TRUE;
}

/**
 *  IA-32e ��� Ŀ���� 0x200000(2Mbyte) ��巹���� ����
 */
void kCopyKernel64ImageTo2Mbyte( void )
{
    WORD wKernel32SectorCount, wTotalKernelSectorCount;
    DWORD* pdwSourceAddress,* pdwDestinationAddress;
    int i;

    // 0x7C05�� �� Ŀ�� ���� ��, 0x7C07�� ��ȣ ��� Ŀ�� ���� ���� ��� ����
    wTotalKernelSectorCount = *( ( WORD* ) 0x7C05 );
    wKernel32SectorCount = *( ( WORD* ) 0x7C07 );

    pdwSourceAddress = ( DWORD* ) ( 0x10000 + ( wKernel32SectorCount * 512 ) );
    pdwDestinationAddress = ( DWORD* ) 0x200000;
    // IA-32e ��� Ŀ�� ���� ũ�⸸ŭ ����
    for( i = 0 ; i < 512 * ( wTotalKernelSectorCount - wKernel32SectorCount ) / 4;
        i++ )
    {
        *pdwDestinationAddress = *pdwSourceAddress;
        pdwDestinationAddress++;
        pdwSourceAddress++;
    }
}

