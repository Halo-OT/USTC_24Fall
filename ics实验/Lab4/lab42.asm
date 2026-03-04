.ORIG x3000

; Load the input value (number of months) into R1
LD R1, INPUT

; Initialize constants
AND R2, R2, #0          ; R2 = 0 (savings accumulator)
ADD R2, R2, #10         ; Initial savings = 10
AND R3, R3, #0          ; R3 = 0 (income accumulator)
ADD R3, R3, #6          ; Initial income = 6
AND R4, R4, #0          ; R4 = 0 (expense accumulator)
ADD R4, R4, #2          ; Initial expense = 2

; Recursive function entry point
JSR CALC_SAVINGS

; Store the result in memory location x3200
ST R2, RESULT

; Halt the program
HALT

; Recursive function to calculate savings
CALC_SAVINGS
    ; Base case: If R1 (months) == 0, return
    ADD R5, R1, #0      ; Copy R1 to R5 for comparison
    BRz DONE            ; If R5 is zero, we are done

    ; Recursive case: Decrement months and call recursively
    ADD R1, R1, #-1     ; R1 = R1 - 1
    JSR CALC_SAVINGS    ; Recursive call

    ; Update income and expenses
    ADD R3, R3, R3      ; Income doubles
    ADD R6, R4, R4      ; Temp = Expense * 2
    ADD R4, R6, R6      ; Expense quadruples

    ; Check if expenses >= income
    NOT R6, R3          ; R6 = NOT(income)
    ADD R6, R6, #1      ; R6 = -income
    ADD R6, R6, R3      ; R6 = expense - income
    BRnz RESET_EXPENSE   ; If R6 < 0, skip reset
    ADD R2, R2, R3
    ADD R2, R2, R6
    

    ; Reset expenses to 2
RESET_EXPENSE
    AND R4, R4, #0
    ADD R4, R4, #2

    ; Update savings
    ADD R2, R2, R3      ; Add income to savings
    ADD R2, R2, R4      ; Subtract expenses from savings

DONE
    RET

; Memory allocation
INPUT   .FILL #1        ; Number of months (example: 5)
RESULT  .BLKW 1         ; Memory location to store result

.END
