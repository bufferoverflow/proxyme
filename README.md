proxyme enable and disables proxy settings for several applications such as git, svn, wget, apt, etc.

It uses the templates and the proxyme.ini file located at ~/.proxyme/
to update the proxy settings of these applications.

proxyme can be combined with [cntlm](http://cntlm.sourceforge.net/) to reduce proxy pain on developer machines.

Build & Install it
-------------------
    git clone https://github.com/bufferoverflow/proxyme.git
    cd proxyme
    mkdir cmake-build  && cd cmake-build
    cmake ..
    make
    sudo make install

Use it
-------
help:

    proxyme -h
    proxyme options:
      -h [ --help ]         produce help message
      --user arg            User      (e.g. DOMAIN\user)
      --password arg        Password  (e.g. 1234)
      --host arg            Host      (e.g. proxy.example.com)
      --port arg (=0)       Port      (e.g. 85)
      -d [ --disable ]      Disable Proxy and overwrite all config files!
      -o [ --overwrite ]    Overwriting files, don't care if they exist!
      -s [ --save ]         Save current parameters within proxyme.ini file
      -u [ --urlencode ]    Store the password in URL encoded form
      --HOME arg            Environment Variable: HOME

disable proxies:

    proxyme -d

enable proxies:

    proxyme --host=proxy.example.com --port=85 --user='DOMAIN\user' --password='1234'

you can use -s option to save your settings within ~/.proxyme/proxyme.ini file

Customize it
-------------
edit the files under ~/.proxyme/ to reflect your other personal settings as well

License
-------
see [LICENSE](LICENSE)
