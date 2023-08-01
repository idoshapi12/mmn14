;errors check - first pass phase

;label errors
myLabel: .string "This should be fine"
6Label: .string "This shouldn't"
thisIsLongerThanAllowed : .data 12, 4, 56, 78, 10
myLabel: .string "This label was already used on first line"

;comma errors
myArray: .data 12,6, -9, 10,
    myArray2: .data 12,6,-9,10,,
    myArray3: .data ,12,6, -9, , 10
myArray4: .data 12,     ,    6,-9,10
myArray5: .data 12, 6, -9, 0, -10       ,
    mov, r3, K
add , L3  L3
add , L3,  L3
inc , r1
inc, r1


;line length errors
K1: .data 1200, 1234, 54,90,-23         ,       42224,          3466,   +554,  -7,  12,75553, 763, 345
K2: .string "I am a very long string that will surely exceed the maximum length of a line"

;data errors
    .data
    .data lost, 4, 8, 15, 16, 23, 42
    .data --433, 653, 30
    .data 763, 599, ++875, 5
    .data 4, 8, 1  5 , 16, 23, 42
    .data 4,8,15,16,23,4 +2
    .data  4, 8, 15, x, 16, 23, 42
    .data a, b, c, d
    .data 3, 4, 6.5, 9


;string errors
    .string
    .string za wardo
    .string za wardo"
    .string "za wardo
    .string za "wardo".
    .string za ."wardo"
    .string ",


