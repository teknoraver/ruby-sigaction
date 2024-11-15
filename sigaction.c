#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "ruby.h"

static struct sigaction sa_old[64]; // SIGRTMAX

static void signal_handler(int sig, siginfo_t *info, void *ucontext)
{
	pid_t sender_pid = info->si_pid;
	fprintf(stderr, "Signal %d received from PID %d\n", sig, sender_pid);
}

static VALUE set_signal_handler(VALUE recv, VALUE sig_num)
{
	int signal = NUM2INT(sig_num);

	struct sigaction sa = {
		.sa_sigaction = signal_handler,
		.sa_flags = SA_SIGINFO,
	};
	sigemptyset(&sa.sa_mask);

	if (sigaction(signal, &sa, sa_old + signal) == -1)
		rb_raise(rb_eStandardError, "sigaction failed");

	return Qnil;
}

static VALUE reset_signal_handler(VALUE recv, VALUE sig_num)
{
	int signal = NUM2INT(sig_num);

	if (sigaction(signal, sa_old + signal, NULL) == -1)
		rb_raise(rb_eStandardError, "sigaction reset failed");

	return Qnil;
}

void Init_sigaction()
{
	VALUE mSigAction = rb_define_module("SigAction");
	rb_define_module_function(mSigAction, "set_signal_handler", set_signal_handler,  1);
	rb_define_module_function(mSigAction, "reset_signal_handler", reset_signal_handler, 1);
}
