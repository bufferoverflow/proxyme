proxyme enable and disables proxy settings for several applications such as git, svn, wget, apt, etc.

It uses the templates and the proxyme.ini file located at ~/.proxyme/
to update the proxy settings of these applications

Build & Install it:
-------------------
$ mkdir proxyme 
$ cd proxyme
$ git clone https://github.com/bufferoverflow/proxyme.git
$ mkdir build
$ cd build
$ cmake ../proxyme/
$ make
$ make install

Use it:
-------
help:
$ proxyme -h

disable proxies:
$ proxyme -d

enable proxies:
$ ./proxyme --user='DOMAIN\user' --password=1234 --host=proxy.example.com --port=85 --password='1234'

you can use -s option to save your settings within ~/.proxyme/proxyme.ini file

Customize it:
-------------
edit the files under ~/.proxyme/ to reflect your other personal settings as well
