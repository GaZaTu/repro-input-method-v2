Running this code will input the 👌 emoji into the active window using the sway specific wayland protocol **input-method-unstable-v2**.

This works for *gedit*, *alacritty* and *firefox (in XWayland mode)* but it won't work with *vscode (in XWayland mode)*, *any Qt app* or *firefox (with MOZ_ENABLE_WAYLAND=1)*.

**Not working** in this case means this repro app freezes on `wl_display_dispatch` [src/main.c#L76](src/main.c#L76) until I focus the terminal window again in which I started the repro app.
