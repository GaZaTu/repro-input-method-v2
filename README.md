## Building

- `mkdir build`
- `cd build`
- `cmake ..`
- `make`

## Sway

Running this app will try to input the 👌 emoji into the active window using the sway specific wayland protocol **input-method-unstable-v2**.

This works for *gedit*, *alacritty* and *firefox (in XWayland mode)* but it won't work with *vscode (in XWayland mode)*, *any Qt app* or *firefox (with MOZ_ENABLE_WAYLAND=1)*.

**Not working** in this case means this repro app freezes on `wl_display_dispatch` [src/main.c#L109](src/main.c#L109) until I focus the terminal window again in which I started the repro app.

## Gnome

Running this app will try to input the 👌 emoji into the active window using the wayland protocol **text-input-unstable-v3**. (not really yet)

This doesn't work with any app i tested.

**Not working** in this case means this repro app freezes on `wl_display_dispatch` [src/main.c#L134](src/main.c#L134).
