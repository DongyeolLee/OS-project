#include "Types.h"
#include "Page.h"

// 함수 선언
void kPrintString( int iX, int iY, const char* pcString );
BOOL kIntializeKernel64Area( void );
BOOL kIsMemoryEnough( void );

/**
 *  아래 함수는 C 언어 커널의 시작 부분임
 *      반드시 다른 함수들 보다 가장 앞쪽에 존재해야 함
 */
void Main( void )
{
	DWORD i;

	kPrintString(0, 3, "C Language Kernel Start --------------------[PASS]");

	// 최소 메모리 크기를 확인
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

	// IA-32e 모드의 커널 영역을 초기화
	kPrintString(0, 5, "IA-32e Kernel Area Initialize --------------[    ]");
	if(kIntializeKernel64Area() == FALSE)
	{
		kPrintString(0, 5, "FAIL");
		kPrintString(0, 6, "Kernel Area Initialization Fail");
		while(1);
	}

	kPrintString(45, 5, "PASS");

	// IA-32e 모드 커널을 위한 페이지 테이블 생성
	kPrintString(0, 6, "IA-32e Page Tables Initialize --------------[    ]");
	kInitializePageTables();
	kPrintString(45, 6, "PASS");

    while( 1 ) ;
}

/**
 *  문자열을 X, Y 위치에 출력
 */
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

// IA-32e mode Kernel initialize to 0
BOOL kIntializeKernel64Area()
{
	DWORD* pdwCurrentAddress;

	// 초기화을 시작할 어드레스 0x100000(1MB)을 설정
	pdwCurrentAddress = (DWORD*) 0x100000;

	// 마지막 어드레스인 0x600000까지 루프를 돌면서 4바이트씩 0으로 채움
	while( ( DWORD ) pdwCurrentAddress < 0x600000 )
	{
		*pdwCurrentAddress = 0x00;

		// 0으로 저장한 후 다시 읽었을 때 0이 나오지 않으면 해당 어드레스를
		// 사용하는데 문제가 생긴것으로 종료
		if ( *pdwCurrentAddress != 0)
		{
			return FALSE;
		}

		// 다음 주소로 이동
		pdwCurrentAddress++;
	}

	return TRUE;
}

// OS를 실행하기에 충분한 메모리를 가지고 있는지 확인
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

		// 1MB씩 이동하면서 확인
		pwdCurrentAddress += (0x100000 / 4);
	}

	return TRUE;
}
