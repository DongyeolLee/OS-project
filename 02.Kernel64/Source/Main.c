#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"
#include "PIC.h"
#include "Console.h"
#include "ConsoleShell.h"
#include "Task.h"
#include "PIT.h"

/**
 *      아래 함수는 C 언어 커널의 시작 부분임
 */
void Main( void )
{
    int iCursorX, iCursorY;

    // console init
    kInitializeConsole( 0, 10 );
    kPrintf( "Switch To IA-32e Mode Success~!!\n" );
    kPrintf( "IA-32e C Language Kernel Start -------------[PASS]\n" );
    kPrintf( "Initialize Console -------------------------[PASS]\n" );


    kGetCursor( &iCursorX, &iCursorY );
    kPrintf( "GDT Initialize And Switch For IA-32e Mode --[    ]" );
    kInitializeGDTTableAndTSS();
    kLoadGDTR( GDTR_STARTADDRESS );
    kSetCursor( 45, iCursorY++ );
    kPrintf( "PASS\n" );

    kPrintf( "TSS Segment Load ---------------------------[    ]" );
    kLoadTR( GDT_TSSSEGMENT );
    kSetCursor( 45, iCursorY++ );
    kPrintf( "PASS\n" );

    kPrintf( "IDT Initialize -----------------------------[    ]" );
    kInitializeIDTTables();
    kLoadIDTR( IDTR_STARTADDRESS );
    kSetCursor( 45, iCursorY++ );
    kPrintf( "PASS\n" );

    kPrintf( "Total RAM Size Check -----------------------[    ]" );
    kCheckTotalRAMSize();
    kSetCursor( 45, iCursorY++ );
    kPrintf( "PASS], Size = %d MB\n", kGetTotalRAMSize() );

    kPrintf( "TCB Pool And Scheduler Initialize ----------[PASS]\n");
    iCursorY++;
    kInitializeScheduler();
    // 1ms당 한 번씩 인터럽트가 발생하도록 설정
    kInitializePIT( MSTOCOUNT( 1 ), 1 );

    kPrintf( "Keyboard Activate And Queue Initialize -----[    ]" );
    if( kInitializeKeyboard() == TRUE )
    {
        kSetCursor( 45, iCursorY++ );
        kPrintf( "PASS\n" );
        kChangeKeyboardLED( FALSE, FALSE, FALSE );
    }
    else
    {
        kSetCursor( 45, iCursorY++ );
        kPrintf( "FAIL\n" );
        while( 1 ) ;
    }

    kPrintf( "PIC Controller And Interrupt Initialize ----[    ]" );

    kInitializePIC();
    kMaskPICInterrupt( 0 );
    kEnableInterrupt();
    kSetCursor( 45, iCursorY++ );
    kPrintf( "PASS\n" );

    kStartConsoleShell();
}
