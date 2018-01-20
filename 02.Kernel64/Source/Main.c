#include "Types.h"
#include "Keyboard.h"

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

	kPrintString(0, 10, "Switch To IA-32e Mode Success~!!");
	kPrintString(0, 11, "IA-32e C Language Kernel Start -------------[PASS]");
	kPrintString(0, 12, "Keyboard Activate --------------------------[    ]");

	// 키보드 활성화
	if(kActivateKeyboard() == TRUE) {
		kPrintString(45, 12, "PASS");
		kChangeKeyboardLED(FALSE, FALSE, FALSE);
	}
	else {
		kPrintString(45, 12, "FAIL");
		while(1);
	}

	while(1) {
		// 출력 버퍼(포트 0x60)가 차 있으면 스캔 코드를 읽을 수 있음
		if(kIsOutputBufferFull() == TRUE) {
			// 출력 버퍼에서 스캔 코드를 읽어서 저장
			bTemp = kGetKeyboardScanCode();

			// 스캔 코드를 아스키 코드로 변환하는 함수를 호출하여 아스키 코드와
			// 눌림 또는 떨어짐 정보를 반환
			if(kConvertScanCodeToASCIICode(bTemp, &(vcTemp[0]), &bFlags) == TRUE) {

				if(bFlags & KEY_FLAGS_DOWN) {
					kPrintString(i++, 13, vcTemp);
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
