.ORIG x3000
; 初始化部分
LEA R0, PROMPT            ; 加载提示信息的地址
PUTS                      ; 显示提示信息

; 输入循环
LD R2, BUF_PTR_INIT       ; 初始化输入缓冲区指针
LD R3, LOWER              ; R3 = -48, 让输入的 ASCII 码变成 对应数字 
LD R4, Y                  ; R4 检测 y
INPUT_LOOP:
    GETC                  ; 从键盘获取一个字符
    ADD R1, R0, R3        ; 将字符复制到 R1， 并转换为 数字
    STR R1, R2, #0        ; 将数字存储到缓冲区
    ADD R2, R2, #1        ; 增加缓冲区指针
    PUTC
    ADD R1, R1, R4        ; 检查字符是否为 'y' (ASCII 89)
    BRz INPUT_END         ; 如果是 'y'，结束输入
    BR INPUT_LOOP         ; 否则继续输入

INPUT_END
; 有限状态机部分，用于统计 "1010" 的出现次数
LD R2, BUF_PTR_INIT       ; 重置缓冲区指针
AND R3, R3, #0           ; 清空状态寄存器
AND R5, R5, #0           ; 清空计数器

FSM_LOOP:
    LDR R0, R2, #0       ; 从缓冲区加载下一个字符
    ADD R2, R2, #1       ; 移动到下一个字符
    ADD R1, R0, R4
    BRz FSM_END          ; 如果已经遍历完成，结束有限状态机

STATE_0    
    ; 状态转换
    ; 状态 0：初始状态，寻找 '1'
    ADD R1, R3, #0       ; COPY R3 到 R1
    ADD R1, R1, #0       ; 比较当前状态是否为 0
    BRnp STATE_1         ; 如果不是状态 0，跳过
    ADD R3, R0, #0       ; 如果输入是 '1'，移动到状态 1
    BR FSM_LOOP          ; 继续
    
STATE_1:
    ADD R1, R3, #0         ; 将当前状态（R3）复制到 R1
    ADD R1, R1, #-1        ; 比较当前状态是否为 1（R3 - 1）
    BRnp STATE_2            ; 如果当前状态是 1，跳转到 STATE_2

    ADD R0, R0, #0         ; 读取输入字符
    BRz UPDATE_STATE_2     ; 如果输入为 0，更新状态为 2
    ; 如果输入不是 0，继续保持在 STATE_1
    BR FSM_LOOP            ; 返回到 FSM 主循环

STATE_2:
    ADD R1, R3, #0
    ADD R1, R1, #-2      ; 比较当前状态是否为 2
    BRnp STATE_3         ; 如果不是状态 2，跳过
    
    ADD R0, R0, #0         ; 读取输入字符
    BRz UPDATE_STATE_0     ; 如果输入为 0，更新状态为 0
    BR UPDATE_STATE_3      ; 如果输入是 1，更新为状态 3


STATE_3:
    ADD R1, R3, #0
    ADD R1, R1, #-3      ; 比较当前状态是否为 3
    BRnp FSM_LOOP        ; 如果不是状态 3，继续

    ADD R0, R0, #0         ; 读取输入字符
    BRz COUNTER_UP          ; 如果输入为 0，更新状态为 2, count++
    BR UPDATE_STATE_1      ; 如果输入是 1，更新为状态 1
    
UPDATE_STATE_0:
    AND R3, R3, #0
    BR FSM_LOOP            ; 继续返回到 FSM 主循环

UPDATE_STATE_1:
    AND R3, R3, #0
    ADD R3, R3, #1         ; 更新状态为 1（即 R3 = 1）
    BR FSM_LOOP            ; 继续返回到 FSM 主循环
 
UPDATE_STATE_2:
    AND R3, R3, #0
    ADD R3, R3, #2         ; 更新状态为 2（即 R3 = 2）
    BR FSM_LOOP            ; 继续返回到 FSM 主循环
    
UPDATE_STATE_3:
    AND R3, R3, #0
    ADD R3, R3, #3         ; 更新状态为 3（即 R3 = 3）
    BR FSM_LOOP            ; 继续返回到 FSM 主循环
    
COUNTER_UP
    AND R3, R3, #0
    ADD R3, R3, #2        ; 更新状态为 2（即 R3 = 2）
    ADD R5, R5, #1       ; 计数器加 1
    BR FSM_LOOP          ; 返回有限状态机循环

FSM_END:
    LD R4, UPPER
    ADD R5, R4, R5
    STI R5, COUNT        ; 存储计数器

; 显示结果
LD R0, NEWLINE
PUTC
LEA R0, RESULT_MSG       ; 加载结果消息
PUTS
LDI R0, COUNT             ; 加载计数
OUT                      ; 将计数显示为字符
LEA R0, RESULT_COUNT     ; 加载结果后缀
PUTS
HALT

UPPER        .FILL #48
LOWER        .FILL #-48
Y            .FILL #-73  ; y (ASCII 121) = 48 + 73
BUF_PTR_INIT .FILL x6000 ; 输入缓冲区起始地址
COUNT .FILL x5FFF             ; "1010" 序列的计数器
; 提示信息
PROMPT .STRINGZ "SD is ready! Please input your number:\n"
RESULT_MSG .STRINGZ "There are(is) "
RESULT_COUNT .STRINGZ "  1010 in the sequence!\n"
NEWLINE .FILL x0A  ; 换行符的 ASCII 值

.END