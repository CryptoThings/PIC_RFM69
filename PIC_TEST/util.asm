
#include <aspic.h>

;; Function exernal callers should use.
GLOBAL  _ShiftBuffer

;; Variables for this assembly code to use.
GLOBAL  _dataPacket

;; Tell the linker how it should be called.
SIGNAT _ShiftBuffer,4217

PSECT text,class=CODE,local,delta=2
_ShiftBuffer:
    BANKSEL (_dataPacket)
    bcf		CARRY
    rlf		_dataPacket+20,f
    rlf		_dataPacket+19,f
    rlf		_dataPacket+18,f
    rlf		_dataPacket+17,f
    rlf		_dataPacket+16,f
    rlf		_dataPacket+15,f
    rlf		_dataPacket+14,f
    rlf		_dataPacket+13,f
    rlf		_dataPacket+12,f
    rlf		_dataPacket+11,f
    rlf		_dataPacket+10,f
    rlf		_dataPacket+9,f
    rlf		_dataPacket+8,f
    rlf		_dataPacket+7,f
    rlf		_dataPacket+6,f
    rlf		_dataPacket+5,f
    rlf		_dataPacket+4,f
    rlf		_dataPacket+3,f
    rlf		_dataPacket+2,f
    rlf		_dataPacket+1,f
    rlf		_dataPacket+0,f
    RETLW   0

    END