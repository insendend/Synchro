# Synchro (Синхронизация двух каталогов)

# Задание
Написать программу синхронизации двух дисков/каталогов. При запуске программа должна принять путь к одному и второму диску/каталогу. Выбрать какой главный, какой синхронизируемый. После сихронизации синхронизируемый диск/каталог должен быть по содержимому аналогичный главному. Для этого можно использовать размер файла и дату изменения.

# Реализация
IDE: MSVC 2015
LANG: C

# Описание приложения
Программа синхронизации двух директорий.

1. Выбрать режим синхронизации (автоматический или выбрать пути к каталогам)
   1.1. Авто-режим. Пути к директориям будут взяты с файла "paths.txt", 
	который расположен в папке с проэктом
   1.2. Режим ввода путей к каталогам. Можно вручную ввести пути к каталогам.

2. Выбрать трубеумое действие над каталогами.
   2.1. Заменить локальную информацию. Замена файлов/каталогов в "локальном" каталоге на информацию
	из "серверного" каталога.
   2.2. Заменить серверную информацию. Замена файлов/каталогов в "серверном" каталоге на информацию
	из "локального" каталога.
   2.3. Объеденить содержимое обоих каталогов и сохранить результат в "локальной" папке.
   2.4. Объеденить содержимое обоих каталогов и сохранить результат в "серверной" папке.

3. Все изменения будут записаны в "Logs.txt", который находиться в папке с проэктом и будет открыт
   после завершения программы.


# Проверено
Проверено на копировании (если выбрать в качестве синхронизируемого каталога - пустую папку, а в качестве главного - копируемую)
Папка содержала 10.7 Гб (Файлов: 8 375; Папок: 1 152)
Результат:
ЦП ~ 0.5% - 1.8%
Память - 0.8 MB
HDD - 49% (45-100 MB/s)
Скопированы все данные
Время: ~7мин