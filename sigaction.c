#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "ruby.h"

struct handler {
	char *name;
	struct sigaction sa;
};

static struct handler handlers[64];	// SIGRTMAX

static void signal_handler(int sig, siginfo_t *info, void *ucontext)
{
	struct handler *h = handlers + sig;
	pid_t sender_pid = info->si_pid;

	fprintf(stderr, "Signal %d received from PID %d\n", sig, sender_pid);

	if (h->name)
		rb_funcall(rb_mKernel, rb_intern(h->name), 0);
	fprintf(stderr, "Signal handler %s called\n", h->name);
}

static VALUE set_signal_handler(VALUE recv, VALUE sig_num, VALUE handler)
{
	int signal = NUM2INT(sig_num);
	char *handler_name = StringValueCStr(handler);
	struct handler *h = handlers + signal;
	struct sigaction sa = {
		.sa_sigaction = signal_handler,
		.sa_flags = SA_SIGINFO,
	};

	sigemptyset(&sa.sa_mask);

	if (!sigaction(signal, &sa, &h->sa))
		h->name = strdup(handler_name);
	else
		rb_raise(rb_eStandardError, "sigaction failed");

	return Qnil;
}

static VALUE reset_signal_handler(VALUE recv, VALUE sig_num)
{
	int signal = NUM2INT(sig_num);
	struct handler *h = handlers + signal;

	if (!h->name)
		return Qnil;

	if (!sigaction(signal, &h->sa, NULL)) {
		free(h->name);
		h->name = NULL;
	} else
		rb_raise(rb_eStandardError, "sigaction reset failed");

	return Qnil;
}

void Init_sigaction()
{
	VALUE mSigAction = rb_define_module("SigAction");
	rb_define_module_function(mSigAction, "set_signal_handler", set_signal_handler,  2);
	rb_define_module_function(mSigAction, "reset_signal_handler", reset_signal_handler, 1);
}
