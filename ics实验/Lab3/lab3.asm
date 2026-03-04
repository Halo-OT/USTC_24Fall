.ORIG x3000
        LDI R0, LENGTH      ; R0 = N (字符串长度)
        BRz DONE1
        LD R1, STRING      ; R1 = 起始指针
        ADD R2, R1, R0     ; R2 = R1 + N (字符串末尾之后的位置)
        ADD R2, R2, #-1    ; R2 = 字符串末尾指针

        AND R3, R3, #0     ; 清零 R3 (用于存储比较结果)

LOOP    LDR R4, R1, #0     ; R4 = M[R1] (当前字符)
        LDR R5, R2, #0     ; R5 = M[R2] (对应末尾字符)
        
        NOT R5, R5         ; R5 = ~M[R2]
        ADD R5, R5, #1     ; R5 = -M[R2]
        ADD R4, R4, R5     ; 比较字符是否相等，R4 = M[R1] - M[R2]

        BRz CONTINUE       ; 如果相等，继续
        AND R3, R3, #0     ; 如果不相等，将 R3 置为 0，表示不是回文
        BRnzp DONE2         ; 跳转到结束

CONTINUE ADD R1, R1, #1     ; R1++
        ADD R2, R2, #-1    ; R2--
        
        NOT R5, R2
        ADD R5, R5, #1      ; R5 = -R2
        ADD R6, R1, R5      ; R6 = R1 - R2 <= 0
        
        BRn LOOP           ; 如果 R1 <= R2，继续循环

DONE1   LD R7, RESULT
        ADD R3, R3, #1     ; 假设字符串是回文，R3 = 1 (FLAG)
        STR R3, R7, #0
        

DONE2   LD R7, RESULT      ; 加载结果地址
        STR R3, R7, #0     ; 将 R3 的值存入结果地址 (1: 是回文，0: 不是回文)

        TRAP x25           ; 结束程序

LENGTH  .FILL x3100        
STRING  .FILL x3101        ; 假设字符串存储在地址 x3101 开始
RESULT  .FILL x3200        ; 结果存储地址
.END


.ORIG x3100
.FILL #4            ; Fill in x3100 with 5
.STRINGZ "aEfa"    ; Fill in "abcba" starting from x3101
.END