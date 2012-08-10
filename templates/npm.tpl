# {{PROXYME}}
{{#PROXY}}
proxy = http://{{#PROXY_AUTH}}{{PROXY_USER}}:{{PROXY_PWD}}@{{/PROXY_AUTH}}{{PROXY_HOST}}:{{PROXY_PORT}}
https-proxy = http://{{#PROXY_AUTH}}{{PROXY_USER}}:{{PROXY_PWD}}@{{/PROXY_AUTH}}{{PROXY_HOST}}:{{PROXY_PORT}}
{{/PROXY}}
