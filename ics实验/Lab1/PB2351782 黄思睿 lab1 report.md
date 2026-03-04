### Name & Date： 

​	黄思睿 PB2351782  2024/10/24

### Purpose of the experiment

​	利用由学生学号产生的密钥，用 LC-3 实现 XOR 运算 解密存储在 R0 中的秘文。

### The core algorithm

	
	0011000000000000  
	
	; 初始化 R2 为 x7C :学号23151782 -> 0111 1100
	AND R2, R2, #0      ; 清空 R2
	ADD R2, R2, #15     ; R2 = 15 (xF)
	ADD R2, R2, R2      ; 左移 1 位 
	ADD R2, R2, R2      ; 左移 1 位 
	ADD R2, R2, R2      ; 左移 1 位
	ADD R2, R2, #4     ; R2 = 124 (x7C)
	
	; A XOR B = (A AND NOT B) OR (NOT A AND B)；
	NOT R1, R0 ; R1 = NOT R0
	NOT R3, R2 ; R3 = NOT R2
	AND R4, R0, R3 ; R4 = R0 AND R3
	AND R5, R1, R2 ; R5 = R1 AND R2
	NOT R4, R4 ; R4 = NOT R4
	NOT R5, R5 ; R5 = NOT R5
	AND R6, R4, R5 ; R6 = R4 AND R5
	NOT R3, R6 ; R3 = NOT R6
	
	1111000000100101    ; HALT                ; 停止程序


### Critical code to implement the algorithm

```
0011000000000000  

; 免去初始化 R0
; 初始化 R2 为 x7C :学号23151782 -> 0111 1100
; 存入 124 (0111 1100)
0001 010 010 1 01111   ; ADD R2, R2, #15     ; R2 = 15 (xF)
0001 010 010 000 010   ; ADD R2, R2, R2      ; R2 = 30 (0x1E)
0001 010 010 000 010   ; ADD R2, R2, R2      ; R2 = 60 (0x3C)
0001 010 010 000 010   ; ADD R2, R2, R2      ; R2 = 120 (0x78)
0001 010 010 1 00100   ; ADD R2, R2, #4      ; R2 = 124 (0x7C)

; A XOR B = (A AND NOT B) OR (NOT A AND B)
1001 001 000 111111   ; R1 = NOT R0
1001 011 010 111111   ; R3 = NOT R2
0101 100 000 000 011   ; R4 = R0 AND R3
0101 101 001 000 010   ; R5 = R1 AND R2
1001 100 100 111111   ; R4 = NOT R4
1001 101 101 111111   ; R5 = NOT R5
0101 110 100 000 101   ; R6 = R4 AND R5
1001 011 110 111111   ; R3 = NOT R6

1111000000100101  ; 0x3013:  HALT                ; 停止程序

```

 

------



### Run results

> #### **我的学号下的实验结果**
>
> > 我的学号(23151782 - > 01111100)下 R0 和 R2 输出测试
>
> > ![image-20241027221350327](/Users/halo/Library/Application Support/typora-user-images/image-20241027221350327.png)
>





> #### **测试样例下的实验结果**
>
> > 测试样例下 R0 和 R2 的输出测试
>
> > ![image-20241020143134935](/Users/halo/Library/Application Support/typora-user-images/image-20241020143134935.png)
>
> 
>
> > 测试样例下 解密结果 R3 = x0068 和预期一致
>
> > ![image-20241020143311426](/Users/halo/Library/Application Support/typora-user-images/image-20241020143311426.png)