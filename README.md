# PhoneBookServerCMake

Репозиторий включает в себя проект сервера, написанного на C++ и проект клиента на Python.

# Сервер
По умолчанию, сервер открывает сокет на адресе 127.0.0.1, для того, чтобы это изменить, необходимо передать в конструктор класса другой ip адрес.

Сервер работает в многопоточном режиме где главный поток отвечает за обработку новых подключений, второстепенный поток отвечает за получение новых сообщений, новые потоки вызываются только для обработки и отправки сообщений.

Сервер является кроссплатформенным и выполняется на платформах Windows, Linux, macOS.
# Клиент

Для подключения к серверу в первую очередь необходимо нажать кнопку "Connect", ввести адрес и порт сервера и убедиться что подключение выполнено успешно.

Для взаимодействия с сервером пользователь может использовать кнопки взаимодействия или вводить команды серверу напрямую.

Список команд:

ADD_LINE "FIRST_NAME";"SECOND_NAME";"LAST_NAME";"PHONE_NUMBER";"COMMENT" - добавление новой записи.

DELETE_LINE ID - удаление записи из базы данных.

VIEW_LINE ID - просмотр записи по ID.

GET_LINE NUM - просмотр записи по порядковому номеру в базе данных.

GET_LASTLINE_ID - получить ID последней записи в базе даннных.

SEARCH_BY FIRST_NAME|SECOND_NAME|LAST_NAME|PHONE_NUMBER|COMMENT arg - поиск записи по конкретному атрибуту.



