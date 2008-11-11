#!/usr/bin/python
import sys
import os
import gtk
import gobject
from subprocess import Popen, PIPE
import re

# Something like this. When the user clicks on the filef from the list
# view we will populate the memo view.

SEARCH = "searchInvertedIsam "
basename = ""

def run_grep(gui):
    srch = ' "' + gui.get_search_string() + '" '
    grep_str = SEARCH + basename + " " + srch
          
    p = Popen(grep_str,
              stdout=PIPE, shell=True)
    output = p.stdout.read()
    p.wait()

    matches = output.split('\n')

    gui.clear_list()
    for i in matches:
        if i != "":
            sr = SearchResult(i)
            gui.add_to_list(sr)

def load_file(gui, path, col):
    row = path[0]
    fileName = gui.matching_file(row)
    f = open(fileName,'r')
    buf = f.read()
    f.close()
    gui.display_text(buf)


class SearchResult:
    def __init__(self, search_string):
        #parse the searching
        c = re.compile("(Frequency) (.{8}) (Document) (.+)")
        m = c.match(search_string)
        self.__filename = m.group(4)
        self.__line     = m.group(2)
        self.__match    = self.__filename
        
    def get_file_name(self):
        return self.__filename
    
    def get_line_number(self):
        return self.__line
    
    def get_matching_text(self):
        return self.__match

class Gui:
    def __init__(self):
        self.__search_results = []
        
        self.__win = gtk.Window()
        self.__win.set_title("Indexed ISAM Search")
        self.__win.set_border_width(5)
        self.__win.connect("destroy", gtk.main_quit)

        mainBox = gtk.HPaned()
        self.__win.add(mainBox)

        vbox = gtk.VBox(homogeneous=False)
        vbox.set_spacing(5)
        mainBox.add(vbox)

        self.__memoTxt = gtk.TextBuffer()
        self.__memoTxt.width = 400
        self.__memoTxt.set_text("")
        self.__default_tag = self.__memoTxt.create_tag("default",
                                                       family = "Monospace")
        self.__memo = gtk.TextView(self.__memoTxt)
        scrolled_window = gtk.ScrolledWindow()
        scrolled_window.add_with_viewport(self.__memo)        
        mainBox.add(scrolled_window)

        editBox = gtk.HBox()
        editBox.set_spacing(5)
        vbox.pack_start(editBox,False,False,5)

        #Build list with the files that we found
        self.__listStore = gtk.ListStore(str)
        self.__treeview = gtk.TreeView(self.__listStore)
        cell = gtk.CellRendererText()
        self.__col = gtk.TreeViewColumn("Matching Files")
        self.__col.pack_start(cell, True)
        self.__col.set_attributes(cell,text=0, foreground=2, background=3)

        self.__treeview.append_column(self.__col)
        self.__treeview.connect_object("row-activated", load_file, self)
        scrolled_window = gtk.ScrolledWindow()
        scrolled_window.add_with_viewport(self.__treeview)        
        vbox.add(scrolled_window)

        btnBox = gtk.HBox()
        btnBox.set_spacing(5)
        vbox.pack_start(btnBox,False,False,5)

        label = gtk.Label()
        label.set_text("Search String:")
        editBox.add(label)

        self.__text = gtk.Entry()
        self.__text.width = 400
        editBox.add(self.__text)


        okBtn = gtk.Button()
        okBtn.set_label("Ok")
        okBtn.connect_object("clicked", run_grep, self)
        btnBox.add(okBtn)

        canBtn = gtk.Button()
        canBtn.set_label("Close")
        canBtn.connect_object("clicked", gtk.Widget.destroy, self.__win)

        btnBox.add(canBtn)
        self.__win.resize(800,600)

    def get_search_string(self):
        return self.__text.get_text()

    def run(self):
        self.__win.show_all()
        gtk.main()

    def add_to_list(self, sr):
        self.__search_results.append(sr)
        mt = sr.get_matching_text().lstrip()
        self.__listStore.append([mt])

    def clear_list(self):
        self.__search_results = []
        self.__listStore.clear()

    def matching_file(self, row):
        return self.__search_results[row].get_file_name()


    def display_text(self,buf):
        self.__memoTxt.set_text("")
        self.__memoTxt.insert_with_tags(
            self.__memoTxt.get_end_iter(),
            buf,self.__default_tag)

#main..
progname = sys.argv[0]

if (len(sys.argv) != 2):
    print "Usage:"
    print progname + " Index_base_name"
    sys.exit(1)
else:
    basename = sys.argv[1]
    gui = Gui();
    gui.run()
