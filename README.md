# Woody-woodpacker

64-bit ELF binary packer that injects x86-64 assembly into executables and encrypts their code at the binary level. It parses the ELF structure to locate the '.text' section, encrypts it with a randomly generated key, and embeds a custom decryption stub directly into the binary, in the gap between ELF segments. When the packed binary is executed, the stub decrypts the original code at runtime and transfers control to it, running identically to the original.

## Usage

```sh
make
./woody_woodpacker <elf64-binary>
```

The encryption key is printed to stdout. The packed binary is written to `./woody`.

## Example

```sh
# ./sample
Hello, World!
# ./woody_woodpacker sample
key_value: 07A51FF040D45D5CD
# ls
sample sample.c woody woody_woodpacker
# objdump -D sample | tail -f -n 20
45: 67 73 2f addr16 jae 77 <_init-0x80481f9>
48: 52 push %edx
49: 45 inc %ebp
4a: 4c dec %esp
4b: 45 inc %ebp
4c: 41 inc %ecx
4d: 53 push %ebx
4e: 45 inc %ebp
4f: 5f pop %edi
50: 33 36 xor (%esi),%esi
52: 32 2f xor (%edi),%ch
54: 66 69 6e 61 6c 29 imul $0x296c,0x61(%esi),%bp
5a: 20 28 and %ch,(%eax)
5c: 62 61 73 bound %esp,0x73(%ecx)
5f: 65 64 20 6f 6e gs and %ch,%fs:0x6e(%edi)
64: 20 4c 4c 56 and %cl,0x56(%esp,%ecx,2)
68: 4d dec %ebp
69: 20 33 and %dh,(%ebx)
6b: 2e 36 2e 32 29 cs ss xor %cs:(%ecx),%ch
...
# objdump -D woody | tail -f -n 20
197: 64 69 6e 5f 75 73 65 imul $0x64657375,%fs:0x5f(%rsi),%ebp
19e: 64
19f: 00 5f 5f add %bl,0x5f(%rdi)
1a2: 6c insb (%dx),%es:(%rdi)
1a3: 69 62 63 5f 63 73 75 imul $0x7573635f,0x63(%rdx),%esp
1aa: 5f pop %rdi
1ab: 69 6e 69 74 00 5f 5f imul $0x5f5f0074,0x69(%rsi),%ebp
1b2: 62 73 (bad) {%k7}
1b4: 73 5f jae 215 <(null)-0x400163>
1b6: 73 74 jae 22c <(null)-0x40014c>
1b8: 61 (bad)
1b9: 72 74 jb 22f <(null)-0x400149>
1bb: 00 6d 61 add %ch,0x61(%rbp)
1be: 69 6e 00 5f 5f 54 4d imul $0x4d545f5f,0x0(%rsi),%ebp
1c5: 43 5f rex.XB pop %r15
1c7: 45 rex.RB
1c8: 4e rex.WRX
1c9: 44 5f rex.R pop %rdi
1cb: 5f pop %rdi
...
# ./sample
Hello, World!
# ./woody
....WOODY.....
Hello, World!
```
