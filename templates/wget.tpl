# {{PROXYME}}
{{#PROXY}}
http_proxy=http://{{#PROXY_AUTH}}{{PROXY_USER}}:{{PROXY_PWD}}@{{/PROXY_AUTH}}{{PROXY_HOST}}:{{PROXY_PORT}}
https_proxy=http://{{#PROXY_AUTH}}{{PROXY_USER}}:{{PROXY_PWD}}@{{/PROXY_AUTH}}{{PROXY_HOST}}:{{PROXY_PORT}}
ftp_proxy=http://{{#PROXY_AUTH}}{{PROXY_USER}}:{{PROXY_PWD}}@{{/PROXY_AUTH}}{{PROXY_HOST}}:{{PROXY_PORT}}
{{/PROXY}}
