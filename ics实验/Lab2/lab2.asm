        .ORIG x3000          ; 程序从地址 x3000 开始
        LD  R6, My_Val       ; 从 My_Val中读取数据，存入到R6中
        STI R6, NUM          ; 将 R6 数据存储到 NUM 的地址中
START   LDI R0, NUM           ; 从内存 x3100 中加载初始值 a0 到寄存器 R0
        BRz END
        AND R2, R2, #0       ; 清空 R2，R2 用来计数 k 的值（步数）
        

LOOP    ADD R2, R2, #1       ; 每次循环增加步数计数器 k
        AND R3, R0, #1       ; 检查当前数字是偶数还是奇数
        BRz EVEN             ; 如果 R3 是 0，则数字为偶数，跳转到 EVEN

        ; 奇数情况：计算 3 * a0 + 1
        AND R4, R4, #0
        ADD R4, R0, R0       ; R4 寄存 2 * a0
        ADD R0, R4, R0       ; 更新 R0 变为 3 * a0
        ADD R0, R0, #1       ; 加 1，得到 3 * a0 + 1
        BRnzp CHECK1          ; 跳转到检查 a0 是否等于 1

EVEN    ; 偶数情况：a0 = a0 / 2
        AND R4, R4, #0       ; R4  初始化为 0
        
UP      ADD R4, R4, #1       ; R4 自增
        BRnzp CHECK2
    
UPDATE  ADD R0, R4, #0       ; 更新 R0 的值

CHECK1   ; 检查当前 a0 是否为 1
        ADD R3, R0, #-1      ; 将当前值减去 1
        BRz STORE_RESULT     ; 如果 R3 结果为 0，跳转到存储结果
        BRnzp LOOP           ; 否则继续循环

CHECK2  ; 检查 R4 是否为满足目标的 R2/2
        AND R5, R5, #0
        ADD R5, R4, #0
        ADD R5, R5, R5      ; R5 = 2* R5
        NOT R5, R5          ; R5 = NOT R5
        ADD R5, R5, #1      ; R5 = R5 + 1（取反加一）; 
        ADD R5, R0, R5      ; R0 和 2* R4 相减法， 结果存储在 R5 
        BRnp UP             ; 如果结果 非0， R4 继续自增
        BRz UPDATE          ;
        

STORE_RESULT
        STI R2, K_COUNT       ; 将步数计数器 k 存储到内存 x3101 中

END     TRAP x25             ; 程序结束

NUM     .FILL x3100          ; a0 的初始值存储在内存地址 x3100
K_COUNT .FILL x3101          ; k 的结果将存储在内存地址 x3101
My_Val  .FILL #7
        .END                 ; 程序结束
