#include "Types.h"
#include "Page.h"

// �Լ� ����
void kPrintString( int iX, int iY, const char* pcString );
BOOL kIntializeKernel64Area( void );
BOOL kIsMemoryEnough( void );

/**
 *  �Ʒ� �Լ��� C ��� Ŀ���� ���� �κ���
 *      �ݵ�� �ٸ� �Լ��� ���� ���� ���ʿ� �����ؾ� ��
 */
void Main( void )
{
	DWORD i;

	kPrintString(0, 3, "C Language Kernel Start --------------------[PASS]");

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
