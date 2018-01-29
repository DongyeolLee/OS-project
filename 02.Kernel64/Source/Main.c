#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"
#include "PIC.h"

void kPrintString( int iX, int iY, const char* pcString );

/**
 *      �Ʒ� �Լ��� C ��� Ŀ���� ���� �κ���
 */
void Main( void )
{
	char vcTemp[2] = {0, };
	BYTE bFlags;
	BYTE bTemp;
	int i = 0;
	KEYDATA stData;

	kPrintString(0, 10, "Switch To IA-32e Mode Success~!!");
	kPrintString(0, 11, "IA-32e C Language Kernel Start -------------[PASS]");
	kPrintString(0, 12, "GDT Initialize And Switch For IA-32e Mode---[    ]" );
	kInitializeGDTTableAndTSS();
	kLoadGDTR(GDTR_STARTADDRESS);
	kPrintString(45, 12, "PASS");

	kPrintString(0, 13, "TSS Segment Load ---------------------------[    ]" );
	kLoadTR(GDT_TSSSEGMENT);
	kPrintString(45, 13, "PASS");

	kPrintString(0, 14, "IDT Initialize -----------------------------[    ]" );
	kInitializeIDTTables();
	kLoadIDTR(IDTR_STARTADDRESS);
	kPrintString(45, 14, "PASS");

	kPrintString(0, 15, "Keyboard Activate And Queue Initialize -----[    ]");

	// Ű���� Ȱ��ȭ
	if(kInitializeKeyboard() == TRUE) {
		kPrintString(45, 15, "PASS");
		kChangeKeyboardLED(FALSE, FALSE, FALSE);
	}
	else {
		kPrintString(45, 15, "FAIL");
		while(1);
	}

	kPrintString(0, 16, "PIC Controller And Interrupt Initialize ----[    ]");
	// PIC ��Ʈ�ѷ� �ʱ�ȭ �� ��� ���ͷ�Ʈ Ȱ��ȭ
	kInitializePIC();
	kMaskPICInterrupt( 0 );
	kEnableInterrupt();
	kPrintString(45, 16, "PASS");

	while(1) {

		// Ű ť�� �����Ͱ� ������ Ű�� ó��
		if( kGetKeyFromKeyQueue( &stData ) == TRUE )
		{
			// Ű�� ���������� Ű�� ASCII �ڵ� ���� ȭ�鿡 ���
			if( stData.bFlags & KEY_FLAGS_DOWN )
			{
				// Ű �������� ASCII �ڵ� ���� ����
				vcTemp[ 0 ] = stData.bASCIICode;
				kPrintString( i++, 17, vcTemp );

				// 0�� �ԷµǸ� ������ 0���� ������ ���ܸ� �߻���Ŵ
				if( vcTemp[ 0 ] == '0' )
				{
					bTemp = bTemp / 0;
				}
			}
		}
	}
}

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
