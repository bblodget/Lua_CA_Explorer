
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include <iostream>
#include "zforth_cam.h"

namespace zf
{

	/*
	 * Evaluate buffer with code, check return value and report errors
	 */

	zf_result do_eval(const char* src, int line, const char* buf)
	{
		const char* msg = NULL;

		zf_result rv = zf_eval(buf);

		switch (rv)
		{
		case ZF_OK: break;
		case ZF_ABORT_INTERNAL_ERROR: msg = "internal error"; break;
		case ZF_ABORT_OUTSIDE_MEM: msg = "outside memory"; break;
		case ZF_ABORT_DSTACK_OVERRUN: msg = "dstack overrun"; break;
		case ZF_ABORT_DSTACK_UNDERRUN: msg = "dstack underrun"; break;
		case ZF_ABORT_RSTACK_OVERRUN: msg = "rstack overrun"; break;
		case ZF_ABORT_RSTACK_UNDERRUN: msg = "rstack underrun"; break;
		case ZF_ABORT_NOT_A_WORD: msg = "not a word"; break;
		case ZF_ABORT_COMPILE_ONLY_WORD: msg = "compile-only word"; break;
		case ZF_ABORT_INVALID_SIZE: msg = "invalid size"; break;
		case ZF_ABORT_DIVISION_BY_ZERO: msg = "division by zero"; break;
		default: msg = "unknown error";
		}

		if (msg) {
			//fprintf(stderr, "\033[31m");
			//if (src) fprintf(stderr, "%s:%d: ", src, line);
			//fprintf(stderr, "%s\033[0m\n", msg);
			if (src) std::cout << src << ":" << line << ": ";
			std::cout << msg << "\n";
		}

		return rv;
	}


	/*
	 * Load given forth file
	 */

	void include(const char* fname)
	{
		char buf[256];
		errno_t err;
		FILE* f;

		//FILE *f = fopen_s(fname, "rb");
		err = fopen_s(&f, fname, "rb");
		int line = 1;
		if (f) {
			while (fgets(buf, sizeof(buf), f)) {
				do_eval(fname, line++, buf);
			}
			fclose(f);
		}
		else {
			//fprintf(stderr, "error opening file '%s': %s\n", fname, strerror(errno));
			std::cout << "error opening file '" << fname << "' \n";
		}
	}


	/*
	 * Save dictionary
	 */

	void save(const char* fname)
	{
		size_t len;
		void* p = zf_dump(&len);
		errno_t err;
		FILE* f;
		//FILE *f = fopen(fname, "wb");
		err = fopen_s(&f, fname, "wb");

		if (f) {
			fwrite(p, 1, len, f);
			fclose(f);
		}
	}


	/*
	 * Load dictionary
	 */

	void load(const char* fname)
	{
		size_t len;
		void* p = zf_dump(&len);
		errno_t err;
		FILE* f;

		//FILE *f = fopen(fname, "rb");
		err = fopen_s(&f, fname, "rb");
		if (f) {
			fread(p, 1, len, f);
			fclose(f);
		}
		else {
			perror("read");
		}
	}

}


/*
 * Sys callback function
 */

zf_input_state zf_host_sys(zf_syscall_id id, const char *input)
{
	switch((int)id) {


		/* The core system callbacks */

		case ZF_SYSCALL_EMIT:
			//putchar((char)zf_pop());
			//fflush(stdout);
			std::cout << (char)zf_pop();
			break;

		case ZF_SYSCALL_PRINT:
			//printf(ZF_CELL_FMT " ", zf_pop());
			std::cout << zf_pop() << " ";
			break;

		case ZF_SYSCALL_TELL: {
			zf_cell len = zf_pop();
			void *buf = (uint8_t *)zf_dump(NULL) + (int)zf_pop();
			(void)fwrite(buf, 1, len, stdout);
			fflush(stdout); 
			//std::cout << (uint8_t*)buf;
			//(void)std::cout.write((char *)buf, len );
			}
			break;


		/* Application specific callbacks */

		case ZF_SYSCALL_USER + 0:
			//printf("\n");
			std::cout << "\n";
			exit(0);
			break;

		case ZF_SYSCALL_USER + 1:
			zf_push(sin(zf_pop()));
			break;

		case ZF_SYSCALL_USER + 2:
			if(input == NULL) {
				return ZF_INPUT_PASS_WORD;
			}
			zf::include(input);
			break;
		
		case ZF_SYSCALL_USER + 3:
			zf::save("zforth.save");
			break;

		default:
			//printf("unhandled syscall %d\n", id);
			std::cout << "unhandled syscall " << id << " \n";
			break;
	}

	return ZF_INPUT_INTERPRET;
}


/*
 * Tracing output
 */

void zf_host_trace(const char *fmt, va_list va)
{
	//fprintf(stderr, "\033[1;30m");
	vfprintf(stderr, fmt, va);
	//fprintf(stderr, "\033[0m");
}


/*
 * Parse number
 */

zf_cell zf_host_parse_num(const char *buf)
{
	zf_cell v;
	int n = 0;
	int r = sscanf_s(buf, "%f%n", &v, &n);
	if(r == 0 || buf[n] != '\0') {
		zf_abort(ZF_ABORT_NOT_A_WORD);
	}
	return v;
}


/*
 * End
 */

