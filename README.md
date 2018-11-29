# news

Enlightenment news reader gadget (rss/atom)


Sceenshot:

 https://www.enlightenment.org/ss/display.php?image=e-5c0026ee41bcb1.15223570.png

working:

- atom/rss feeds
- auto update
- user set feed image
- popup on new Infos
- coloring the feedicons
- showing/hiding Feed Images
- click an image to open the articel in webbrowser
- display/hiding feed images
- set the popup dimension
- set font size
- keyword support
- click to open the articel in webbrowser

not working:

- rdf feeds
- no html support (wont come)
- refresh content of popup with visible popup
- better notification of new infos
- entry bug to set feed url (e bug)

install:

meson build && cd build
meson configure
ninja
sudo ninja install
