import tkinter as tk
from tkinter import messagebox
import socket
import threading
import asyncio



currentNumberTest = 0

mySocket = socket.socket()
def serverconnect(ip,port):
    mySocket.connect((ip, int(port)))
    data = mySocket.recv(1024)
    text.insert("end", data.decode())




def thread_Connection(ip,port):
    thread = threading.Thread(target=serverconnect(ip,port))
    thread.daemon = True
    thread.start()



def scroll_text(*args):
    text.yview(*args)


connection = False




def submit(ip,port):

    dialog_window.destroy()
    connection = True
    connection_status_text.config(text= f"Connection status : {connection}")
    thread_Connection(ip,port)



def send_new_line(first_name,second_name,last_name,phone,comment):
    if(first_name == ""):
        first_name = " "
    if (second_name == ""):
        second_name = " "
    if (last_name == ""):
        last_name = " "
    if (phone == ""):
        phone = " "
    if (comment == ""):
        comment = " "

    line = f"ADD_LINE \"{first_name}\";\"{second_name}\";\"{last_name}\";\"{phone}\";\"{comment}\""
    send_message(line)
    dialog_window_newline.destroy()



def send_delete_message(delete_id):
    line = f"DELETE_LINE {delete_id}"
    send_message(line)
    dialog_delete_newline.destroy()





def open_dialog_delete():
    global dialog_delete_newline
    dialog_delete_newline = tk.Toplevel(root)
    dialog_delete_newline.title("Удалить запись")
    delete_name_label = tk.Label(dialog_delete_newline, text="Delete id : ")
    delete_name_label.grid(row=0, column=0, padx=10, pady=5)
    delete_name_entry = tk.Entry(dialog_delete_newline)
    delete_name_entry.grid(row=0, column=1, padx=10, pady=5)
    submit_button = tk.Button(dialog_delete_newline, text="Delete line", command=lambda:send_delete_message(delete_name_entry.get()))
    submit_button.grid(row=1, column=0, columnspan=2, padx=10, pady=10)



def parse_line(line):
    id_entry.delete(0, tk.END)
    first_name_entry.delete(0, tk.END)
    second_name_entry.delete(0, tk.END)
    last_name_entry.delete(0, tk.END)
    phone_entry.delete(0, tk.END)
    comment_entry.delete(0, tk.END)

    a=[]
    word = ""
    for i in range (len(line)):
        if(line[i]== ";" and len(a)<5):
            a.append(word)
            word = ""
        else:
            word+=line[i]
    a.append(word.replace('\r',''))
    #print(a)
    if(a[0] != "Error"):
        id_entry.insert("end",a[0])
        first_name_entry.insert("end",a[1])
        second_name_entry.insert("end", a[2])
        last_name_entry.insert("end", a[3])
        phone_entry.insert("end", a[4])
        comment_entry.insert("end", a[5])
    else:
        id_entry.insert("end", "Empty")

def get_next_line_by_number(first_check):
    global currentNumberTest
    if(first_check == False):
        currentNumberTest += 1
    mySocket.sendall(bytes(f"GET_LINE {currentNumberTest}", 'utf-8'))
    data = mySocket.recv(1024).decode()
    parse_line(data)



def get_prev_line_by_number():
    global currentNumberTest
    if(currentNumberTest >0):
        currentNumberTest -= 1
    mySocket.sendall(bytes(f"GET_LINE {currentNumberTest}", 'utf-8'))
    data = mySocket.recv(1024).decode()
    parse_line(data)




def search_lines_by_arg(type,arg):
    line = "SEARCH_BY "
    if(type=="First name"):
        line+="FIRST_NAME " + arg
    if (type == "Second name"):
        line += "SECOND_NAME " + arg
    if (type == "Last name"):
        line += "LAST_NAME " + arg
    if (type == "Phone"):
        line += "PHONE_NUMBER " + arg
    if (type == "Comment"):
        line += "COMMENT " + arg
    if(arg == ""):
        search_text.insert("end", f"Empty entry!")
    else:
        mySocket.sendall(bytes(line, 'utf-8'))
        data = mySocket.recv(1024)
        search_text.insert("end", f"\n{data.decode()}")

def open_dialog_search():

    global dialog_window_search
    dialog_window_search = tk.Toplevel(root)
    dialog_window_search.title("Поиск записей")

    options = ["First name", "Second name", "Last name", "Phone","Comment"]

    # Переменная для хранения выбранной опции
    variable = tk.StringVar(dialog_window_search)
    variable.set(options[0])  # Устанавливаем первую опцию по умолчанию

    # Создаем поле выбора опций
    option_menu = tk.OptionMenu(dialog_window_search, variable, *options)
    #option_menu.pack()
    option_menu.grid(row = 2,column=0,sticky= "e",padx= 120)

    entry = tk.Entry(dialog_window_search)
    entry.grid(row=1, column=0, padx=10, pady=5)
    global search_text
    search_text = tk.Text(dialog_window_search, wrap="word", height=10, width=60)
    search_text.grid(row=0, column=0, sticky="w", padx=38)

    search_button = tk.Button(dialog_window_search, text="Search by:",command=lambda: search_lines_by_arg(variable.get(),entry.get()))
    search_button.grid(row=2, column=0, columnspan=2, padx=10, pady=10)





def open_dialog_view():
    global dialog_window_view

    dialog_window_view = tk.Toplevel(root)
    dialog_window_view.title("Просмотр записей")

    currentNumber = 0
    if_label = tk.Label(dialog_window_view, text="ID : ")
    if_label.grid(row=0, column=0, padx=10, pady=5)
    global id_entry
    id_entry = tk.Entry(dialog_window_view)
    id_entry.grid(row=0, column=1, padx=10, pady=5)

    # Создание виджетов Entry для ввода данных
    first_name_label = tk.Label(dialog_window_view, text="First name : ")
    first_name_label.grid(row=1, column=0, padx=10, pady=5)
    global first_name_entry
    first_name_entry = tk.Entry(dialog_window_view)
    first_name_entry.grid(row=1, column=1, padx=10, pady=5)

    second_name_label = tk.Label(dialog_window_view, text="Second name : ")
    second_name_label.grid(row=2, column=0, padx=10, pady=5)
    global second_name_entry
    second_name_entry = tk.Entry(dialog_window_view)
    second_name_entry.grid(row=2, column=1, padx=10, pady=5)

    last_name_label = tk.Label(dialog_window_view, text="Last name : ")
    last_name_label.grid(row=3, column=0, padx=10, pady=5)
    global last_name_entry
    last_name_entry = tk.Entry(dialog_window_view)
    last_name_entry.grid(row=3, column=1, padx=10, pady=5)

    phone_label = tk.Label(dialog_window_view, text="Phone : ")
    phone_label.grid(row=4, column=0, padx=10, pady=5)
    global phone_entry
    phone_entry = tk.Entry(dialog_window_view)
    phone_entry.grid(row=4, column=1, padx=10, pady=5)

    comment_label = tk.Label(dialog_window_view, text="Comment : ")
    comment_label.grid(row=5, column=0, padx=10, pady=5)
    global comment_entry
    comment_entry = tk.Entry(dialog_window_view)
    comment_entry.grid(row=5, column=1, padx=10, pady=5)


    get_next_line_by_number(True)
    submit_button = tk.Button(dialog_window_view, text="Previous",command =get_prev_line_by_number)
    submit_button.grid(row=6, column=0, columnspan=2, padx=10, pady=10)
    submit_button = tk.Button(dialog_window_view, text="Next",command=lambda: get_next_line_by_number(False))
    submit_button.grid(row=6, column=2, columnspan=2, padx=10,pady=10)







def open_dialog_newline():
    global dialog_window_newline
    dialog_window_newline = tk.Toplevel(root)
    dialog_window_newline.title("Добавить новую запись")

    # Создание виджетов Entry для ввода данных
    first_name_label = tk.Label(dialog_window_newline, text="First name : ")
    first_name_label.grid(row=0, column=0, padx=10, pady=5)
    first_name_entry = tk.Entry(dialog_window_newline)
    first_name_entry.grid(row=0, column=1, padx=10, pady=5)

    second_name_label = tk.Label(dialog_window_newline, text="Second name : ")
    second_name_label.grid(row=1, column=0, padx=10, pady=5)
    second_name_entry = tk.Entry(dialog_window_newline)
    second_name_entry.grid(row=1, column=1, padx=10, pady=5)

    last_name_label = tk.Label(dialog_window_newline, text="Last name : ")
    last_name_label.grid(row=2, column=0, padx=10, pady=5)
    last_name_entry = tk.Entry(dialog_window_newline)
    last_name_entry.grid(row=2, column=1, padx=10, pady=5)

    phone_label = tk.Label(dialog_window_newline, text="Phone : ")
    phone_label.grid(row=3, column=0, padx=10, pady=5)
    phone_entry = tk.Entry(dialog_window_newline)
    phone_entry.grid(row=3, column=1, padx=10, pady=5)

    comment_label = tk.Label(dialog_window_newline, text="Comment : ")
    comment_label.grid(row=4, column=0, padx=10, pady=5)
    comment_entry = tk.Entry(dialog_window_newline)
    comment_entry.grid(row=4, column=1, padx=10, pady=5)


    # Кнопка для отправки данных
    submit_button = tk.Button(dialog_window_newline, text="Add new line", command=lambda: send_new_line(first_name_entry.get(),second_name_entry.get(),last_name_entry.get(),phone_entry.get(),comment_entry.get()))
    submit_button.grid(row=5, column=0, columnspan=2, padx=10, pady=10)


def open_dialog():
    global dialog_window
    dialog_window = tk.Toplevel(root)
    dialog_window.title("Диалоговое окно")

    # Создание виджетов Entry для ввода данных
    ip_label = tk.Label(dialog_window, text="IP : ")
    ip_label.grid(row=0, column=0, padx=10, pady=5)
    ip_entry = tk.Entry(dialog_window)
    ip_entry.grid(row=0, column=1, padx=10, pady=5)
    ip_entry.insert("end","127.0.0.1")
    port_label = tk.Label(dialog_window, text="PORT : ")
    port_label.grid(row=1, column=0, padx=10, pady=5)
    port_entry = tk.Entry(dialog_window)
    port_entry.insert("end","1234")
    port_entry.grid(row=1, column=1, padx=10, pady=5)

    # Кнопка для отправки данных
    submit_button = tk.Button(dialog_window, text="Connect", command=lambda: submit(ip_entry.get(),port_entry.get()))
    submit_button.grid(row=2, column=0, columnspan=2, padx=10, pady=10)



def send_message(message):
    text.insert("end", f"\n{message}")
    mySocket.sendall(bytes(message, 'utf-8'))
    data = mySocket.recv(1024)
    text.insert("end", f"\n{data.decode()}")




def button_send_message():
    message = command_entry.get()
    command_entry.delete(0,tk.END)
    send_message(message)


root = tk.Tk()
root.title("Phone book client")




    # Создание кнопки
button = tk.Button(root, text="Connect to a server", command=open_dialog)
button.grid(row=0,columnspan = 1,sticky="w",padx= 200)
connection_status_text = tk.Label(root, text=f"Connection status : {connection}")
connection_status_text.grid(row=9, column=0, padx=10, pady=5)

text = tk.Text(root, wrap="word", height=10, width=60)
text.grid(row=5,column=0,sticky="w",padx= 38)
#text.config(state = "disabled")
scrollbar = tk.Scrollbar(root, command=scroll_text)
scrollbar.grid(row = 5,column=1)

command_entry = tk.Entry(root, width= 60)
command_entry.grid(row=7, column=0,sticky="w",padx= 38)

command = command_entry.get()

enter_button = tk.Button(root, text="Enter", command=button_send_message,width= 10)
enter_button.grid(row=7, column=0, sticky="e",padx= 120)

new_line_button = tk.Button(root, text="Add new line", command=open_dialog_newline,width= 10)
new_line_button.grid(row=8, column=0, sticky="w",padx=38)



delete_line_button = tk.Button(root, text="Delete line", command=open_dialog_delete,width= 10)
delete_line_button.grid(row=8, column=0, sticky="w",padx= 120 )


view_line_button = tk.Button(root, text="View line",command = open_dialog_view,width= 10)
view_line_button.grid(row=8, column=0,sticky="w",padx= 200)

search_button = tk.Button(root, text="Search",command = open_dialog_search,width= 10)
search_button.grid(row=8, column=0,sticky="w",padx= 280)


text.config(yscrollcommand=scrollbar.set)

root.geometry("600x300")

root.mainloop()



















