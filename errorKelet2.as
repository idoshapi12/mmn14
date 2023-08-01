;extraneous words
    .data 943 .data
    .string "sdf" .string
    .extern Hello World
mov A, r1, r2
mov A, r1 r2
cmp A, r1, r2
cmp A, r1 r2
add A, r0, r1
add A, r0 r1
sub #3, r1, r5
sub #3, r1 r5
lea HAHAHA, r3, r4
lea HAHAHA, r3 r4
clr r2, r6
clr r2 r6
inc r1, r4
inc r1 r4
dec Count, Monte, Cristo
dec Count Monte Cristo
jmp %ROOF, %FLOOR
jmp %ROOF %FLOOR
bne Bob, Sponge
bne Bob Sponge
jsr BAT, MAN
jsr BAT MAN
red r3, r5
red r3 r5
prn r1, r2
prn r1 r2
customLabel1:.data 943 .data
customLabel2:.string "sdf" .string
customLabel3:.extern Hello World
customLabel4:.entry Goodbye Earth
customLabel5:mov A, r1, r2
customLabel6:mov A, r1 r2
customLabel7:cmp A, r1, r2
customLabel8:cmp A, r1 r2
customLabel9:add A, r0, r1
customLabel10:add A, r0 r1
customLabel11:sub #3, r1, r5
customLabel12:sub #3, r1 r5
customLabel13:lea HAHAHA, r3, r4
customLabel14:lea HAHAHA, r3 r4
customLabel15:clr r2, r6
customLabel16:clr r2 r6
customLabel17:inc r1, r4
customLabel18:inc r1 r4
customLabel19:dec Count, Monte, Cristo
customLabel20:dec Count Monte Cristo
customLabel21:jmp %ROOF, %FLOOR
customLabel22:jmp %ROOF %FLOOR
customLabel23:bne Bob, Sponge
customLabel24:bne Bob Sponge
customLabel25:jsr BAT, MAN
customLabel26:jsr BAT MAN
customLabel27:red r3, r5
customLabel28:red r3 r5
customLabel29:prn r1, r2
customLabel30:prn r1 r2
;missing word
    .data
    .string
    .extern
mov A
cmp A
add A
sub #3
lea HAHAHA
clr
inc
dec
prn

someLabel31:.data
someLabel32:.string
someLabel33:.extern
someLabel34:.entry
someLabel35:mov A
someLabel36:cmp A
someLabel37:add A


;unknown words
COOK tasty, pie
    .Data 12, 675
    .sTring "Whoops"

;operand addressing errors
mov aohno, #1
mov #-1, #5
lea #-1, someLabel47
lea #-1, kohno
lea r1, r0
dec #-1

stop #-1
stop someLabel47

