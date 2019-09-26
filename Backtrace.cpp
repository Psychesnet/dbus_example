#include "Backtrace.h"

Backtrace::Backtrace()
{
	logger_init(LOGGER_TYPE_STDOUT);
	register_backtrace();
	log_info("register backtrace feature");
}

Backtrace::~Backtrace()
{
	logger_deinit();
	log_info("unregister backtrace feature");
}

void Backtrace::register_backtrace()
{
	struct sigaction sigact;
	sigact.sa_sigaction = handle_backtrace;
	// we need SA_RESTART to continue running interrupted process
	sigact.sa_flags = SA_RESTART | SA_SIGINFO;
	sigaction(SIGSEGV, &sigact, (struct sigaction *)nullptr);
    sigaction(SIGABRT, &sigact, (struct sigaction *)nullptr);
    sigaction(SIGTRAP, &sigact, (struct sigaction *)nullptr);
    sigaction(SIGXCPU, &sigact, (struct sigaction *)nullptr);
    sigaction(SIGFPE,  &sigact, (struct sigaction *)nullptr);
    sigaction(SIGPIPE, &sigact, (struct sigaction *)nullptr);
    sigaction(SIGALRM, &sigact, (struct sigaction *)nullptr);
}

void Backtrace::handle_backtrace(int signum
#ifdef Darwin
		,
		struct __siginfo *act,
		void *oact
#endif
		)

{
	log_critical("stack trace(%d):", signum);

	const int max_frames = 63;
	// storage array for stack trace address data
	void* addrlist[max_frames+1];

	// retrieve current stack addresses
	int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

	if (addrlen == 0) {
		log_critical("<empty, possibly corrupt>");
		return;
	}

	// resolve addresses into strings containing "filename(function+address)",
	// this array must be free()-ed
	char** symbollist = backtrace_symbols(addrlist, addrlen);

	// allocate string which will be filled with the demangled function name
	size_t funcnamesize = 256;
	char* funcname = (char*)malloc(funcnamesize);

	// iterate over the returned symbol lines. skip the first, it is the
	// address of this function.
	for (int i = 1; i < addrlen; i++)
	{
		char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

		// find parentheses and +address offset surrounding the mangled name:
		// ./module(function+0x15c) [0x8048a6d]
		for (char *p = symbollist[i]; *p; ++p)
		{
			if (*p == '(')
				begin_name = p;
			else if (*p == '+')
				begin_offset = p;
			else if (*p == ')' && begin_offset) {
				end_offset = p;
				break;
			}
		}

		if (begin_name && begin_offset && end_offset
				&& begin_name < begin_offset)
		{
			*begin_name++ = '\0';
			*begin_offset++ = '\0';
			*end_offset = '\0';

			// mangled name is now in [begin_name, begin_offset) and caller
			// offset in [begin_offset, end_offset). now apply
			// __cxa_demangle():

			int status;
			char* ret = abi::__cxa_demangle(begin_name,
					funcname, &funcnamesize, &status);
			if (status == 0) {
				funcname = ret; // use possibly realloc()-ed string
				log_critical("  %s : %s+%s",
						symbollist[i], funcname, begin_offset);
			}
			else {
				// demangling failed. Output function name as a C function with
				// no arguments.
				log_critical("  %s : %s()+%s",
						symbollist[i], begin_name, begin_offset);
			}
		}
		else
		{
			// couldn't parse the line? print the whole line.
			log_critical("  %s", symbollist[i]);
		}
	}

	free(funcname);
	free(symbollist);
}
