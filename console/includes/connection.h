#include "splendor_structs.h"

typedef struct c_string_vec {
  char **ptr;
  uint64_t len;
  uint64_t cap;
} c_string_vec;

// ~-~-~-~-~-~-~ connection.c ~-~-~-~-~-~-~

c_string_vec 	*get_connections(void);
extern void 	free_strvec(c_string_vec vec);
void 	render_connect_screen(Context *ctx);

char 	*recv_from	(char *handle);
int 	send_to		(char *handle, char *msg);

int sortHandles(const void *a, const void *b);

int connect_handles		(Context *ctx, char **handles, int len);
int disconnect_handles	(Context *ctx, char **handles, int len);
int handle_Connect		(Context *ctx, c_string_vec *new);
