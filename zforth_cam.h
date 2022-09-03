#ifndef zforth_eval_h
#define zforth_eval_h

#include "zforth.h"

//#ifdef __cplusplus
//extern "C" {
//#endif

namespace zf
{

	zf_result do_eval(const char* src, int line, const char* buf);
	void include(const char* fname);
	void save(const char* fname);
	void load(const char* fname);

}

zf_input_state zf_host_sys(zf_syscall_id id, const char *input);
void zf_host_trace(const char *fmt, va_list va);
zf_cell zf_host_parse_num(const char *buf);


//#ifdef __cplusplus
//}
//#endif

#endif
