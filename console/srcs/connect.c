#include "../includes/splendor.h"
#include <stdbool.h>
# define SUCCESS 0;
# define FAILURE 1;
typedef uint64_t Err;
static c_string_vec handles = {0};
static c_string_vec msg;
extern void free_strvec(c_string_vec vec);
extern Err clients_changed(bool * did_change);
extern Err get_client_handles(c_string_vec * client_handles);
extern Err send_message(char * client, char * msg);
extern Err get_messages(char * client, c_string_vec * messages);

// control pad check
#define CP_CHECK(x) do {                                                \
        uint64_t retval = (x);                                          \
        if (retval != 0) {                                              \
            fprintf(stderr, "controlpads error: %s returned %ld as %s:%d", \
                    #x, retval, __FILE__, __LINE__);                    \
        }                                                               \
    } while (0)

c_string_vec *get_connections(void)
{
	bool did_change;

	CP_CHECK(clients_changed(&did_change));
	if (did_change) {
		free_strvec(handles);
		CP_CHECK(get_client_handles(&handles));
		// for (int i = 0; i < handles.len; i++) {
		// 	printf("  %s\n", handles.ptr[i]);
		// }
	}
	else
		return NULL;
	return &handles;
}

char *recv_from(int handle)
{
	 CP_CHECK(get_messages(handles.ptr[handle], &msg));
		if (msg.len > 0) {
			for (int j = 0; j < msg.len; j++) {
				printf("received %s from Handle(%s)\n",msg.ptr[j],  handles.ptr[handle]);
			}
			return msg.ptr[0];
		}
		return NULL;
		// free_strvec(messages);
}

int send_to(char *msg, char *handle)
{
	CP_CHECK(send_message(handle, msg));
}
