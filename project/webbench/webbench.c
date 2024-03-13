#include "socket.c"
#include <unistd.h>
#include <sys/param.h>
#include <rpc/types.h>
#include <getopt.h>
#include <strings.h>
#include <time.h>
#include <signal.h>

#define DEBUG           1

/* globals */
int http10 = 1; /* 0 - http/0.9, 1 - http/1.0, 2 - http/1.1 */

/*  方法定义  Allow: GET, HEAD, OPTIONS, TRACE*/
#define METHOD_GET 0
#define METHOD_HEAD 1
#define METHOD_OPTIONS 2
#define METHOD_TRACE 3
/* 软件版本 */
#define PROGRAM_VERSION "1.5"

int force = 0;
int force_reload = 0;
int benchtime = 30; // 测试时间

static const struct option long_options[] = 
{
    {"force", no_argument, &force, 1},  // 不需要参数, 返回0, force变量置为1
    {"reload", no_argument, &force_reload, 1},
    {"time", required_argument, NULL, 't'}, // 需要参数, 返回t
    {"help", no_argument, NULL, '?'}, // 不需要参数, 返回?
    {"http09", no_argument, NULL, '9'},
    {"http10", no_argument, NULL,'1'},
    {"http11", no_argument,NULL, '2'},
    {"get", no_argument, &method, METHOD_GET},
    {"head", no_argument, &method, METHOD_HEAD},
    {"options", no_argument, &method, METHOD_OPTIONS},
    {"trace", no_argument, &method, METHOD_TRACE},
    {"version", no_argument, NULL, 'V'},
    {"proxy", required_argument, NULL, 'p'},
    {"clients", required_argument, NULL, 'c'},
    {NULL, 0, NULL, 0}  // 结尾
}

static void usage(void)
{
    fprintf(stderr,
    "webbench [option]... URL\n"
    "  -f|--force               Don't wait for reply from server.\n"
    "  -r|--reload              Send reload request - Praga: no_cache.\n"
    "  -t|--time <sec>          Run benchmark for <sec> seconds. Default 30.\n"
	"  -p|--proxy <server:port> Use proxy server for request.\n"
	"  -c|--clients <n>         Run <n> HTTP clients at once. Default one.\n"
	"  -9|--http09              Use HTTP/0.9 style requests.\n"
	"  -1|--http10              Use HTTP/1.0 protocol.\n"
	"  -2|--http11              Use HTTP/1.1 protocol.\n"
	"  --get                    Use GET request method.\n"
	"  --head                   Use HEAD request method.\n"
	"  --options                Use OPTIONS request method.\n"
	"  --trace                  Use TRACE request method.\n"
	"  -?|-h|--help             This information.\n"
	"  -V|--version             Display program version.\n"
    );
}

int main(int argc, char *argv[])
{
    int opt = 0;
    int options_index = 0;
    char *tmp = NULL;

    if (argc == 1) {
        usage();
        return 2;
    }

    while(opt = getopt_long(argc, argv, "912Vfrt:p:c:?h", long_options, &options_index)) {
        switch (opt)
        {
            // long_opt 且 option结构体的flag不为NULL时返回0
            case 0:
                break;
            case 'f': force = 1; break;
            case 'r': force_reload = 1; break;
            case '9': http10 = 0; break;
            case '1': http10 = 1; break;
            case '2': http10 = 2; break;
            case "V": printf(PROGRAM_VERSION"\n"); exit(0);
            case 't': 
                #ifdef DEBUG
                    print("case 't' optarg = %s", optarg);
                #endif
                benchtime = atoi(optarg); 
                break;
            case 'p':
                #ifdef DEBUG
                    print("case 'p' optarg = %s", optarg);
                #endif
                /* proxy server parsing server:port */
                tmp = strrchr(optarg, ':');
                if(tmp == NULL)
                {
                    break;
                }
                if(tmp == optarg)
                {
                    fprintf(stderr, "Error in option --proxy %s: Missing hostname.\n", optarg);
		            return 2;
                }
                if (tmp == (optarg + strlen(optarg) - 1))
                {
                    fprintf(stderr, "Error in option --proxy %s Port number is missing.\n", optarg);
		            return 2;
                }
                
            default:
                break;
        }
    }
    // 20240313暂时没有编译通过, 先提交, 后面完善

    return 0;
}