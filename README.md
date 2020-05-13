# hashTable
Analysis of different hash functions and table optimization with the help of assembler

### Differrent hash functions
The comparison was made with a dictionary containing 6,765 unique words and the size of table 4,597.

* #### Const hash
    return const 1
![const hash](https://github.com/BorisTab/hashTable/blob/inlineOptimization/hashAnalysis/constHash.png)

* #### Size hash
    return size of word
![size hash](https://github.com/BorisTab/hashTable/blob/inlineOptimization/hashAnalysis/sizeHash.png)

* #### ASCII and ASCII/size hashes
    ASCII return amount of ASCII-code of word                                      
    ASCII/size return amount of ASCII-code of word divided by size
![ASCII hash](https://github.com/BorisTab/hashTable/blob/inlineOptimization/hashAnalysis/ASCIIHash.png)

* #### Xor cycle hash
    Implementation:
        unsigned int hash = 0;
        unsigned int mask = 1;
        mask <<= sizeof(unsigned int)*8 - 1;

        while (*word != '\0') {
            hash ^= *word;
            ++word;

            unsigned int firstBit = hash & mask;
            firstBit >>= sizeof(unsigned int)*8 - 1;

            hash <<= 1;
            hash |= firstBit;
        }

        return hash;
![xor cycle hash](https://github.com/BorisTab/hashTable/blob/inlineOptimization/hashAnalysis/xorHash.png)

* #### H37 hash
    Implementation:
      
        unsigned int hash = 2139062143;
        while (*word != '\0') {
            hash = 37*hash + *word;
            ++word;
        }
        return hash;

![H37 hash](https://github.com/BorisTab/hashTable/blob/inlineOptimization/hashAnalysis/h37Hash.png)

* #### H37 modified hash
    return low two bytes of H37 hash
![H37 mod hash](https://github.com/BorisTab/hashTable/blob/inlineOptimization/hashAnalysis/h37modHash.png)


### Hash table optimization
Find the busiest funtions with Perf
![perf](https://github.com/BorisTab/hashTable/blob/inlineOptimization/hashAnalysis/perf.png)

On top we can see two funtions <code>hash</code> and <code>strcmp</code>

* #### Inline optimization
    <code>strcmp</code> inline optimization

        asm (".intel_syntax noprefix\n"
                 "            xor rax, rax\n"
                 "            mov   bl, 0\n"
                 "            cld\n"

                 ".compare:   cmpsb\n"
                 "            jne   .endCmp\n"
                 "            dec   rsi\n"
                 "            lodsb\n"
                 "            cmp   bl, al\n"
                 "            jne    .compare\n"

                 ".endCmp:    mov   al, [rsi - 1]\n"
                 "            sub   al, [rdi - 1]\n"
                 ".att_syntax prefix\n"
            : "=a"(equal)
            : "S"(currentPointer->value), "D" (val)
            : "rbx"
            );
       
* #### Asm function
    Implemetation of xor cycle hash as separate asm-function:
    
        hash5asm:
        xor     rdx, rdx
        xor     rax, rax
        mov     ebx, 1
        shl     ebx, 0x1f

        .loop:
        xor     al, [rdi]
        inc     rdi

        mov     ecx, eax
        and     ecx, ebx
        shr     ecx, 0x1f

        shl     eax, 1
        or      eax, ecx

        cmp     dl, [rdi]
        jne     .loop

        ret
        
### Speed changes
Program was compiled with -O0 flag
| Modification | Time, s |
|:------------:|:-------:|
| Without mod  |  28.76  |
| strcmp opt   | 23.10   |
| hash opt     | 18.52   |
| both         | 14.64   |

Maximum speed acceleration **1.96**
