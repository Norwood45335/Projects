                    .syntax     unified
                    .cpu        cortex-m4
                    .text
                    .thumb_func
                    .align       2

                    .global     CallReturnOverhead
CallReturnOverhead: BX          LR

// uint32_t ReverseBits(uint32_t word)
                    // R0 = word;
                    .global     ReverseBits
ReverseBits:        .rept       32
                    LSLS        R0,R0,1         // Left shift most significant bit into carry
                    RRX         R1,R1           // push that significant bit into the right side of new register
                    .endr

                    MOV         R0,R1           // move result back into R0
                    BX          LR

                    .global     ReverseBytes
ReverseBytes:       BFI         R1,R0,24,8      // Replace bits 24-31 of R1 with bits 0-8 of R0
                    LSR         R0,R0,8         // Left shift R0 8 times to grab a new set of 8 bits

                    BFI         R1,R0,16,8      // Replace bits 16-23 of R1 with bits 0-8 of R0
                    LSR         R0,R0,8         // Left shift R0 8 times to grab a new set of 8 bits

                    BFI         R1,R0,8,8       // Replace bits 8-15 of R1 with bits 0-8 of R0
                    LSR         R0,R0,8         // Left shift R0 8 times to grab a new set of 8 bits

                    BFI         R1,R0,0,8       // Replace bits 0-8 of R1 with bits 0-8 of R0
                    MOV         R0,R1           // move result back into R0

                    BX          LR
                    .end

