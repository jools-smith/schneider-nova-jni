#include <stdio.h>
#include <stdlib.h>

int schneider_nova_jni_test();

/**
 * gcc test.c -L. -lNovaJni -o test
 *
 * must have SO in local directory
 */

int main() {

	system("clear");

	printf("+-------------------------+\n");
	printf("| NovaJni.so test harness |\n");
	printf("+-------------------------+\n");
	printf("\n");

	printf("library path = [%s]\n", getenv("LD_LIBRARY_PATH"));
	printf("\n");

	schneider_nova_jni_test();

	return 0;
}
