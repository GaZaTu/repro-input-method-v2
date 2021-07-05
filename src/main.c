#include "wayland-input-method-unstable-v2-client-protocol.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct registry_data_t {
  struct wl_seat* active_seat;
  struct zwp_input_method_manager_v2* input_method_manager_v2;
};

void handle_registry_listener_global(
    void* _data, struct wl_registry* registry, uint32_t name, const char* interface, uint32_t version) {
  struct registry_data_t* data = (struct registry_data_t*)_data;

  printf("interface: %s\n", interface);

  if (strcmp(interface, wl_seat_interface.name) == 0) {
    data->active_seat =
        (struct wl_seat*)wl_registry_bind(registry, name, &wl_seat_interface, version <= 7 ? version : 7);
  }

  if (strcmp(interface, zwp_input_method_manager_v2_interface.name) == 0) {
    data->input_method_manager_v2 = (struct zwp_input_method_manager_v2*)wl_registry_bind(
        registry, name, &zwp_input_method_manager_v2_interface, 1);
  }
}

void handle_registry_listener_global_remove(void* data, struct wl_registry* registry, uint32_t name) {
}

int main(void) {
  printf("focus a different window now\n");
  sleep(5);

  struct registry_data_t registry_data;
  registry_data.active_seat = NULL;
  registry_data.input_method_manager_v2 = NULL;

  struct wl_display* display = wl_display_connect(NULL);
  if (!display) {
    printf("display = NULL\n");
    return -1;
  }

  struct wl_registry* registry = wl_display_get_registry(display);
  if (!registry) {
    printf("registry = NULL\n");
    return -1;
  }

  struct wl_registry_listener registry_listener;
  registry_listener.global = &handle_registry_listener_global;
  registry_listener.global_remove = &handle_registry_listener_global_remove;

  wl_registry_add_listener(registry, &registry_listener, &registry_data);

  wl_display_dispatch(display);
  wl_display_roundtrip(display);

  printf("connected\n");

  if (!registry_data.active_seat) {
    printf("registry_data.active_seat = NULL\n");
    return -1;
  }

  if (!registry_data.input_method_manager_v2) {
    printf("registry_data.input_manager_v2 = NULL\n");
    return -1;
  }

  struct zwp_input_method_v2* input_method_v2 =
      zwp_input_method_manager_v2_get_input_method(registry_data.input_method_manager_v2, registry_data.active_seat);

  printf("dispatch input_method_v2\n");
  wl_display_dispatch(display);

  printf("roundtrip input_method_v2\n");
  wl_display_roundtrip(display);

  printf("created input_method_v2\n");

  zwp_input_method_v2_commit_string(input_method_v2, "👌");
  zwp_input_method_v2_commit(input_method_v2, 0);

  printf("dispatch commit_string\n");
  wl_display_dispatch(display);

  printf("roundtrip commit_string\n");
  wl_display_roundtrip(display);

  printf("done\n");

  return 0;
}
