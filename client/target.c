#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct c_string_vec {
  char **ptr;
  uint64_t len;
  uint64_t cap;
} c_string_vec;

typedef uint64_t Err;
static const Err SUCCESS = 0;
static const Err FAILURE = 1;

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
    

int main() {
    uint64_t err = 0;
    bool did_change = false;
    c_string_vec handles = {0};
    c_string_vec messages = {0};
    char * my_msg = "hello";
    char response_buf[512] = {0};
    int button_presses = 0;
    int did_recv_msg = 0;
    while (1) {

        // changing clients
        CP_CHECK(clients_changed(&did_change));
        if (did_change) {
            printf("Clients changed:\n");
            free_strvec(handles);
            CP_CHECK(get_client_handles(&handles));
            for (int i = 0; i < handles.len; i++) {
                printf("  %s\n", handles.ptr[i]);
            }
        }

        // print inbound messages
        for (int i = 0; i < handles.len; i++) {
            CP_CHECK(get_messages(handles.ptr[i], &messages));
            if (messages.len > 0) {
                did_recv_msg = 1;
                for (int j = 0; j < messages.len; j++) {
                    button_presses++;
                    printf("Button press from %s\n", handles.ptr[i]);
                }
            }
            free_strvec(messages);
        }

        // send a message to each client
        if (did_recv_msg) {
            sprintf(response_buf, "Buttons have been pressed %d times", button_presses);
            for (int i = 0; i < handles.len; i++) {
                CP_CHECK(send_message(handles.ptr[i], response_buf));
            }
            did_recv_msg = 0;
        }

        // sleep for a tenth of a second
        sleep(0.1);
    }

    return 0;
}
