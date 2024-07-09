.ORIG x3000        ; addr where prog will be loaded
LEA R0, HELLO_STR  ; load addr of the HELLO_STR into R0
PUTs               ; output the str pointed to by R0
HALT               ; halt prog
HELLO_STR .STRINGZ "Hello world!"  ; store this str here in the prog
.END               ; end of file
