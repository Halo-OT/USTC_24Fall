.ORIG x3000
; 初始化寄存器和变量
LDI R1, INPUT         ; 月数存储到 R1
LD R2, INITIAL_SAV   ; 初始存款存储到 R2
LD R3, INCOME        ; 初始收入存储到 R3
LD R4, EXPENSE       ; 初始支出存储到 R4
LD R6, STACK

; 调用递归函数计算最终存款
JSR CALCULATE_SAVINGS

; 将结果存储到 RESULT
HALT

; 递归函数 CALCULATE_SAVINGS
CALCULATE_SAVINGS
    ; 保存当前状态到堆栈
    STR R7, R6, #0      ;保存R7的值，以便后续函数一层一层返回时可以从栈中取出
    ADD R6, R6, #-1   ; 堆栈指针向下移动
    STR R1, R6, #0    ; 保存 R1（当前月数）
    ADD R6, R6, #-1
    STR R2, R6, #0    ; 保存 R2（当前存款）
    ADD R6, R6, #-1
    STR R3, R6, #0    ; 保存 R3（当前收入）
    ADD R6, R6, #-1
    STR R4, R6, #0    ; 保存 R4（当前支出）
    
    ; 如果月份为 0，则返回当前存款
    ADD R1, R1, #0
    BRz DONE          ; 若 R1 == 0，则跳转到 DONE
    
    ; 更新当前存款
    ADD R2, R2, R3    ; 存款加上收入
    NOT R5, R4        ; R5 = -支出
    ADD R5, R5, #1
    ADD R2, R2, R5    ; 存款减去支出

    ; 检查本月支出是否超过收入
    NOT R5, R3        ; R5 = -收入
    ADD R5, R5, #1
    ADD R5, R5, R4    ; R5 = 支出 - 收入
    
    BRzp RESET         ; 若支出 >= 收入，重置
    ADD R4, R4, R4
    ADD R4, R4, R4    ; 下个月支出翻四倍
    BRnzp CONTINUE
RESET
    LD R4, RESET_EXP  ; 下个月支出重置支出为 2

CONTINUE
    ; 更新下个月的收入
    ADD R3, R3, R3    ; 收入翻倍
    ADD R1, R1, #-1   ; 月份减 1
    ; 递归调用下一月
    JSR CALCULATE_SAVINGS

RESTORE
    ; 恢复堆栈中的状态
    LDR R4, R6, #0    ; 恢复支出
    ADD R6, R6, #1
    LDR R3, R6, #0    ; 恢复收入
    ADD R6, R6, #1
    LDR R2, R6, #0    ; 恢复存款
    ADD R6, R6, #1
    LDR R1, R6, #0    ; 恢复月数
    ADD R6, R6, #1
    LDR R7, R6, #0
    RET

DONE
    STI R2, RESULT      ; 将 R2（最终存款）存储到 RESULT
    BRnzp RESTORE


; 数据段
INITIAL_SAV .FILL #10 ; 初始存款为 10
INCOME      .FILL #6  ; 初始收入为 6
EXPENSE     .FILL #2  ; 初始支出为 2
RESET_EXP   .FILL #2  ; 支出重置为 2
INPUT       .FILL x3100 ; 输入的月份存储地址
RESULT      .FILL x3200 ; 最终结果存储地址
STACK       .FILL x6000

.END


