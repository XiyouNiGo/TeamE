void my_err(const char *err_string, int line)
{
	fprintf(stderr, "(Line:%d) ", line);
	perror(err_string);
	exit(1);
}
#ifdef _DEBUG_H_

#define debug(format,...) printf("(File:%s,Line:%d) " format, __FILE__, __LINE__,  ##__VA_ARGS__)

#else

#define debug(format,...)

#endif
