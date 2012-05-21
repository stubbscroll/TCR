oversikt over nehe-tutorialene

nb, 01-04 har en merkelig form for fokus-sjekking. hvis muspekeren er over vinduet, er det aktivt
og redraw skjer. er fjernet fra og med 05.

uløste problemer: hvordan printe til console mens opengl er aktivt

alle lessons har filer som starter på ab* der ab er sifrene i tutorialnummeret.

01 open window, draw black screen. BUG: SDL_VM_ToggleFullScreen() doesn't seem to work.
02 display two white polygons. introduces glBegin(), glVertex(), glEnd().
03 display two coloured polygons. introduces glColor().
04 display two rotating polygons. introduces glRotate().
05 display two rotating 3d-shapes. bugfix by initgl() before resize. fixed fullscreen toggle bug, called init instead of using SDL_VM_ToggleFullScreen().
06 display texturemapped cube. introduces many texture mapping related functions.
07 display different texture filters, ambient light, diffuse light, cube is controllable (f/l/pageup/pagedown/up/down/left/right). checks held down keys in sdl.
08 blending (transparency). use keys (f/l/b/pageup/pagedown/up/down/left/right). need file crate.bmp.
09 display blended stars. uses keys t/pageup/pagedown/up/down. need file star.bmp.
10 load world from external file.
11 display waving flag. introduces display modes for front and back of polygon: glPolygonMode().
12 display lists for faster rendering, introduces glGenLists(), glNewList(), glEndList(), glCallList().
16 display fog. introduces glFog().
17 display text using font defined in texture. also, plot stuff at absolute coordinates (ortho-proj)
18 display quadrics, introduces many glu-functions for drawing quadrics.
19 display particles. for the most part, it uses existing gl routines, but gl triangle strips is new.
20 display masking using a separate mask image
