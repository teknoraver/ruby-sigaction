CFLAGS += -O2 -Wall -fPIC
LDFLAGS += -shared

all:: sigaction.so

%.so: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $<

test:: all
	ruby sigaction_test.rb

clean::
	$(RM) *.so
