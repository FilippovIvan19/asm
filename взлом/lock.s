

00001000  B80100            mov ax,0x1
00001003  0000              add [bx+si],al
00001005  BF0100            mov di,0x1
00001008  0000              add [bx+si],al
0000100A  48                dec ax
0000100B  BE0020            mov si,0x2000;Enter a password (8 symbols)
0000100E  40                inc ax
0000100F  0000              add [bx+si],al
00001011  0000              add [bx+si],al
00001013  00488B            add [bx+si-0x75],cl
00001016  1425              adc al,0x25
00001018  1E                push ds
00001019  204000            and [bx+si+0x0],al
0000101C  0F05              syscall

0000101E  B80000            mov ax,0x0
00001021  0000              add [bx+si],al
00001023  BF0100            mov di,0x1
00001026  0000              add [bx+si],al
00001028  48                dec ax
00001029  BE2620            mov si,0x2026;abcdefhg
0000102C  40                inc ax
0000102D  0000              add [bx+si],al
0000102F  0000              add [bx+si],al
00001031  00BA3200          add [bp+si+0x32],bh
00001035  0000              add [bx+si],al
00001037  0F05              syscall

00001039  48                dec ax
0000103A  8B04              mov ax,[si]
0000103C  252620            and ax,0x2026;abcdefhg
0000103F  40                inc ax
00001040  004803            add [bx+si+0x3],cl
00001043  0425              add al,0x25
00001045  64204000          and [fs:bx+si+0x0],al
00001049  48                dec ax
0000104A  3B04              cmp ax,[si]
0000104C  252F20            and ax,0x202f;qbttxps
0000104F  40                inc ax
00001050  007423            add [si+0x23],dh
00001053  48                dec ax
00001054  BE4E20            mov si,0x204e;Access denied
00001057  40                inc ax
00001058  0000              add [bx+si],al
0000105A  0000              add [bx+si],al
0000105C  00488B            add [bx+si-0x75],cl
0000105F  1425              adc al,0x25
00001061  5C                pop sp
00001062  204000            and [bx+si+0x0],al
00001065  B80100            mov ax,0x1
00001068  0000              add [bx+si],al
0000106A  BF0100            mov di,0x1
0000106D  0000              add [bx+si],al
0000106F  0F05              syscall

00001071  E88AFF            call 0xffe
00001074  FF                db 0xff
00001075  FF48BE            dec word [bx+si-0x42]
00001078  37                aaa
00001079  204000            and [bx+si+0x0],al
0000107C  0000              add [bx+si],al
0000107E  0000              add [bx+si],al
00001080  48                dec ax
00001081  8B14              mov dx,[si]
00001083  254620            and ax,0x2046;empty
00001086  40                inc ax
00001087  00B80100          add [bx+si+0x1],bh
0000108B  0000              add [bx+si],al
0000108D  BF0100            mov di,0x1
00001090  0000              add [bx+si],al
00001092  0F05              syscall

00001094  B83C00            mov ax,0x3c     ;exit
00001097  0000              add [bx+si],al
00001099  48                dec ax
0000109A  31FF              xor di,di
0000109C  0F05              syscall




00002000  45                inc bp
00002001  6E                outsb
00002002  7465              jz 0x2069
00002004  7220              jc 0x2026
00002006  61                popa
00002007  207061            and [bx+si+0x61],dh
0000200A  7373              jnc 0x207f
0000200C  776F              ja 0x207d
0000200E  7264              jc 0x2074
00002010  2028              and [bx+si],ch
00002012  3820              cmp [bx+si],ah
00002014  7379              jnc 0x208f
00002016  6D                insw
00002017  626F6C            bound bp,[bx+0x6c]
0000201A  7329              jnc 0x2045
0000201C  3A0A              cmp cl,[bp+si]
0000201E  1E                push ds
0000201F  0000              add [bx+si],al
00002021  0000              add [bx+si],al
00002023  0000              add [bx+si],al
00002025  006162            add [bx+di+0x62],ah
00002028  636465            arpl [si+0x65],sp
0000202B  6667680A716274    push dword 0x7462710a
00002032  7478              jz 0x20ac
00002034  7073              jo 0x20a9
00002036  6541              gs inc cx
00002038  636365            arpl [bp+di+0x65],sp
0000203B  7373              jnc 0x20b0
0000203D  206772            and [bx+0x72],ah
00002040  61                popa
00002041  6E                outsb
00002042  7465              jz 0x20a9
00002044  640A0F            or cl,[fs:bx]
00002047  0000              add [bx+si],al
00002049  0000              add [bx+si],al
0000204B  0000              add [bx+si],al
0000204D  004163            add [bx+di+0x63],al
00002050  636573            arpl [di+0x73],sp
00002053  7320              jnc 0x2075
00002055  64656E            gs outsb
00002058  6965640A0E        imul sp,[di+0x64],word 0xe0a
