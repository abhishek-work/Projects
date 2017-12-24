from os import listdir
from tkinter import ttk
import tkinter as tk
from tkinter import messagebox
import subprocess
import requests
import time
import sys
import os


window = tk.Tk()
window.title("Xz downloader")
window.geometry("640x480")
labels = []

#--FUNCTIONS---
def download():
        status1.config(text="Downloading file..")
        window.update_idletasks()
        CHUNK_SIZE = 8192
        bytes_read = 0
        checkdir()
        url = url_field.get()
        r = requests.get(url,allow_redirects=True,stream=True)
        filename = os.path.basename(url)
        path,r,bytes_read = setpath(r,filename,url)
        url = r.url
        total_bytes = len(r.content) + bytes_read
        with open(path,"ab") as file: 
                for chunk in r.iter_content(CHUNK_SIZE):
                        file.write(chunk)
                        bytes_read += len(chunk)
                        progress = 100* float(bytes_read)/float(total_bytes)
                        #print("progress: %d "%(progress))
        r.close()
        status1.config(text="Download completed")
        result = messagebox.askyesno("Download Completed","Do you want to open the file")
        if result == True:
                subprocess.Popen("explorer %s" %path, shell = True)
                
                
        lst_files()
        window.update_idletasks()
def setpath(r,filename,url):
        resume_pos = 0
        file_name,extension = os.path.splitext(filename)
        extension = extension.split("?")[0]
        extension = extension[1:]
        file_name = file_name[:100].split("?")[0]
        content_type = r.headers['content-type']
        file_type,file_extension=content_type.split("/")
        file_type = file_type.title()
        if content_type == "application/octet-stream" or not content_type :
                file_extension = extension
                file_type = "Others"
        if file_extension[0] == "x":
                file_type = file_extension.split("-")[2].title()
                file_extension = file_extension.split("-")[1]
        if file_type == "Program" :
                file_type= "Application"
                file_extension = extension
        file_extension = "." + file_extension
        file_name = file_name + file_extension
        path = os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads",file_type,file_name)
        if os.path.exists(path):
                resume_check,r,resume_pos = resume_download(r,path,url)
                if resume_check == False:
                        initial_path = path
                        file_count = 1;
                        while os.path.exists(path) == True:
                                path = initial_path
                                file_name,extension = os.path.splitext(path)
                                path=file_name + "(" + str(file_count) + ")" + extension
                                file_count = file_count + 1
        return (path,r,resume_pos)

def checkdir():
        if os.path.isdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads")) == False:
                 os.mkdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads"))
        if os.path.isdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads","Application")) == False:
                os.mkdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads","Application"))
        if os.path.isdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads","Image")) == False:
                os.mkdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads","Image"))
        if os.path.isdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads","Audio")) == False:
                os.mkdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads","Audio"))
        if os.path.isdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads","Compressed")) == False:
                os.mkdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads","Compressed"))
        if os.path.isdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads","Others")) == False:
                os.mkdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads","Others"))
        if os.path.isdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads","Video")) == False:
                os.mkdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads","Video"))
        if os.path.isdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads","Text")) == False:
                os.mkdir(os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads","Text"))

def resume_download(r,path,url):
        statinfo = os.stat(path)
        resume_pos = statinfo.st_size
        if resume_pos < len(r.content):
                
                result = messagebox.askyesno("Resume download","Partially downloaded file found, do you want to resume the download ? ")
                if result == True:
                        print("Resuming download...")
                        resume_header = { 'Range' : 'bytes=%d-' % resume_pos}
                        r=requests.get(url,headers = resume_header, allow_redirects = True, stream = True)
                        return (True,r,resume_pos)
                else:
                        raise Exception("User doesn't want to resume the download")
        else:
                result = messagebox.askyesno("Download file again ","Same file found, do you want to download the file again ? ")
                if result == True:
                        print("File already downloaded, downloading again..")
                        resume_pos = 0;
                        return (False,r,resume_pos)
                else:
                        raise Exception("User doesn't want to download the file")

def open_file(dir,file):
        path = os.path.join(dir,file)
        #window.filename = filedialog.askopenfilename(initialdir = path, title = "Open File")
        subprocess.Popen("explorer %s" %dir, shell = True)

def lst_files():
        
        for label in labels:
                label.destroy()
        fields = ["File Type", "File Name", "File Size"]
        col_field=0
        for field in fields:
                field_label = tk.Label(text = field)
                field_label.grid(row=3,column=col_field)
                labels.append(field_label)
                col_field= col_field + 1
        folders = ["Application","Audio","Compressed","Image","Others","Text","Video"]
        rowinc = 6
        for folder in folders:
                dirr = os.path.join(os.path.dirname(os.path.abspath(__file__)),"Downloads",folder)
                files = [f for f in listdir(dirr)]
                
                for file in files:
                        folder_type = tk.Label(text = "%s:"%folder)
                        labels.append(folder_type)
                        folder_type.grid(row=rowinc,column=0)
                        file_label = tk.Label(text = "   %s"%file)
                        labels.append(file_label)
                        file_label.bind("<Button-1>", lambda e, dirr= dirr, file=file:open_file(dirr,file))
                        file_label.grid(row=rowinc,column=1)
                        path = os.path.join(dirr,file)
                        statinfo = os.stat(path)
                        file_size = statinfo.st_size
                        file_size = file_size/1048576
                        file_size_label = tk.Label(text = " %f MB" %file_size)
                        file_size_label.grid(row=rowinc,column=2)
                        labels.append(file_size_label)
                        rowinc = rowinc + 1
                        


        
#-----LABEL-----
title = tk.Label(text="Enter the URL : ")
title.grid(row=0, column=0)
status1= tk.Label()
status1.grid(row=4, column=50)



#-----ENTRY FIELD----
url_field = tk.Entry()
url_field.grid(row=0, columnspan=50)

#----BUTTON-----
download_button = tk.Button(text="Download", command=download)
download_button.grid(row=0,column=4)

#--SEPARAOTS----
dn_separator = ttk.Separator(orient=tk.HORIZONTAL)
dn_separator.grid(row=1,columnspan= 200,sticky="ew")

checkdir()
lst_files()
                

window.mainloop()
