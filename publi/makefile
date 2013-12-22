PI-RPI: main.c
	gcc  -Wall -g -O2 -DPUBLICISTA -o Nides main.c main_win.c ../frontends/proyeccion.c ../frontends/proyeccion_vid.c ../backends/carga_biblioteca.c ../backends/carga_imagenes.c ../backends/carga_audio.c ../backends/carga_video.c ../frontends/progressbar.c ../common/pi_xml_parser.c -lgthread-2.0 `xml2-config --cflags` `pkg-config --cflags MagickWand gstreamer-0.10 gtk+-3.0 gmodule-export-2.0 gstreamer-interfaces-0.10` `pkg-config --libs MagickWand gstreamer-0.10 gtk+-3.0 gmodule-export-2.0 gstreamer-interfaces-0.10` `xml2-config --libs`

#-Wl, -export-dynamic
