# drone1
Opcion 1 de drone para informatica
Registro de versiones para el entregable 1 de informatica industrial
Se ha implementado una serie de clases que simula el funcionamiento de un drone, con una torre de control
y una cola de informacion entre ambas. El drone recibe ordenes por pantalla, como arrancar, subir, desplazarse etc.
La informacion se almacena en una cola de tamaño 10 mensajes, y en una ilimitada, usada como caja negra. La torre de control se encarga de recivir la ç
informacion de la cola limitada, y sacar los mensajes por pantalla, por otro lado, la cola de registro, se vuelca en un fichero
llamado droneInfo, al finalizar la actividad del aparato
