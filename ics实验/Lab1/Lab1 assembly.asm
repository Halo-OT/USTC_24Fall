AND R0, R0, x0 ; 初始化 R0
AND R2, R2, x0 ; 初始化 R2
ADD R2, R2, xf ; 学号23151782 -> 11111100 -> xfc
ADD R2, R2, R2 ; 
ADD R2, R2, R2 ;
ADD R2, R2, R2 ;
ADD R2, R2, R2 ;
ADD R2, R2, xc ; secret 存储到R2

; A XOR B = (A AND NOT B) OR (NOT A AND B)

; 此处 R0 = A， R2 = B

NOT R1, R2          ; R1 <- NOT R2
AND R1, R2, R0      ; R1 = R0 AND NOT R2

NOT R0, R0          ; R0 <- NOT R0
AND R2, R0, R2      ; R2 <- NOT R0 AND R2

ADD R3, R1, R2      ; R3 <- R0 AND NOT R2 + NOT R0 AND R2

