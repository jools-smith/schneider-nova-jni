CFLAGS=-O0 -pie
IFLAGS=-I/home/jools/eclipse-workspace/schneider-nova-jni -I/home/jools/eclipse-workspace/schneider-nova-jni/fne-toolkit/include
LDLIBS=-L. -lNovaJni

all: nova-jni-test-harness

nova-jni-test-harness: nova-jni-test-harness.c
	g++ $(CFLAGS) $(IFLAGS) -o nova-jni-test-harness nova-jni-test-harness.c $(LDLIBS)

clean:
	rm -f nova-jni-test-harness
	