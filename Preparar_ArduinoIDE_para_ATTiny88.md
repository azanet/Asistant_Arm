
Preparar el IDE de Arduino para programar el ATTiny88 
(Para distros LINUX)
===============================================================================================



CONFIGURACIÓN ADICIONAL PARA ALGUNOS SISTEMAS 'Linux'
===============================================================================================

Ubuntu y otras distribuciones Linux modernas utilizan udev para gestionar los archivos de dispositivo 
cuando se añaden y eliminan dispositivos USB. 
Por defecto, udev creará un dispositivo con permiso de sólo lectura que no le permitirá descargar código. 
Debe colocar las siguientes reglas udev en un archivo llamado: 
        "/etc/udev/rules.d/49-micronucleus.rules"
Estas reglas son una adaptación de las utilizadas para las placas Teensy, mantenidas por PJRC:

```

# UDEV Rules for Micronucleus boards including the Digispark.
# This file must be placed at:
#
# /etc/udev/rules.d/49-micronucleus.rules    (preferred location)
#   or
# /lib/udev/rules.d/49-micronucleus.rules    (req'd on some broken systems)
#
# After this file is copied, physically unplug and reconnect the board.
#
SUBSYSTEMS=="usb", ATTRS{idVendor}=="16d0", ATTRS{idProduct}=="0753", MODE:="0666"
KERNEL=="ttyACM*", ATTRS{idVendor}=="16d0", ATTRS{idProduct}=="0753", MODE:="0666", ENV{ID_MM_DEVICE_IGNORE}="1"
#
# If you share your linux system with other users, or just don't like the
# idea of write permission for everybody, you can replace MODE:="0666" with
# OWNER:="yourusername" to create the device owned by you, or with
# GROUP:="somegroupname" and mange access using standard unix groups.

```

#Es probable que deba refrescar las reglas,
#Lanzar el siguente comando como 'root' para refrescarlas.
  udevadm control --reload-rules




DEPENDENCIAS
===============================================================================================

# Instalar dependencias:
 sudo apt-get install libusb-dev lib32stdc++6



 

CONFIGURANDO "IDE de Arduino" para "ATTiny88"
===============================================================================================

# 1- Abrimos el IDE de Arduino y nos dirigimos a:
 "File" > "Preferences..." > y en la opción "Additional manager URLs" 
 
 IMAGEN-1
 
# Agregamos el siguiente enlace, para que sepa donde encontrar la libreria 'MH-ET LIVE Boards':
    https://raw.githubusercontent.com/MHEtLive/arduino-boards-index/master/package_mhetlive_index.json
# También podemos agregar el siguiente enlace para obtener soporte para las ESP32:
    https://dl.espressif.com/dl/package_esp32_index.json
# Y También podemos agregar el siguiente enlace para obtener soporte de ATTinyCore:
    http://drazzy.com/package_drazzy.com_index.json
 
 IMAGEN-2
 
 IMAGEN-3
 
 
 
 
   
# Damos "OK" a las ventanas para Salir de estas y se guarde la nueva configuración.



# 2- En este paso procederemos a instalar la librería correspondiente a la Placa Attiny88, 
# para que pueda ser reconocida y programada. Para esto, en el IDE de Arduino nos dirigimos a:
  "Tools" > "Board: " > "Boards Manager..."    (O directamente pulsamos CTRL + MAYUSC + B) 

IMAGEN-4

# Y se nos abrirá la ventana de gestión de las diferentes placas soportadas.
# En esta ventana, nos dirigimos a buscar e introducimos "MH-ET LIVE" y procedemos a instalar el
# paquete que nos muestra el buscador. 


IMAGEN-5


# Una vez instalado, dispondremos de la opción para la Attiny88.
# La encontraremos en: 
 "Tools" > "Boards: " y dentro de la opción "MH-ET LIVE Boards"

IMAGEN-6





ACTUALIZANDO 'micronucleus' EL PROGRAMADOR DEL ATTINY88 A LA ÚLTIMA VERSIÓN
===============================================================================================

# 1- Nos dirigimos al Github de 'micronucleus' y procedemos a descargar la última release 
#    DEBEMOS DESCARGAR la Versión Correspondiente al Sistema Operativo que estemos utilizando
  - https://github.com/micronucleus/micronucleus/releases/tag/v2.6


# 2- Una vez descargada, procedemos a descomprimir el archivo.
#    Accedemos a la carpeta que se ha generado al descomprimir el archivo
#    Y dentro de esta, se debe encontrar la última versión del binario de 'micronucleus'.


# 3- Se necesita "Actualizar" el binario de 'micronucleus' existente en una de las carpetas de Arduino 
#    para poder programar el ATTiny88, por lo que procedemos en primer lugar a renombrar el archivo original para mantenerlo ahí
  mv ~/.arduino15/packages/mhetlive/tools/micronucleus/2.0a4/micronucleus ~/.arduino15/packages/mhetlive/tools/micronucleus/2.0a4/micronucleus_old


# 4- Procedemos a copiar el binario actualizado (Suponiendo que aún nos encontramos dentro de la carpeta indicasa en el punto 2)
  cp ./micronucleus ~/.arduino15/packages/mhetlive/tools/micronucleus/2.0a4/micronucleus


# 5- Listo!! ya solo queda ir a nuestro IDE, Seleccionar la Placa correspondiente y quemar nuestro Sketch

# 5.1- También podemos quemar nuestro Sketch directamente desde el terminal, utilizando el binario de 'micronucleus'
#      se aporta de Ejemplo el siguiente comando:
  ./micronucleus --run Blink.ino.hex



###FUENTES:
Wiki de Digispark, configuraciones,etc -> http://digistump.com/wiki/digispark/tutorials/linuxtroubleshooting
Preparar IDE Arduino para ATTiny88 -> https://robojax.com/learn/arduino/?vid=robojax_MH-Tinny_Tinny88 
ATTinyCore -> https://github.com/SpenceKonde/ATTinyCore/blob/v2.0.0-devThis-is-the-head-submit-PRs-against-this/Installation.md 

