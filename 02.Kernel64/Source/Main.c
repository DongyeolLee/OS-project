#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"
#include "PIC.h"

void kPrintString( int iX, int iY, const char* pcString );

/**
 *      아래 함수는 C 언어 커널의 시작 부분임
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

	// 키보드 활성화
	if(kInitializeKeyboard() == TRUE) {
		kPrintString(45, 15, "PASS");
		kChangeKeyboardLED(FALSE, FALSE, FALSE);
	}
	else {
		kPrintString(45, 15, "FAIL");
		while(1);
	}

	kPrintString(0, 16, "PIC Controller And Interrupt Initialize ----[    ]");
	// PIC 컨트롤러 초기화 및 모든 인터럽트 활성화
	kInitializePIC();
	kMaskPICInterrupt( 0 );
	kEnableInterrupt();
	kPrintString(45, 16, "PASS");

	while(1) {

		// 키 큐에 데이터가 있으면 키를 처리
		if( kGetKeyFromKeyQueue( &stData ) == TRUE )
		{
			// 키가 눌러졌으면 키의 ASCII 코드 값을 화면에 출력
			if( stData.bFlags & KEY_FLAGS_DOWN )
			{
				// 키 데이터의 ASCII 코드 값을 저장
				vcTemp[ 0 ] = stData.bASCIICode;
				kPrintString( i++, 17, vcTemp );

				// 0이 입력되면 변수를 0으로 나누어 예외를 발생시킴
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

    // X, Y 좌표를 이용해서 문자열을 출력할 어드레스를 계산
    pstScreen += ( iY * 80 ) + iX;

    // NULL이 나올 때까지 문자열 출력
    for( i = 0 ; pcString[ i ] != 0 ; i++ )
    {
        pstScreen[ i ].bCharactor = pcString[ i ];
    }
}
