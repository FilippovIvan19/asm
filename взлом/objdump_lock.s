
lock:     file format elf64-x86-64


Disassembly of section .note.gnu.property:

0000000000400120 <.note.gnu.property>:
  400120:	04 00                	add    al,0x0
  400122:	00 00                	add    BYTE PTR [rax],al
  400124:	10 00                	adc    BYTE PTR [rax],al
  400126:	00 00                	add    BYTE PTR [rax],al
  400128:	05 00 00 00 47       	add    eax,0x47000000
  40012d:	4e 55                	rex.WRX push rbp
  40012f:	00 01                	add    BYTE PTR [rcx],al
  400131:	00 00                	add    BYTE PTR [rax],al
  400133:	c0 04 00 00          	rol    BYTE PTR [rax+rax*1],0x0
  400137:	00 01                	add    BYTE PTR [rcx],al
  400139:	00 00                	add    BYTE PTR [rax],al
  40013b:	00 00                	add    BYTE PTR [rax],al
  40013d:	00 00                	add    BYTE PTR [rax],al
	...

Disassembly of section .text:

0000000000401000 <_start>:
  401000:	b8 01 00 00 00       	mov    eax,0x1
  401005:	bf 01 00 00 00       	mov    edi,0x1
  40100a:	48 be 00 20 40 00 00 	movabs rsi,0x402000
  401011:	00 00 00
  401014:	48 8b 14 25 1e 20 40 	mov    rdx,QWORD PTR ds:0x40201e
  40101b:	00
  40101c:	0f 05                	syscall


  40101e:	b8 00 00 00 00       	mov    eax,0x0
  401023:	bf 01 00 00 00       	mov    edi,0x1
  401028:	48 be 26 20 40 00 00 	movabs rsi,0x402026
  40102f:	00 00 00
  401032:	ba 32 00 00 00       	mov    edx,0x32
  401037:	0f 05                	syscall


  401039:	48 8b 04 25 26 20 40 	mov    rax,QWORD PTR ds:0x402026;inputBuf
  401040:	00
  401041:	48 03 04 25 64 20 40 	add    rax,QWORD PTR ds:0x402064;mask
  401048:	00
  401049:	48 3b 04 25 2f 20 40 	cmp    rax,QWORD PTR ds:0x40202f;checkBuf
  401050:	00
  401051:	74 23                	je     401076 <askPassword.denAccess>


  401053:	48 be 4e 20 40 00 00 	movabs rsi,0x40204e
  40105a:	00 00 00
  40105d:	48 8b 14 25 5c 20 40 	mov    rdx,QWORD PTR ds:0x40205c
  401064:	00
  401065:	b8 01 00 00 00       	mov    eax,0x1
  40106a:	bf 01 00 00 00       	mov    edi,0x1
  40106f:	0f 05                	syscall
  401071:	e8 8a ff ff ff       	call   401000 <_start>

0000000000401076 <askPassword.denAccess>:
  401076:	48 be 37 20 40 00 00 	movabs rsi,0x402037
  40107d:	00 00 00
  401080:	48 8b 14 25 46 20 40 	mov    rdx,QWORD PTR ds:0x402046
  401087:	00
  401088:	b8 01 00 00 00       	mov    eax,0x1
  40108d:	bf 01 00 00 00       	mov    edi,0x1
  401092:	0f 05                	syscall

  401094:	b8 3c 00 00 00       	mov    eax,0x3c
  401099:	48 31 ff             	xor    rdi,rdi
  40109c:	0f 05                	syscall

Disassembly of section .data:

0000000000402000 <welcMsg>:
  402000:	45 6e                	rex.RB outs dx,BYTE PTR ds:[rsi]
  402002:	74 65                	je     402069 <mask+0x5>
  402004:	72 20                	jb     402026 <inputBuf>
  402006:	61                   	(bad)
  402007:	20 70 61             	and    BYTE PTR [rax+0x61],dh
  40200a:	73 73                	jae    40207f <_end+0xf>
  40200c:	77 6f                	ja     40207d <_end+0xd>
  40200e:	72 64                	jb     402074 <_end+0x4>
  402010:	20 28                	and    BYTE PTR [rax],ch
  402012:	38 20                	cmp    BYTE PTR [rax],ah
  402014:	73 79                	jae    40208f <_end+0x1f>
  402016:	6d                   	ins    DWORD PTR es:[rdi],dx
  402017:	62                   	(bad)
  402018:	6f                   	outs   dx,DWORD PTR ds:[rsi]
  402019:	6c                   	ins    BYTE PTR es:[rdi],dx
  40201a:	73 29                	jae    402045 <grMsg+0xe>
  40201c:	3a 0a                	cmp    cl,BYTE PTR [rdx]

000000000040201e <welcMsgSize>:
  40201e:	1e                   	(bad)
  40201f:	00 00                	add    BYTE PTR [rax],al
  402021:	00 00                	add    BYTE PTR [rax],al
  402023:	00 00                	add    BYTE PTR [rax],al
	...

0000000000402026 <inputBuf>:
  402026:	61                   	(bad)
  402027:	62 63 64 65 66       	(bad)  {k5}
  40202c:	67 68 0a       	addr32 push 0x7462710a

000000000040202f <checkBuf>:
  40202f:	71 62                	jno    402093 <_end+0x23>
  402031:	74 74                	je     4020a7 <_end+0x37>
  402033:	78 70                	js     4020a5 <_end+0x35>
  402035:	73 65                	jae    40209c <_end+0x2c>

0000000000402037 <grMsg>:
  402037:	41 63 63 65          	movsxd esp,DWORD PTR [r11+0x65]
  40203b:	73 73                	jae    4020b0 <_end+0x40>
  40203d:	20 67 72             	and    BYTE PTR [rdi+0x72],ah
  402040:	61                   	(bad)
  402041:	6e                   	outs   dx,BYTE PTR ds:[rsi]
  402042:	74 65                	je     4020a9 <_end+0x39>
  402044:	64 0a              	or     cl,BYTE PTR fs:[rdi]

0000000000402046 <grMsgSize>:
  402046:	0f 00 00             	sldt   WORD PTR [rax]
  402049:	00 00                	add    BYTE PTR [rax],al
  40204b:	00 00                	add    BYTE PTR [rax],al
	...

000000000040204e <dnMsg>:
  40204e:	41 63 63 65          	movsxd esp,DWORD PTR [r11+0x65]
  402052:	73 73                	jae    4020c7 <_end+0x57>
  402054:	20 64 65 6e          	and    BYTE PTR [rbp+riz*2+0x6e],ah
  402058:	69 65 64 0a    	imul   esp,DWORD PTR [rbp+0x64],0xe0a

000000000040205c <dnMsgSize>:
  40205c:	0e                   	(bad)
  40205d:	00 00                	add    BYTE PTR [rax],al
  40205f:	00 00                	add    BYTE PTR [rax],al
  402061:	00 00                	add    BYTE PTR [rax],al
	...

0000000000402064 <mask>:
  402064:	01 01                	add    DWORD PTR [rcx],eax
  402066:	01 01                	add    DWORD PTR [rcx],eax
  402068:	01 01                	add    DWORD PTR [rcx],eax
  40206a:	01 01                	add    DWORD PTR [rcx],eax
