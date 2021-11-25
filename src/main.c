#include "wayland-input-method-unstable-v1-client-protocol.h"
#include "wayland-input-method-unstable-v2-client-protocol.h"
// #include "wayland-text-input-unstable-v3-client-protocol.h"
// #include "wayland-gtk-text-input-client-protocol.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct registry_listener_data_t {
  struct wl_seat* active_seat;
  struct zwp_input_method_context_v1* input_method_v1_context;
  struct zwp_input_method_manager_v2* input_method_v2_manager;
  // struct zwp_text_input_manager_v3* text_input_manager_v3;
  // struct gtk_text_input_manager* text_input_manager;
};

void handle_registry_listener_global(
    void* _data, struct wl_registry* registry, uint32_t name, const char* interface, uint32_t version) {
  struct registry_listener_data_t* data = (struct registry_listener_data_t*)_data;

  printf("interface: %s\n", interface);

  if (strcmp(interface, wl_seat_interface.name) == 0) {
    data->active_seat =
        (struct wl_seat*)wl_registry_bind(registry, name, &wl_seat_interface, version <= 7 ? version : 7);
  }

  else if (strcmp(interface, zwp_input_method_context_v1_interface.name) == 0) {
    data->input_method_v1_context = (struct zwp_input_method_context_v1*)wl_registry_bind(
        registry, name, &zwp_input_method_context_v1_interface, 1);
  }

  else if (strcmp(interface, zwp_input_method_manager_v2_interface.name) == 0) {
    data->input_method_v2_manager = (struct zwp_input_method_manager_v2*)wl_registry_bind(
        registry, name, &zwp_input_method_manager_v2_interface, 1);
  }

  // else if (strcmp(interface, zwp_text_input_manager_v3_interface.name) == 0) {
  //   data->text_input_manager_v3 =
  //       (struct zwp_text_input_manager_v3*)wl_registry_bind(registry, name, &zwp_text_input_manager_v3_interface, 1);
  // }

  // else if (strcmp(interface, gtk_text_input_manager_interface.name) == 0) {
  //   data->text_input_manager =
  //       (struct gtk_text_input_manager*)wl_registry_bind(registry, name, &gtk_text_input_manager_interface, 1);
  // }
}

void handle_registry_listener_global_remove(void* data, struct wl_registry* registry, uint32_t name) {
}

// struct text_input_v3_listener_data_t {
//   int placeholder;
// };

// void handle_text_input_v3_listener_enter(
//     void* data, struct zwp_text_input_v3* zwp_text_input_v3, struct wl_surface* surface) {
//   printf("handle_text_input_v3_listener_enter\n");
// }

// void handle_text_input_v3_listener_leave(
//     void* data, struct zwp_text_input_v3* zwp_text_input_v3, struct wl_surface* surface) {
//   printf("handle_text_input_v3_listener_leave\n");
// }

// void handle_text_input_v3_listener_preedit_string(void* data, struct zwp_text_input_v3* zwp_text_input_v3,
//     const char* text, int32_t cursor_begin, int32_t cursor_end) {
//   printf("handle_text_input_v3_listener_leave_preedit_string\n");
// }

// void handle_text_input_v3_listener_commit_string(
//     void* data, struct zwp_text_input_v3* zwp_text_input_v3, const char* text) {
//   printf("handle_text_input_v3_listener_leave_commit_string\n");
// }

// void handle_text_input_v3_listener_delete_surrounding_text(
//     void* data, struct zwp_text_input_v3* zwp_text_input_v3, uint32_t before_length, uint32_t after_length) {
//   printf("handle_text_input_v3_listener_delete_surrounding_text\n");
// }

// void handle_text_input_v3_listener_done(void* data, struct zwp_text_input_v3* zwp_text_input_v3, uint32_t serial) {
//   printf("handle_text_input_v3_listener_done\n");
// }

int main(void) {
  printf("focus a different window now\n");
  sleep(5);

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

  struct registry_listener_data_t registry_listener_data = {0};
  struct wl_registry_listener registry_listener;
  registry_listener.global = &handle_registry_listener_global;
  registry_listener.global_remove = &handle_registry_listener_global_remove;

  wl_registry_add_listener(registry, &registry_listener, &registry_listener_data);

  wl_display_dispatch(display);
  wl_display_roundtrip(display);

  printf("connected\n");

  if (!registry_listener_data.active_seat) {
    printf("registry_data.active_seat = NULL\n");
    return -1;
  }

  if (registry_listener_data.input_method_v2_manager) {
    struct zwp_input_method_v2* input_method_v2 = zwp_input_method_manager_v2_get_input_method(
        registry_listener_data.input_method_v2_manager, registry_listener_data.active_seat);

    printf("dispatch zwp_input_method_v2\n");
    wl_display_dispatch(display);

    printf("roundtrip zwp_input_method_v2\n");
    wl_display_roundtrip(display);

    printf("created zwp_input_method_v2\n");

    zwp_input_method_v2_commit_string(input_method_v2, "👌");
    zwp_input_method_v2_commit(input_method_v2, 0);

    printf("dispatch commit_string\n");
    wl_display_dispatch(display);

    printf("roundtrip commit_string\n");
    wl_display_roundtrip(display);

    printf("DONE\n");
    return 0;
  }

  // if (registry_listener_data.text_input_manager) {
  //   struct gtk_text_input* text_input = gtk_text_input_manager_get_text_input(
  //       registry_listener_data.text_input_manager, registry_listener_data.active_seat);

  //   printf("dispatch gtk_text_input\n");
  //   wl_display_dispatch(display);

  //   printf("roundtrip gtk_text_input\n");
  //   wl_display_roundtrip(display);

  //   printf("created gtk_text_input\n");

  //   gtk_text_input_set_surrounding_text(text_input, "TEST", 0, 0);

  //   printf("dispatch commit_string\n");
  //   wl_display_dispatch(display);

  //   printf("roundtrip commit_string\n");
  //   wl_display_roundtrip(display);

  //   printf("DONE\n");
  //   return 0;
  // }

  if (registry_listener_data.input_method_v1_context) {
    printf("xd\n");

    // struct gtk_text_input* text_input = zwp_input_method(
    //     registry_listener_data.input_method_v1_context, registry_listener_data.active_seat);

    // printf("dispatch gtk_text_input\n");
    // wl_display_dispatch(display);

    // printf("roundtrip gtk_text_input\n");
    // wl_display_roundtrip(display);

    // printf("created gtk_text_input\n");

    // gtk_text_input_set_surrounding_text(text_input, "TEST", 0, 0);

    // printf("dispatch commit_string\n");
    // wl_display_dispatch(display);

    // printf("roundtrip commit_string\n");
    // wl_display_roundtrip(display);

    printf("DONE\n");
    return 0;
  }

  // if (registry_listener_data.text_input_manager_v3) {
  //   struct zwp_text_input_v3* text_input_v3 = zwp_text_input_manager_v3_get_text_input(
  //       registry_listener_data.text_input_manager_v3, registry_listener_data.active_seat);

  //   printf("dispatch zwp_text_input_v3\n");
  //   wl_display_dispatch(display);

  //   printf("roundtrip zwp_text_input_v3\n");
  //   wl_display_roundtrip(display);

  //   printf("created zwp_text_input_v3\n");

  //   struct text_input_v3_listener_data_t text_input_v3_listener_data = {0};
  //   struct zwp_text_input_v3_listener listener;
  //   listener.enter = &handle_text_input_v3_listener_enter;
  //   listener.leave = &handle_text_input_v3_listener_leave;
  //   listener.preedit_string = &handle_text_input_v3_listener_preedit_string;
  //   listener.commit_string = &handle_text_input_v3_listener_commit_string;
  //   listener.delete_surrounding_text = &handle_text_input_v3_listener_delete_surrounding_text;
  //   listener.done = &handle_text_input_v3_listener_done;

  //   zwp_text_input_v3_add_listener(&*text_input_v3, &listener, &text_input_v3_listener_data);

  //   printf("dispatch add_listener\n");
  //   wl_display_dispatch(display);

  //   printf("roundtrip add_listener\n");
  //   wl_display_roundtrip(display);

  //   printf("DONE\n");
  //   return 0;
  // }

  printf("ERROR\n");
  return -1;
}
