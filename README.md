# Proyecto---Line-Production
Aplicación gráfica en C++ con QT Creator y Ubuntu Desktop que permita mostrar una simulación de una línea de producción automatizada de electrodomésticos con múltiples estaciones de trabajo

Notas para el uso de del proyecto:

1. Instalar componentes necesarios para graficar las estadísticas: sudo apt install qt6-charts-dev
     a. Revisar antes version de QT con comando: qmake6 -v o qmake -v o qmake --version. Esto debe decir algo como "Using Qt version 6.6.3 in /usr/lib/x86_64-linux-gnu"
2. Verificamos la ruta donde se instaló con el sguiente comando: which qmake6
3. Luego, abrimos QT CREATOR, edit, preferences, qt versions, add, other locations, computer, buscamos la ruta anterior, Open, click Apply, OK
4. Luego ir al proyecto, dar click en clean > Build o ReBuild

   EN CASO DE NO FUNCIONAR, REVISAR ARCHIVO 'statswindow.h' y probar descomentando una de las siguentes opciones:
   //QT_USE_NAMESPACE;
  //QT_CHARTS_USE_NAMESPACE;
  //using namespace QtCharts;

Y repetir el paso 4. 

Una vez el Build este en verde (Compilado completo), podrás ejecutar el code y probarlo. 
