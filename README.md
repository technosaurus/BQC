# BQC
Brad's Quixotic C - a barebones libc in a single header file

*TODO*
 * [implement DNS lookup and caching](http://www.murga-linux.com/puppy/viewtopic.php?t=96715)
 * [Partial support for Posix C](http://pubs.opengroup.org/onlinepubs/9699919799/)
 * [More Architectures](https://git.kernel.org/cgit/linux/kernel/git/next/linux-next.git/patch/?id=f5738ceed46782aea7663d62cb6398eb05fc4ce0)
 * [Support Windows Syscalls](http://www.drdobbs.com/windows/windows-nt-system-call-hooking/windows/sourcecode/windows-nt-system-call-hooking/30201746)
 * [BSD Syscall support](https://www.freebsd.org/doc/en/books/developers-handbook/x86-system-calls.html)
 * [add inline documentation to functions](https://cs50.harvard.edu/resources/cppreference.com/all_c_functions.html)
 * [use lolremez for libm](https://github.com/samhocevar/lolremez)

*HOWTO*<br>
for best results "amalgamate" your app into a single c file (main.c for example)<br>
compile with `gcc -nostartfiles -nostdlib $CFLAGS -o myapp main.c $LDFLAGS`
