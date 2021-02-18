>1. Pwn 新司机如何快速上路
2. About me ID:muhe(o0xmuhe@gmail.com o0xmuhe.me) 关注二进制安全 /CTFer member of Syclover member of SecBox
3. Outline - Stack - ROP - Ret 2 dl-resolve - Heap - unlink - Malloc Maleficarum - UAF - Other vuln - Format sting - Integer overflow - Tricks - Libc dump - Guess libc
4. Overvie w 先来看一张图
5. 为什么需要 ROP - 1 在 DEP/NX 不存在的 情况下，我们可以看到堆 和栈都是可以执行的。
6. 为什么需要 ROP - 2 在 DEP/NX 存在的情 况下，我们可以看到，只 有代码段是可以执行的。
7. ROP 是什么 ? 返回导向编程 (Return-Oriented Programming, ROP) 是计算机安全漏洞利用技术，该技术允 许攻击者在安全防御的情况下执行代码，如不可执行的内存和代码签名。 攻击者控制堆栈调用以劫持程序控制流并执行针对性的机器语言指令序列（称为 Gadgets ）。 每一段 gadget 通常结束于 return 指令，并位于共享库代码中的子程序。系列调用这些代码，攻击 者可以在拥有更简单攻击防范的程序内执行任意操作。 gadge ts ROP 链 ----------- 经过组合 ------------->
8. ROP 图解 ROP 执行过程 [exit(0)]
9. ROP 的一个例子 - RCTF pwn200
10. ROP 的一个例子 - RCTF pwn200 Exp 中部分代码 (leak 函 数 ):
11. Ret 2 dl-resolve 在之前的例子中，我们使用了 ROP ，但是为了完成整个利用，还是需要去 leak 一些我们需要的 函数地址，然后才可以 getshell 。 利用 linux 下可执行文件函数解析特性，即延迟绑定：在第一次调用时才会解析其地址并填充 至 .got.plt 。 所以，我们不需要做 leak ，就可以直接调用想要的函数。利用的就是伪造所需信息，直接解析得 到 system 的地址进而 ROP 。
12. Ret 2 dl-resolve 第一次调用 read 函数的时 候 跳到下面两条指令去，先是把 offset 压栈操作，然后跳到 .got.plt 处 然后压栈 link-map(GOT+4) 再调用 _dl_runtime_resolve(link_map,reloc_reg)
13. Ret 2 dl-resolve 这 种 利用的 trick 就是在这个 _dl_runtime_resolve() 函数身上了
14. Ret 2 dl-resolve 关键的函数： _dl_fixup (struct link_map *l, ElfW(Word) reloc_arg) 解析执行过程：
15. Ret 2 dl-resolve 利用思路： 1. 控制 EIP 为 PLT[0] 的地址，只需传递一个 index_arg 参数 2. 控制 index_arg 的大小，使 reloc 的位置落在可控地址内 3. 伪造 reloc 的内容，使 sym 落在可控地址内 4. 伪造 sym 的内容，使 name 落在可控地址内 5. 伪造 name 为任意库函数，如 system
16. Ret 2 dl-resolve 例子 - code gate final – yocto( 以此为例 ) - Xdctf 2015 pwn200
17. Heap 1. dlmalloc – General purpose allocator 2. ptmalloc2 – glibc 3. jemalloc – FreeBSD and Firefox 4. tcmalloc – Google 5. libumem – Solaris …
18. Heap 先是一点关于 chunk 结构的介 绍
19. Heap •Fast bin • x86_32:16~64bytes; x86_64:32~128bytes • Last In First Out •Bins • Bin 1 – Unsorted bin • freed small or large chunk added into unsorted bin • Bin 2 to Bin 63 – Small bin • 8 bytes apart,eg:16,24,32....508 • Bin 64 to Bin 126 – Large bin • >=512 bytes 更多详细的内容可以看这篇文章
20. Heap--unlink 例子： defcon ctf -- babyheap
21. Heap--unlink 新版本的 libc 增加了 check 应对策略： 我们找到一个特殊的 指针 ptr 是指向 p 的 (p 指向堆 ) 那么可以根据 p 去构造 bk 和 fd 两个指针
22. Heap--Malloc Maleficarum 1. The House of Prime 2. The House of Mind 3. The House of Force 4. The House of Lore 5. The House of Spirit 6. The House of Chaos
23. Heap--Malloc Maleficarum The House of Force: 能这 重写 这这这这这 的 size ，然后可以分配到任意地址。但是 size 的构造需要堆地址 ，所以需要 heap addr leak 的配合来使用。 例子： BCTF 2016 bcloud
24. Heap—hof 例子 堆地址泄露 Hof ，可以构造 size ，分配 到任意地址
25. Heap—hof 例子 利用思路如图所示 ( 直接从自己博客截的 图)
26. Heap--hos The House of Spirit 要求攻击者需要控制 free() 的指针，通过伪造 chunk 去欺骗 free() 函数去 free() 栈 上空间，然后再通过一次 malloc() 分配去得到之前”释放掉的空间”，一般是包括 ret addr 的 fastbin. 这种利用使用的块一般是 fastbin ，新分配的块包含 ret addr ，结合其他的一些条件就可 以玩耍了。不过要注意在 free 的时候，会 check 下一个块，所以这种利用限制比较多。
27. Heap—hos 例子
28. Heap--UAF Use After Free( 释放后重用 ) ，即重用了释放后的内存。 借鉴了一段典型的代码 : 例子： Pwnable.kr Toddler ‘s Bottle 的 UAF
29. Heap--UAF 可以先 delete 两个对象，再多次分 配空间，分配到刚才 free 的空间，然后再 去调用 ( 原来这 象的方法已 这这这这这这这 被覆盖 这 这 )。 我们可以控制 argv1 和 argv2 第一个是 读取字节数，第二个是 读取的文 件
30. Other vuln Format string( 格式化字符 串) 利用思路： 因为 fmt 是一个任意地址读写的漏洞，我们可以把 sc 写进一段 w+x 的空间，然后改返回地址过去执行 sc ；或者是改 got 之类的。 然而…上面说的是 用户输入在栈中 ( 局部变量 ) 类似下面这种情况
31. Other vuln Format string( 格式化字符 串) 如果用户输入放在全局变量里呢 (.bss) ，就无法用之前的方式确定位置。 例子： plaidctf-2015 ebp
32. Other vuln ebp --- 利用思路 1.leak 栈地址 找到 ret addr( 当前 make_response) 2. 写到上一个 ebp 去 3. 修改 ret addr 到 0x0804A040( 用户输入所在 )
33. Other vuln 整型溢出 (x86) 就像一个圆，绕一圈，又回来了。
34. Other vuln 整型溢出一般是作为漏洞利用的一个部分而 存在的，比如作这 这 这 入这 这 入的 这这这 度。 这 我们的输入 是被限制的， 4~8 的这 度。 这这 但是长度的值是一个 unsigned char 范围是 0~255 这样的话，如果输入超过 255 那就会造成溢出 ( 就像一 个圆环，又回到起点 ) 。
35. Other vuln 这有 利用运算 这这果的整数溢出。 这这这这这这 Xctf-final -richman 在使用 market 进行出售货物的时候，如果出 售的单元数量很大，在 imul 指令后发生整数溢 出。
36. Tricks Libc dump 比赛中，有的时候几个 pwn 是配置在一起的 ( 同一台服务器 ) ，同一个 libc 。即我们可以在搞下第一个服务的时候，拖下 libc 这用 。 具体做法： 本地开启 wireshark ， exp 打这这 去之后，直接 这 这 这 这 这 cat 所对 应的 libc 文件；之后断开这这 接从 这 wireshark 中把 libc 提取出 来 ( 可能要去掉点 junk) 。
37. Tricks Libc dump 如果有任意地址读的漏洞，也可以构造一个循环，去把 libc dump 出来 比如刚刚结束的 LCTF 2016 – Pwn300
38. Tricks Guess libc 假设我们现在已经有一个任意地址读的条件，可以 leak libc 。但是有时候觉得麻 烦些，这时候就可以考虑只 leak 若干个函数，然后去 libcdb 查询 libc 版本了。 比如之前比赛中遇 到的一个 fmt string 的 利用，我就是这么做的 。
39. Summary 1. 由于 CTF 这事 的增多， 这这目的 这 这这度 这这从前有提升， 这 这 这 这 这 这这这度也上升。 这这这这 由原来的单一的 vuln ----> vulns 的组合利用 代码的复杂度提高，越来越偏实际案例。 2. 更注重基本功 ( 逆向分析能力、基础知识掌握情况 ) 3. 现在 CTF 考查重点还是在漏洞的利用，因为一般来说， 洞都比较好找，难可能就难在利用了。未来不知道会不会考察 到漏洞挖掘？比如 fuzz ？ ( 个人猜想 )
40. And… CTF 越来越多了，这 这 这 的 CTF 无疑会帮助我们提高，但是有些 CTF 就… 在选择的时候擦亮眼睛，李逵不是李鬼。 比如 *CTF 2016 和 **CTF2016… 我们就会去选择 *CTF 2016
41. Thanks to 。 在 CTF 比赛中认识了很多优秀的选手 ( 师傅 ) ，和他们多交流、学习，自己会进步的更快 - Chxx wuyan o_0xJ0k3r Bigtang Jarvis zwjj Icemakr ….
42. Q&A
43. Thank You